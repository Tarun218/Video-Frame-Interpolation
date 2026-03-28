# Run as Web Application - Setup Guide

## 🚀 Quick Start (3 Steps)

### 1. Install Dependencies (First Time Only)
```bash
npm install
```

### 2. Start the Web Server
```bash
npm start
```

### 3. Access Your Application

**On your computer:**
- http://localhost:3000

**From other devices on your network:**
- Open command prompt and run: `ipconfig`
- Find your IPv4 Address (usually 192.168.x.x)
- Use: http://192.168.1.100:3000 (replace with your IP)

---

## 📊 Server Features

✅ **Network Accessible** - Works from any device on your network
✅ **Auto IP Detection** - Shows network IP on startup
✅ **Process Management** - Use PM2 for production deployments
✅ **Production Ready** - Proper error handling and logging
✅ **Scalable** - Can handle multiple users simultaneously

---

## 💻 Running Methods

### Method 1: Simple (Recommended for Testing)
Double-click: `start.bat`

### Method 2: From Terminal
```bash
npm start
```

### Method 3: Production with PM2
```bash
npm install -g pm2
npm run pm2-start
```

To view status:
```bash
pm2 list
```

---

## 🌐 Accessing Your Web App

### From Your Machine
- **Localhost**: http://localhost:3000
- **Loopback**: http://127.0.0.1:3000

### From Other Computers on Network

1. Get your computer's IP address:
   ```bash
   ipconfig
   ```
   Look for "IPv4 Address" line

2. Share the URL with others:
   - Format: `http://[YOUR_IP]:3000`
   - Example: `http://192.168.1.50:3000`

### From Internet (Advanced)
- Set up port forwarding on your router
- Use a service like ngrok for tunneling
- Use a VPN for secure remote access

---

## 🛠️ Server Details

**What's Running:**
- Express.js web server
- RESTful API for video processing
- Static file serving for UI
- Multer for file uploads
- Integration with C++ processor

**Ports:**
- Default: 3000
- Change with: `set PORT=8000` then `npm start`

**Upload Handling:**
- Max file size: 2GB (configurable)
- Supported formats: MP4, AVI, MOV, MKV, FLV, WMV
- Files stored in: `input/` directory
- Results saved in: `output/` directory

---

## 📈 Performance Tips

1. **Use Modern Browser:**
   - Chrome 90+
   - Firefox 88+
   - Edge 90+
   - Safari 14+

2. **Network:**
   - Ethernet is faster than WiFi for large uploads
   - 5GHz WiFi is better than 2.4GHz

3. **Processing:**
   - Process one video at a time for best speed
   - Close other applications
   - Use faster settings for testing

4. **Server:**
   - With PM2, server auto-restarts on crashes
   - Monitor with: `pm2 monit`
   - View logs with: `pm2 logs`

---

## 🔒 Security Notes (Production)

For deploying on the internet, add:

1. **HTTPS/SSL Certificates**
   - Use Let's Encrypt (free)
   - Set up with Nginx reverse proxy

2. **Authentication**
   - Add user login
   - Secure API endpoints

3. **Rate Limiting**
   - Prevent abuse
   - Throttle uploads

4. **Input Validation**
   - Validate all uploads
   - Sanitize user input

See `DEPLOYMENT.md` for advanced setup.

---

## 📁 Directory Structure

```
Video Frame Interpolation/
├── public/                 # Web UI files
│   ├── index.html         # Home page
│   ├── about.html         # Info page
│   ├── styles.css         # Styling
│   └── script.js          # Frontend logic
├── input/                 # Uploaded videos
├── output/                # Processed videos
├── server.js              # Web server
├── package.json           # Dependencies
├── start.bat              # Windows launcher
└── deployment.md          # Production guide
```

---

## 🐛 Troubleshooting

### Port 3000 Already in Use
```bash
netstat -ano | findstr :3000
taskkill /PID [process_id] /F
```

### Can't Access from Network
- Check Windows Firewall settings
- Allow Node.js in firewall
- Verify correct IP address using `ipconfig`
- Ping your computer to test connectivity

### Video Processing Fails
- Ensure C++ executable exists
- Check OpenCV DLLs are present
- Try a smaller test video
- Check browser console (F12) for errors

### Server Won't Start
- Close other apps using port 3000
- Reinstall dependencies: `npm install`
- Check Node.js version: `node --version`
- Try running as Administrator

### Slow Upload
- Use Ethernet instead of WiFi
- Try a smaller video file
- Close other network apps
- Check internet connection speed

---

## 📱 Mobile Access

Your web app is mobile-friendly! Access from:
- Smartphones
- Tablets
- Any device with a web browser

Just use your computer's IP address:
```
http://192.168.1.50:3000
```

---

## 🔄 Process Management Commands

Start in background:
```bash
npm run pm2-start
```

Stop the server:
```bash
npm run pm2-stop
```

Restart:
```bash
npm run pm2-restart
```

View logs:
```bash
npm run pm2-log
```

Monitor all processes:
```bash
pm2 monit
```

---

## 📊 Monitoring Performance

With PM2:
```bash
pm2 list                # Show all processes
pm2 status              # Process status
pm2 logs vfi-studio     # View logs
pm2 monit               # Real-time monitoring
pm2 web                 # Web dashboard (port 9615)
```

---

## 💾 Data Backup

Important directories to backup:
```
input/      - Original uploaded videos
output/     - Processed results
```

Automate backups:
```bash
# Example: Copy output to backup location
xcopy output\ "D:\Backups\VFI\output\" /Y /E
```

---

## ⚡ Advanced: Custom Port

Before starting server:
```bash
set PORT=8080
npm start
```

Server will run on: http://localhost:8080

---

## 🎯 Deployment Checklist

- [ ] Node.js installed
- [ ] Dependencies installed (`npm install`)
- [ ] C++ executable built
- [ ] OpenCV DLLs present
- [ ] Input/output directories created
- [ ] Windows Firewall configured
- [ ] Server starts without errors
- [ ] UI accessible from browser
- [ ] Can upload test video
- [ ] Test video processes successfully

---

**Your Video Frame Interpolation Studio is ready as a web application!** 🎬

For advanced production setup, see: `DEPLOYMENT.md`
