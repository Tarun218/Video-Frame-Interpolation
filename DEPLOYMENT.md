# Production Deployment Guide

## Quick Start (Development)

```bash
npm install
npm start
```

Then access:
- **Localhost**: http://localhost:3000
- **Network**: http://[YOUR_IP]:3000

## Production Setup (PM2)

### Step 1: Install PM2 Globally
```bash
npm install -g pm2
```

### Step 2: Start with PM2
```bash
npm run pm2-start
```

### Step 3: Make PM2 Auto-start on Reboot
```bash
pm2 startup
pm2 save
```

### Managing with PM2

**Start:**
```bash
npm run pm2-start
```

**Stop:**
```bash
npm run pm2-stop
```

**Restart:**
```bash
npm run pm2-restart
```

**View Logs:**
```bash
npm run pm2-log
```

**Monitor:**
```bash
pm2 monit
```

---

## Network Access

### From Your Computer:
- http://localhost:3000
- http://127.0.0.1:3000

### From Other Computers on Network:
1. Find your computer's IP address:
   ```bash
   ipconfig
   ```
   Look for "IPv4 Address" (usually starts with 192.168 or 10.x)

2. Share the URL:
   ```
   http://[YOUR_IP]:3000
   ```
   Example: http://192.168.1.100:3000

### Building Output Directory Structure

Ensure these directories exist:
```
Video Frame Interpolation/
├── input/          (Auto-created if missing)
├── output/         (Auto-created if missing)
└── public/         (Already created)
```

---

## System Requirements

- **CPU**: 4+ cores recommended
- **RAM**: 8GB+ recommended
- **Storage**: 50GB+ free space
- **Network**: Stable connection for uploads
- **OpenCV**: Already included via DLLs

---

## Performance Tuning

### Increase File Upload Limit
Edit `server.js` and modify:
```javascript
const upload = multer({
    storage: storage,
    limits: {
        fileSize: 5 * 1024 * 1024 * 1024  // 5GB
    }
});
```

### Custom Port
Set environment variable before starting:
```bash
set PORT=8000
npm start
```

Or edit `server.js` and change:
```javascript
const PORT = process.env.PORT || 3000;  // Change 3000 to desired port
```

---

## Environment Variables

Create a `.env` file (optional):
```
PORT=3000
NODE_ENV=production
MAX_UPLOAD_SIZE=2GB
```

---

## Troubleshooting

### Port Already in Use
```bash
netstat -ano | findstr :3000
taskkill /PID [PID_NUMBER] /F
```

### PM2 Won't Start
```bash
pm2 kill
pm2 start server.js
```

### Can't Access from Network
- Check Windows Firewall allows port 3000
- Verify you're using correct IP address
- Ping the computer to verify network connectivity

### OpenCV DLLs Not Found
- Ensure DLL files are in the project root:
  - opencv_world4120.dll
  - opencv_videoio_ffmpeg4120_64.dll

---

## Nginx Reverse Proxy (Advanced)

For better production setup with Nginx:

```nginx
server {
    listen 80;
    server_name your-domain.com;

    location / {
        proxy_pass http://localhost:3000;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection 'upgrade';
        proxy_set_header Host $host;
        proxy_cache_bypass $http_upgrade;
        proxy_buffering off;
    }
}
```

---

## Security Notes

For production internet deployment:
1. Use HTTPS with SSL certificate
2. Implement user authentication
3. Add rate limiting
4. Validate all file uploads
5. Use Nginx as reverse proxy
6. Implement CORS properly
7. Add input sanitization

---

## Monitoring

Check server health:
```bash
pm2 list
pm2 status
pm2 logs
```

---

## Backup Important Files

Regular backups of:
- `output/` directory (processed videos)
- `input/` directory (original uploads)
- `.env` file (if using)

---

## Auto-restart on Crash

PM2 automatically restarts crashed processes. To disable:
```bash
pm2 delete vfi-studio
pm2 start server.js --name vfi-studio --no-autorestart
```

---

## Scaling (Multiple Instances)

For multiple CPU cores:
```bash
pm2 start server.js -i max --name vfi-studio
```

This creates one process per CPU core with load balancing.

---

**Your Video Frame Interpolation Studio is production-ready!** 🚀
