@echo off
REM Video Frame Interpolation Studio - Production Web App Launcher
REM Runs as a web application accessible from the network

title Video Frame Interpolation Studio - Web Server
color 0A

cls
echo.
echo ================================================
echo  VIDEO FRAME INTERPOLATION STUDIO
echo  Production Web Application
echo ================================================
echo.

REM Check if Node.js is installed
node --version > nul 2>&1
if errorlevel 1 (
    color 0C
    echo ERROR: Node.js is not installed!
    echo Please download and install from: https://nodejs.org/
    echo.
    pause
    exit /b 1
)

echo [OK] Node.js is installed
echo.

REM Check if node_modules exists
if not exist "node_modules" (
    echo [INFO] Installing dependencies... (this may take 1-2 minutes)
    echo.
    call npm install
    if errorlevel 1 (
        color 0C
        echo ERROR: Failed to install dependencies
        echo.
        pause
        exit /b 1
    )
    echo.
)

echo [OK] Dependencies installed
echo.

REM Check if C++ executable exists
if not exist "Video Frame Interpolation.exe" (
    color 0C
    echo ERROR: Video Frame Interpolation.exe not found!
    echo Please build the C++ program first.
    echo Use: npx tsc or build in Visual Studio
    echo.
    pause
    exit /b 1
)

echo [OK] Video processor executable found
echo.

color 0B
echo ================================================
echo  STARTING WEB SERVER...
echo ================================================
echo.
echo Wait for server startup message below...
echo.

REM Start the server
call npm start

REM If we get here, server was stopped
color 0A
echo.
echo ================================================
echo  Server has been stopped
echo ================================================
echo.
pause

