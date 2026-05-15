# Video Frame Interpolation Studio

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE) [![Node.js](https://img.shields.io/badge/node-18+-brightgreen.svg)](https://nodejs.org/) [![Python](https://img.shields.io/badge/python-3.9+-blue.svg)](https://www.python.org/)

A professional video enhancement application with frame interpolation, slow-motion effects, and super-resolution upscaling. Built with Node.js, Express, Python, and OpenCV.

**🌐 Live Demo**:https://video-frame-interpolation.onrender.com

---

## ✨ Features

- **Frame Interpolation**: Create smooth slow-motion videos (2x - 8x)
- **Super-Resolution**: Upscale videos up to 3x resolution
- **Quality Enhancement**: Sharpening, contrast boost, noise reduction
- **Web UI**: Modern, responsive interface
- **File Management**: Drag-and-drop upload, direct download
- **API Ready**: REST endpoints for automation
- **Free Hosting**: Deploy on Render.com at no cost

---

## 🎯 What This Demonstrates

- Full-stack web application (Node.js + Python)
- Video processing pipeline
- File upload/download workflows
- RESTful API design
- CI/CD automation with GitHub Actions
- Cloud deployment

Perfect for portfolios and technical interviews!

---

## 🚀 Quick Start (Local Development)

### Prerequisites

- **Node.js** 18+ ([Download](https://nodejs.org/))
- **Python** 3.9+ ([Download](https://www.python.org/))
- **Git** ([Download](https://git-scm.com/))

### Installation

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/Video-Frame-Interpolation.git
cd Video-Frame-Interpolation

# Install Node.js dependencies
npm install

# Install Python dependencies
pip install -r requirements.txt

# Start the server
npm start
```

Open your browser and go to: **http://localhost:3000**

---


## 📋 Project Structure

```
Video-Frame-Interpolation/
├── public/                      # Frontend files
│   ├── index.html              # Main page
│   ├── about.html              # About page
│   ├── styles.css              # Styling
│   └── script.js               # Frontend logic
├── src/                         # Source files
│   └── VideoFrameInterpolation.cpp
├── .github/
│   └── workflows/
│       └── render-deploy.yml   # CI/CD pipeline
├── input/                       # Uploaded videos (temporary)
├── output/                      # Processed videos (temporary)
├── server.js                    # Express server
├── video_processor.py           # Python video processing
├── requirements.txt             # Python dependencies
├── render.yaml                  # Render deployment config
├── package.json                 # Node.js configuration
├── LICENSE                      # MIT License
└── README.md                    # This file
```

---

## 📖 How to Use

1. **Open the app** in your web browser
2. **Upload a video** - Drag & drop or click to select
3. **Choose enhancement settings**:
   - Slow motion factor (2x - 8x)
   - Resolution scale (1.0x - 3.0x)
   - Quality options (sharpening, contrast, etc.)
4. **Process** - Click the "Process" button
5. **Download** - Download your enhanced video

---

## 🎥 Supported Video Formats

- MP4 (H.264)
- AVI
- MOV (QuickTime)
- MKV
- FLV
- WMV

**Recommended**: MP4 for best compatibility

---

## 🔌 API Reference

### POST `/api/process-video`

Process a video with enhancement options.

**Request:**
```bash
curl -X POST http://localhost:3000/api/process-video \
  -F "video=@input.mp4" \
  -F "slowFactor=2" \
  -F "resolutionScale=1.5" \
  -F "enableSharpening=1"
```

**Parameters:**
| Parameter | Type | Range | Default |
|-----------|------|-------|---------|
| video | File | - | Required |
| slowFactor | Integer | 2-8 | 2 |
| resolutionScale | Float | 1.0-3.0 | 1.0 |
| enableSharpening | 0/1 | - | 1 |
| enableContrast | 0/1 | - | 1 |
| enableNoisReduction | 0/1 | - | 1 |
| enableEdgeUpscale | 0/1 | - | 1 |

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

Download a processed video file.

```bash
curl http://localhost:3000/download/output_1234567890.mp4 > output.mp4
```

### GET `/api/status/:filename`

Check if a video has been processed.

```bash
curl http://localhost:3000/api/status/output_1234567890.mp4
```

---

## 📦 Tech Stack

### Backend
- **Node.js** - JavaScript runtime
- **Express.js** - Web framework
- **Multer** - File upload handling

### Video Processing
- **Python** 3.9+
- **OpenCV** - Computer vision library
- **NumPy** - Numerical computing
- **SciPy** - Scientific computing

### Frontend
- **HTML5** - Structure
- **CSS3** - Styling
- **Vanilla JavaScript** - Interactivity

### Deployment
- **Render.com** - Free hosting
- **GitHub Actions** - CI/CD automation

---

## 📋 Dependencies

### Node.js (`package.json`)
```json
{
  "express": "^4.18.2",
  "multer": "^1.4.5-lts.1"
}
```

### Python (`requirements.txt`)
```
opencv-python>=4.5.0
numpy>=1.19.0
scipy>=1.5.0
scikit-image>=0.17.0
```

---

## 🖥️ System Requirements

| Requirement | Minimum | Recommended |
|-------------|---------|-------------|
| Node.js | 18+ | 18+ |
| Python | 3.9+ | 3.9+ |
| RAM | 512MB | 1GB+ |
| Storage | 1GB | 5GB+ |
| OS | Windows/macOS/Linux | Any |

---

## 🔒 Security Notes

### For Production Deployment:
- Add HTTPS/SSL (Render provides this automatically)
- Implement rate limiting
- Add authentication if needed
- Sanitize file uploads
- Set maximum file size limits
- Regular security updates

---

## 🐛 Troubleshooting

### "Python not found"
```bash
# Install Python dependencies
pip install -r requirements.txt
```

### "Port 3000 already in use"
```bash
# Use a different port
PORT=3001 npm start
```

### "Video processing timeout"
- Use smaller video files
- Upgrade server resources
- Reduce processing parameters

### "Out of memory"
- Process smaller videos
- Upgrade server RAM
- Upgrade plan tier

---

## 💡 For Recruiters & Hiring Managers

This project demonstrates:

✅ Full-stack development (frontend + backend)
✅ Multiple technologies integration (JavaScript + Python)
✅ Video processing pipeline
✅ RESTful API design
✅ File upload/download workflows
✅ Cloud deployment & CI/CD
✅ Production-ready code structure
✅ Comprehensive documentation

---

## 📝 License

This project is licensed under the **MIT License** - see [LICENSE](LICENSE) file for details.

---

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report issues
- Submit pull requests
- Suggest improvements

---

## 📞 Support

- **Issues**: Create a GitHub issue
- **Questions**: Check documentation files

---
---

**Made with ❤️ for video processing enthusiasts**

*Last updated: May 2026*
