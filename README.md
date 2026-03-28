# Video Frame Interpolation Studio - Web UI

A modern web interface for your advanced video frame interpolation tool. This UI provides a professional-grade experience for processing videos with slow motion effects and super-resolution enhancement.

## Features

✨ **Web-Based Interface**

- Modern, responsive design
- Real-time video upload and processing
- Beautiful UI with animations
- Mobile-friendly layout

🎬 **Video Processing**

- Slow Motion Effects (2x to 8x)
- Super Resolution (1.0x to 3.0x)
- Advanced Quality Enhancement
- Professional MP4 Output

📖 **Navigation**

- Home page for video processing
- About page with detailed information
- Responsive navigation bar

## Project Structure

```text
Video Frame Interpolation/
├── public/
│   ├── index.html          # Home page
│   ├── about.html          # About page
│   ├── styles.css          # CSS styling
│   └── script.js           # Frontend JavaScript
├── input/                  # Input video storage
├── output/                 # Processed video output
├── server.js               # Express server
├── package.json            # Node.js dependencies
├── Video Frame Interpolation.cpp  # Modified C++ source
└── Video Frame Interpolation.exe  # Compiled executable
```

## Setup Instructions

### Prerequisites

- Node.js (v14 or higher)
- npm (comes with Node.js)
- C++ executable already compiled: `Video Frame Interpolation.exe`

### Step 1: Install Node.js Dependencies

Open command prompt in the project directory and run:

```bash
npm install
```

This will install:

- **express**: Web server framework
- **multer**: File upload handling
- **body-parser**: Request body parsing

### Step 2: Ensure C++ Executable is Built

Make sure you have the compiled `Video Frame Interpolation.exe` in the project root directory. If not, rebuild it using:

```bash
cd /d "D:\Video Frame Interpolation\Video Frame Interpolation"
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe" /EHsc /std:c++17 /I"D:\opencv\build\include" /I"D:\opencv\build\include\opencv2" "Video Frame Interpolation.cpp" /link /LIBPATH:"D:\opencv\build\x64\vc16\lib" opencv_world4120.lib /OUT:"Video Frame Interpolation.exe"
```

Or use the VS Code build task: `Build Video Frame Interpolation`

### Step 3: Start the Web Server

Run the server with:

```bash
npm start
```

Or if you prefer:

```bash
node server.js
```

### Step 4: Access the Web Interface

Open your browser and navigate to:

```text
http://localhost:3000
```

You should see the Video Frame Interpolation Studio home page.

## Usage

### Processing a Video

1. **Navigate to Home**: Click "Home" in the navigation bar (or go to `http://localhost:3000`)

2. **Upload Video**:
   - Click the upload area or drag-and-drop your video
   - Supported formats: MP4, AVI, MOV, MKV, FLV, WMV

3. **Configure Settings**:
   - **Slow Motion Factor**: Select 2x to 8x (use slider or preset buttons)
   - **Resolution Scale**: Choose from 1.0x to 3.0x
   - Quality enhancements are automatically applied

4. **Process Video**:
   - Click "Start Processing" button
   - Wait for processing to complete (time varies based on video length and settings)

5. **Download Result**:
   - When complete, click "Download Enhanced Video"
   - Video will be saved to your downloads folder

### Viewing Information

Click **"About"** in the navigation bar to view:

- Detailed information about the project
- Technical specifications
- How the algorithms work
- Tips for best results

## Configuration Options

### Slow Motion Factor

- **2x**: Subtle slow motion (best for action scenes)
- **4x**: Moderate slow motion (general purpose)
- **8x**: Extremely slow motion (for dramatic effects)

### Resolution Scale

- **1.0x**: Original resolution (no upscaling)
- **1.5x**: 50% increase in resolution
- **2.0x**: 100% increase in resolution (2x)
- **2.5x**: 150% increase in resolution
- **3.0x**: 200% increase in resolution (up to 4K)

### Quality Features (Always Enabled)

- ✨ Enhanced Sharpness & Clarity
- 🎨 Improved Contrast
- 🔊 Noise Reduction
- ⚡ Advanced Optical Flow

## Output Location

Processed videos are saved to the `output/` directory with a timestamp filename:

- Format: `output_[timestamp].mp4`
- Available for direct download from the UI

Input videos are stored in the `input/` directory for reference.

## Troubleshooting

### Issue: Server won't start

- Make sure Node.js is installed: `node --version`
- Check if port 3000 is available
- Try running as administrator

### Issue: Video processing fails

- Verify the C++ executable exists in the project directory
- Check that OpenCV DLLs are in the same directory
- Try a different video format
- Ensure input video is not corrupted

### Issue: Download doesn't work

- Check browser's download settings
- Ensure output directory has write permissions
- Try right-clicking the download button and selecting "Save Link As"

### Issue: Port 3000 already in use

- Change the port in `server.js`:

```javascript
const PORT = 3001;  // or any available port
```

## Advanced Configuration

### Change Server Port

Edit `server.js`:

```javascript
const PORT = 3000;  // Change this number
```

Then access at: `http://localhost:3000` (replace with your new port number)

### Adjust File Upload Limits

In `server.js`, modify the multer configuration to change size limits:

```javascript
const upload = multer({
    storage: storage,
    limits: {
        fileSize: 2 * 1024 * 1024 * 1024  // 2GB in bytes
    },
    // ... rest of configuration
});
```

### Custom Video Codec Settings

Modify the C++ code in the `VideoWriter` section to adjust codec parameters.

## Performance Tips

1. **Start with smaller videos** to test settings
2. **Use 2-4x slow motion** for most content
3. **Use 1.5-2x resolution** for best quality-to-performance ratio
4. **Close other applications** during processing for faster speeds
5. **Use SSD storage** for faster file I/O

## File Cleanup

To clean up old processed videos:

```bash
# Windows
rmdir /S output

# Then recreate for next use
mkdir output
```

## Browser Support

- Chrome 60+
- Firefox 55+
- Safari 11+
- Edge 79+

Best performance on Chrome and Edge (Chromium-based).

## System Requirements

### Minimum

- CPU: Dual-core processor
- RAM: 4GB
- Storage: 5GB free space
- Network: Not required (runs locally)

### Recommended

- CPU: Quad-core or better
- RAM: 8GB+
- Storage: 20GB+ free space
- GPU: NVIDIA CUDA capable (for acceleration, if added)

## API Endpoints

### POST `/api/process-video`

Upload and process a video.

**Parameters:**

```text
- video: File (multipart/form-data)
- slowFactor: Integer (2-8)
- resolutionScale: Float (1.0-3.0)
```

**Response:**

```json
{
  "success": true,
  "message": "Video processed successfully",
  "outputFile": "output_1234567890.mp4",
  "outputPath": "/download/output_1234567890.mp4"
}
```

### GET `/download/:filename`

Download the processed video file.

### GET `/api/status/:filename`

Check the status of a processed video.

## License

This project is provided as-is for personal use.

## Support

For issues or questions:

1. Check the troubleshooting section above
2. Review the About page for technical details
3. Check browser console for JavaScript errors (F12)
4. Check server console for backend errors

## Version History

**v1.0.0** (Current)

- Initial web UI release
- Full integration with C++ video processor
- Professional UI with responsive design
- About page with detailed information

---

## Happy Video Processing

🎬
