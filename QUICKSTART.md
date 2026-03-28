# Quick Start Guide - Video Frame Interpolation Studio

## 🚀 Get Started in 3 Steps

### Step 1: Install Dependencies
```bash
npm install
```

### Step 2: Build the C++ Program (if needed)
Use the "Build Video Frame Interpolation" task in VS Code, or run:
```bash
cl.exe /EHsc /std:c++17 /I"D:\opencv\build\include" "src\VideoFrameInterpolation.cpp" /link /LIBPATH:"D:\opencv\build\x64\vc16\lib" opencv_world4120.lib
```

### Step 3: Start the Server
```bash
npm start
```

Then open your browser to: **http://localhost:3000**

---

## 🎬 Processing Your First Video

1. **Drag & Drop or Click** to upload a video file
2. **Select Slow Motion Factor** (2x to 8x) using the slider
3. **Choose Resolution Scale** (1.0x to 3.0x)
4. **Click "Start Processing"** and wait
5. **Download** your enhanced video when ready!

---

## 📚 Page Guide

### Home Page (`/`)
- Main video processing interface
- Upload videos, configure settings
- View processing status and download results
- Real-time feedback with progress tracking

### About Page (`/about`)
- Project overview and features
- How the technology works
- Technical specifications
- Tips for best results
- Algorithm explanations

---

## ⚙️ Default Settings Explained

| Setting | Default | Range | Description |
|---------|---------|-------|-------------|
| Slow Motion | 2x | 2x - 8x | Video playback speed reduction |
| Resolution | 1.0x | 1.0x - 3.0x | Output resolution multiplier |
| Quality | Auto | N/A | Sharpness + Contrast + Noise Reduction |

### Recommended Combinations

- **Action Videos**: 4x slow motion, 1.5x resolution
- **Nature/Slow Content**: 2x slow motion, 2.0x resolution  
- **Sports**: 6x slow motion, 1.5x resolution
- **High Quality Output**: 2x slow motion, 3.0x resolution

---

## 📁 Folder Structure

```
Video Frame Interpolation/
├── public/              # Web interface files
│   ├── index.html       # Home page
│   ├── about.html       # About page
│   ├── styles.css       # Styling
│   └── script.js        # Interactivity
├── src/                 # C++ source code
│   └── VideoFrameInterpolation.cpp
├── input/               # Input videos (auto-created)
├── output/              # Processed videos (auto-created)
├── server.js            # Node.js web server
├── package.json         # Dependencies
├── package-lock.json    # Installed dependency tree
├── .gitignore           # Ignore local artifacts
└── README.md            # Full documentation
```

---

## 🔧 Troubleshooting

### Port Already in Use
Change the port in `server.js` (line with `const PORT = 3000`) to a different number.

### Can't Find C++ Executable
Make sure you have built the C++ program and `Video Frame Interpolation.exe` exists in the project root.

### Video Upload Fails
- Check file format (MP4, AVI, MOV supported)
- Verify video is not corrupted
- Check file size (max 2GB)

### Processing Takes Too Long
- This is normal! Video processing is CPU intensive
- Close other applications
- Try a shorter video first to test

---

## 🌐 Browser Tips

- Use **Chrome** or **Edge** for best performance
- Modern browsers recommended (2020+)
- JavaScript must be enabled
- Enable pop-ups if download doesn't work

---

## 📞 Need Help?

1. Check the **About** page for technical info
2. See full **README.md** for advanced configuration
3. Review browser console (F12) for error messages
4. Check Node.js console for processing errors

---

## 🎯 Pro Tips

✓ **Start with fast settings** (2x slow, 1.5x res) to test
✓ **Use high-quality input videos** for best output
✓ **Let processing finish completely** - don't close the browser
✓ **One video at a time** for best performance
✓ **Backup your input videos** before processing

---

## 📊 What to Expect

- **Small videos (< 100MB)**: 5-10 minutes processing time
- **Medium videos (100-500MB)**: 20-40 minutes
- **Large videos (> 500MB)**: 60+ minutes
- **2x slow motion + 2x resolution**: ~30 minutes for 100MB video

Processing time varies based on:
- Video length
- Resolution scale
- Slow motion factor
- Your computer's CPU

---

## ✨ Quality Features (Always On)

Every video gets automatically enhanced with:
- 🎯 Advanced optical flow frame interpolation
- ✨ Sharpness enhancement
- 🎨 Contrast improvement
- 🔊 Noise reduction
- 📈 Adaptive contrast stretching

---

**Ready to create amazing slow-motion videos? Let's go! 🎬**

Visit: **http://localhost:3000**
