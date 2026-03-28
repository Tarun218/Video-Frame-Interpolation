// File Upload Handling
const videoInput = document.getElementById('videoInput');
const fileUploadArea = document.querySelector('.file-upload-area');
const fileInfo = document.getElementById('fileInfo');
let selectedFile = null;

// Drag and drop functionality
fileUploadArea.addEventListener('click', () => videoInput.click());

fileUploadArea.addEventListener('dragover', (e) => {
    e.preventDefault();
    fileUploadArea.classList.add('active');
});

fileUploadArea.addEventListener('dragleave', () => {
    fileUploadArea.classList.remove('active');
});

fileUploadArea.addEventListener('drop', (e) => {
    e.preventDefault();
    fileUploadArea.classList.remove('active');
    
    const files = e.dataTransfer.files;
    if (files.length > 0) {
        selectedFile = files[0];
        updateFileInfo();
        videoInput.files = files;
    }
});

videoInput.addEventListener('change', (e) => {
    if (e.target.files.length > 0) {
        selectedFile = e.target.files[0];
        updateFileInfo();
    }
});

function updateFileInfo() {
    if (selectedFile) {
        const fileName = selectedFile.name;
        const fileSize = (selectedFile.size / (1024 * 1024)).toFixed(2);
        fileInfo.textContent = `✓ ${fileName} (${fileSize} MB)`;
    }
}

// Slow Motion Factor Slider
const slowFactorSlider = document.getElementById('slowFactor');
const slowFactorValue = document.getElementById('slowFactorValue');

slowFactorSlider.addEventListener('input', (e) => {
    const value = e.target.value;
    slowFactorValue.textContent = `${value}x`;
});

// Resolution Scale Selection
const resolutionOptions = document.querySelectorAll('.res-btn');
const resolutionScale = document.getElementById('resolutionScale');
const resolutionInfo = document.getElementById('resolutionInfo');

const resolutionLabels = {
    '1.0': 'Original',
    '1.5': '1.5x (Medium)',
    '2.0': '2x (High)',
    '2.5': '2.5x (Very High)',
    '3.0': '3x (Ultra)'
};

resolutionOptions.forEach(btn => {
    btn.addEventListener('click', (e) => {
        // Remove active class from all buttons
        resolutionOptions.forEach(b => b.classList.remove('active'));
        
        // Add active class to clicked button
        e.currentTarget.classList.add('active');
        
        // Update hidden input
        const value = e.currentTarget.getAttribute('data-value');
        resolutionScale.value = value;
        
        // Update info text
        resolutionInfo.innerHTML = `Selected: <strong>${value}x</strong> (${resolutionLabels[value]})`;
    });
});

// Set initial active button
resolutionOptions[0].classList.add('active');

// Quality Enhancement Toggles
const enableSharpening = document.getElementById('enableSharpening');
const enableContrast = document.getElementById('enableContrast');
const enableNoisReduction = document.getElementById('enableNoisReduction');
const enableEdgeUpscale = document.getElementById('enableEdgeUpscale');

// Add visual feedback to toggles
[enableSharpening, enableContrast, enableNoisReduction, enableEdgeUpscale].forEach(toggle => {
    toggle.addEventListener('change', () => {
        // Visual feedback handled by CSS
    });
});

// Form Submission
const videoForm = document.getElementById('videoForm');
const submitBtn = document.getElementById('submitBtn');
const statusSection = document.getElementById('statusSection');
const progressText = document.getElementById('progressText');
const resultContent = document.getElementById('resultContent');
const errorContent = document.getElementById('errorContent');

videoForm.addEventListener('submit', async (e) => {
    e.preventDefault();
    
    if (!selectedFile) {
        alert('Please select a video file');
        return;
    }
    
    // Disable submit button and show processing status
    submitBtn.disabled = true;
    videoForm.style.display = 'none';
    statusSection.style.display = 'block';
    resultContent.style.display = 'none';
    errorContent.style.display = 'none';
    
    // Create FormData
    const formData = new FormData();
    formData.append('video', selectedFile);
    formData.append('slowFactor', slowFactorSlider.value);
    formData.append('resolutionScale', resolutionScale.value);
    formData.append('enableSharpening', enableSharpening.checked ? 1 : 0);
    formData.append('enableContrast', enableContrast.checked ? 1 : 0);
    formData.append('enableNoisReduction', enableNoisReduction.checked ? 1 : 0);
    formData.append('enableEdgeUpscale', enableEdgeUpscale.checked ? 1 : 0);
    
    try {
        // Send request
        progressText.textContent = 'Uploading video and starting processing...';
        
        const response = await fetch('/api/process-video', {
            method: 'POST',
            body: formData
        });
        
        const data = await response.json();
        
        if (!response.ok) {
            throw new Error(data.error || 'Processing failed');
        }
        
        // Success
        progressText.textContent = 'Processing complete! Your video is ready!';
        
        setTimeout(() => {
            resultContent.style.display = 'block';
            
            // Update download button
            const downloadBtn = document.getElementById('downloadBtn');
            downloadBtn.href = data.outputPath;
            downloadBtn.download = data.outputFile;
            
            // Update processing details
            const details = document.getElementById('processingDetails');
            const enhancements = [];
            if (enableSharpening.checked) enhancements.push('Sharpening');
            if (enableContrast.checked) enhancements.push('Contrast Boost');
            if (enableNoisReduction.checked) enhancements.push('Noise Reduction');
            if (enableEdgeUpscale.checked) enhancements.push('Edge Upscaling');
            
            const enhancementText = enhancements.length > 0 ? enhancements.join(', ') : 'None';
            
            details.innerHTML = `
                <strong>Processing Details:</strong>
                <ul style="margin-left: 20px; margin-top: 10px;">
                    <li>Slow Motion Factor: ${slowFactorSlider.value}x</li>
                    <li>Resolution Scale: ${resolutionScale.value}x</li>
                    <li>Quality Enhancements: ${enhancementText}</li>
                    <li>Output File: ${data.outputFile}</li>
                </ul>
            `;
            
            // Update progress bar
            const progressFill = document.getElementById('progressFill');
            progressFill.style.width = '100%';
        }, 500);
        
    } catch (error) {
        console.error('Error:', error);
        
        progressText.textContent = 'Processing failed';
        
        setTimeout(() => {
            errorContent.style.display = 'block';
            document.getElementById('errorMessage').textContent = error.message;
            
            const progressFill = document.getElementById('progressFill');
            progressFill.style.width = '0%';
        }, 500);
    }
});

// Reset Form
function resetForm() {
    // Reset file input
    videoInput.value = '';
    selectedFile = null;
    fileInfo.textContent = '';
    
    // Reset sliders to default
    slowFactorSlider.value = 2;
    slowFactorValue.textContent = '2x';
    
    resolutionScale.value = '1.0';
    resolutionOptions.forEach(b => b.classList.remove('active'));
    resolutionOptions[0].classList.add('active');
    resolutionInfo.innerHTML = `Selected: <strong>1.0x</strong> (Original)`;
    
    // Reset quality toggles
    enableSharpening.checked = true;
    enableContrast.checked = true;
    enableNoisReduction.checked = true;
    enableEdgeUpscale.checked = true;
    
    // Hide status section and show form
    statusSection.style.display = 'none';
    videoForm.style.display = 'flex';
    
    // Re-enable submit button
    submitBtn.disabled = false;
    
    // Reset progress bar
    document.getElementById('progressFill').style.width = '0%';
    progressText.textContent = 'Processing your video...';
}

// Initialize - set default resolution info text
document.addEventListener('DOMContentLoaded', () => {
    resolutionInfo.innerHTML = `Selected: <strong>1.0x</strong> (Original)`;
    
    // Add keyboard shortcut for file selection
    document.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' && e.ctrlKey) {
            if (!submitBtn.disabled && selectedFile) {
                videoForm.dispatchEvent(new Event('submit'));
            }
        }
    });
});

// Preview video metadata
videoInput.addEventListener('change', (e) => {
    if (e.target.files.length > 0) {
        const file = e.target.files[0];
        const video = document.createElement('video');
        video.onloadedmetadata = () => {
            console.log('Video loaded - Duration:', video.duration, 'seconds');
        };
        video.src = URL.createObjectURL(file);
    }
});
