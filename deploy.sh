#!/bin/bash

# Azure deployment script for Video Frame Interpolation
# This script prepares the environment for deployment

echo "=========================================="
echo "Video Frame Interpolation - Azure Deployment"
echo "=========================================="

# Set deployment variables
export DEPLOYMENT_SOURCE=$DEPLOYMENT_SOURCE
export DEPLOYMENT_TARGET=$DEPLOYMENT_TARGET

echo "Deployment source: $DEPLOYMENT_SOURCE"
echo "Deployment target: $DEPLOYMENT_TARGET"

# Navigate to the source directory
if [ -d "$DEPLOYMENT_SOURCE" ]; then
  cd "$DEPLOYMENT_SOURCE"
else
  echo "Error: Deployment source not found"
  exit 1
fi

echo "Current directory: $(pwd)"

# Install Node.js dependencies
echo "Installing Node.js dependencies..."
npm install --production

# Check if Python is available
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    PYTHON_CMD="python"
else
    echo "Warning: Python not found in PATH"
    PYTHON_CMD="python3"
fi

echo "Using Python: $PYTHON_CMD"

# Install Python dependencies (if requirements.txt exists)
if [ -f "requirements.txt" ]; then
  echo "Installing Python dependencies..."
  $PYTHON_CMD -m pip install --upgrade pip
  $PYTHON_CMD -m pip install -r requirements.txt
else
  echo "Note: No requirements.txt found. Python dependencies may need to be installed manually."
fi

# Create necessary directories
echo "Creating application directories..."
mkdir -p input
mkdir -p output
mkdir -p tmp

# Set proper permissions
echo "Setting permissions..."
chmod -R 755 input
chmod -R 755 output
chmod -R 755 tmp

echo "=========================================="
echo "Deployment preparation complete!"
echo "=========================================="

exit 0
