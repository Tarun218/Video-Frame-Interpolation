const express = require('express');
const multer = require('multer');
const path = require('path');
const { execSync } = require('child_process');
const fs = require('fs');
const os = require('os');

const app = express();
const PORT = process.env.PORT || 3000;
const HOST = '0.0.0.0';  // Accept connections from any network interface

// Configure multer for file uploads
const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        const inputDir = path.join(__dirname, 'input');
        if (!fs.existsSync(inputDir)) {
            fs.mkdirSync(inputDir, { recursive: true });
        }
        cb(null, inputDir);
    },
    filename: (req, file, cb) => {
        cb(null, Date.now() + path.extname(file.originalname));
    }
});

const upload = multer({
    storage: storage,
    fileFilter: (req, file, cb) => {
        const allowedMimes = ['video/mp4', 'video/avi', 'video/quicktime', 'video/x-msvideo'];
        const allowedExts = ['.mp4', '.avi', '.mov', '.mkv', '.flv', '.wmv'];
        
        const ext = path.extname(file.originalname).toLowerCase();
        
        if (allowedExts.includes(ext)) {
            cb(null, true);
        } else {
            cb(new Error('Only video files are allowed'), false);
        }
    }
});

// Middleware
app.use(express.json());
app.use(express.static('public'));
app.use(express.urlencoded({ extended: true }));

// Routes
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.get('/about', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'about.html'));
});

// API endpoint to process video
app.post('/api/process-video', upload.single('video'), (req, res) => {
    try {
        if (!req.file) {
            return res.status(400).json({ error: 'No video file uploaded' });
        }

        const slowFactor = parseInt(req.body.slowFactor) || 2;
        const resolutionScale = parseFloat(req.body.resolutionScale) || 1.0;
        const enableSharpening = parseInt(req.body.enableSharpening) || 1;
        const enableContrast = parseInt(req.body.enableContrast) || 1;
        const enableNoisReduction = parseInt(req.body.enableNoisReduction) || 1;
        const enableEdgeUpscale = parseInt(req.body.enableEdgeUpscale) || 1;
        
        // Combine flags into single parameter: bit 0=sharpening, bit 1=contrast, bit 2=noise, bit 3=edge
        const qualityFlags = (enableSharpening * 1) + (enableContrast * 2) + (enableNoisReduction * 4) + (enableEdgeUpscale * 8);
        
        // Validate input
        if (slowFactor < 2 || slowFactor > 8) {
            return res.status(400).json({ error: 'Slow factor must be between 2 and 8' });
        }
        if (resolutionScale < 1.0 || resolutionScale > 3.0) {
            return res.status(400).json({ error: 'Resolution scale must be between 1.0 and 3.0' });
        }

        const inputPath = req.file.path;
        const outputFileName = `output_${Date.now()}.mp4`;
        const outputPath = path.join(__dirname, 'output', outputFileName);

        // Create output directory if it doesn't exist
        const outputDir = path.join(__dirname, 'output');
        if (!fs.existsSync(outputDir)) {
            fs.mkdirSync(outputDir, { recursive: true });
        }

        // Execute the C++ program with arguments
        const exePath = path.join(__dirname, 'Video Frame Interpolation.exe');
        
        // Validate the executable exists
        if (!fs.existsSync(exePath)) {
            return res.status(500).json({ 
                error: 'C++ executable not found. Please ensure Video Frame Interpolation.exe is compiled and in the project directory.' 
            });
        }

        const command = `"${exePath}" "${inputPath}" "${outputPath}" ${slowFactor} ${resolutionScale} ${qualityFlags}`;

        console.log(`Executing: ${command}`);
        
        // Execute directly without setTimeout wrapper
        try {
            execSync(command, { 
                timeout: 30 * 60 * 1000,
                stdio: 'pipe'
            });

            // Check if output file exists
            if (fs.existsSync(outputPath)) {
                res.json({
                    success: true,
                    message: 'Video processed successfully',
                    outputFile: outputFileName,
                    outputPath: `/download/${outputFileName}`
                });
                
                // Clean up input file
                setTimeout(() => {
                    try {
                        fs.unlinkSync(inputPath);
                        console.log(`Cleaned up input file: ${inputPath}`);
                    } catch (e) {
                        console.warn(`Could not delete input file: ${e.message}`);
                    }
                }, 1000);
            } else {
                res.status(500).json({ error: 'Output file not created after processing completed' });
            }
        } catch (error) {
            console.error('Processing error:', error.message);
            res.status(500).json({ error: 'Video processing failed: ' + error.message });
        }

    } catch (error) {
        console.error('Upload error:', error);
        res.status(500).json({ error: 'File upload failed: ' + error.message });
    }
});

// Download endpoint
app.get('/download/:filename', (req, res) => {
    const filename = req.params.filename;
    const filepath = path.join(__dirname, 'output', filename);

    try {
        if (!fs.existsSync(filepath)) {
            return res.status(404).json({ error: 'File not found' });
        }

        res.download(filepath, (err) => {
            if (err) {
                console.error('Download error:', err.message);
            }
        });
    } catch (error) {
        console.error('Download endpoint error:', error.message);
        res.status(500).json({ error: 'Download failed' });
    }
});

// Get processing status
app.get('/api/status/:filename', (req, res) => {
    const filename = req.params.filename;
    const filepath = path.join(__dirname, 'output', filename);

    try {
        if (fs.existsSync(filepath)) {
            const stats = fs.statSync(filepath);
            res.json({
                exists: true,
                size: stats.size,
                createdAt: stats.birthtime
            });
        } else {
            res.json({ exists: false });
        }
    } catch (error) {
        console.error('Status check error:', error.message);
        res.status(500).json({ error: 'Failed to check file status' });
    }
});

// Start server
app.listen(PORT, HOST, () => {
    // Get local IP address
    const interfaces = os.networkInterfaces();
    let localIP = 'localhost';
    
    for (const name of Object.keys(interfaces)) {
        for (const iface of interfaces[name]) {
            // Skip internal and non-IPv4 addresses
            if (iface.family === 'IPv4' && !iface.internal) {
                localIP = iface.address;
                break;
            }
        }
    }

    console.log(`
╔═══════════════════════════════════════════════════╗
║  VIDEO FRAME INTERPOLATION STUDIO - WEB SERVER    ║
╚═══════════════════════════════════════════════════╝

✓ Server is RUNNING and READY for connections!

📍 ACCESS YOUR APPLICATION:

  Local Machine:
  • http://localhost:${PORT}
  • http://127.0.0.1:${PORT}

  Network Access:
  • http://${localIP}:${PORT}
  • Share this URL with other devices on your network

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

🎬 Ready to process videos!
   Upload videos, select enhancement options, and download results.

⚠️  Press Ctrl+C to stop the server

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    `);
});
