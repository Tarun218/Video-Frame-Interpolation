# Video Frame Interpolation Studio

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE) [![Node.js](https://img.shields.io/badge/node-%3E%3D14.0.0-brightgreen.svg)](https://nodejs.org/)

A professional video enhancement project showcasing full-stack integration between a Node.js/Express web UI and a native C++ OpenCV processor.

This repository includes:
- A modern, responsive web UI for local video upload and video processing.
- Slow motion interpolation from 2x to 8x.
- Super-resolution upscaling from 1.0x to 3.0x.
- Quality enhancement with sharpening, contrast, and noise reduction.
- Clear developer onboarding for recruiters and hiring managers.

---

## What This Project Solves

This project demonstrates full-stack integration between a JavaScript web interface and a native C++ media processor. It is a strong portfolio piece because it:

- shows desktop-level media processing in a browser-ready application,
- includes real-world file upload and processing flows,
- documents a complete developer setup,
- and is ready to present to recruiters with clear purpose.

---

## Features

- Responsive web dashboard for video selection and processing
- Drag-and-drop file upload plus upload feedback
- Slow motion interpolation adjustable from 2x to 8x
- Resolution scaling from 1.0x to 3.0x
- Quality boost options including sharpening, contrast, and noise reduction
- Downloadable output with direct file delivery
- API endpoints for automation or integration

---

## Tech Stack

- Node.js + Express
- Multer file upload middleware
- Native C++ video processor using OpenCV
- HTML, CSS, JavaScript front-end

---

## Quick Start

    cd  d:\Projects\Video Frame Interpolation\Video Frame Interpolation
    npm install

Build the C++ executable and place it in the project root as Video Frame Interpolation.exe.

Then run:

    npm start

Open:

    http://localhost:3000

---

## Build the C++ Executable

This repo excludes compiled binaries from source control. Build the executable locally and place it in the repository root.

Example Visual Studio build command:

    C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat
    C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe /EHsc /std:c++17 /ID:\opencv\build\include /ID:\opencv\build\include\opencv2 src\VideoFrameInterpolation.cpp /link /LIBPATH:D:\opencv\build\x64\vc16\lib opencv_world4120.lib /OUT:Video Frame Interpolation.exe

Note: Video Frame Interpolation.exe is intentionally excluded from Git via .gitignore.

---

## Folder Structure

    Video Frame Interpolation/
    +-- public/                 # Front-end UI assets
    �   +-- index.html
    �   +-- about.html
    �   +-- styles.css
    �   +-- script.js
    +-- src/                    # C++ source
    �   +-- VideoFrameInterpolation.cpp
    +-- input/                  # Uploaded input videos
    +-- output/                 # Processed output videos
    +-- server.js               # Express server
    +-- package.json            # Node metadata and scripts
    +-- package-lock.json       # Dependency lock file
    +-- LICENSE                # Open source license
    +-- README.md               # Project documentation

---

## How to Use

1. Open the project in your browser
2. Upload a supported video file
3. Set the slow motion factor and resolution scale
4. Start processing
5. Download the enhanced video when it is ready

---

## Supported Formats

- MP4
- AVI
- MOV
- MKV
- FLV
- WMV

---

## APIs

### POST /api/process-video

Upload a video and process it.

Parameters:

- video: multipart file upload
- slowFactor: integer, 2-8
- resolutionScale: float, 1.0-3.0
- enableSharpening: 0 or 1
- enableContrast: 0 or 1
- enableNoisReduction: 0 or 1
- enableEdgeUpscale: 0 or 1

Sample response:

    {
      success: true,
      message: Video processed successfully,
      outputFile: output_1234567890.mp4,
      outputPath: /download/output_1234567890.mp4
    }

### GET /download/:filename

Download a processed video file.

### GET /api/status/:filename

Check whether a processed file exists and view its metadata.

---

## System Requirements

### Recommended

- Node.js 14+
- Windows 10/11
- OpenCV development libraries installed
- 8GB+ RAM
- SSD storage

---

## License

This project is released under the MIT License. See LICENSE.

---

## Notes for Recruiters

This repo demonstrates:

- cross-language integration between Node.js and C++,
- user-facing UX design for technical applications,
- practical file upload and processing workflow,
- clear technical documentation and build instructions.

---

## Status

Prototype / portfolio-ready implementation with production-oriented documentation and developer onboarding.
