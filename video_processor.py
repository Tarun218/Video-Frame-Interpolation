#!/usr/bin/env python3
"""
Video Frame Interpolation Processor
Uses OpenCV for optical flow-based frame interpolation
"""

import cv2
import sys
import os
import numpy as np
from pathlib import Path

class VideoInterpolationProcessor:
    def __init__(self, slow_factor=2, resolution_scale=1.0, quality_flags=15):
        self.slow_factor = max(2, min(8, slow_factor))  # Clamp 2-8
        self.resolution_scale = max(1.0, min(3.0, resolution_scale))  # Clamp 1-3
        self.quality_flags = quality_flags
        
        # Unpack quality flags
        self.enable_sharpening = bool(quality_flags & 1)
        self.enable_contrast = bool(quality_flags & 2)
        self.enable_noise_reduction = bool(quality_flags & 4)
        self.enable_edge_upscale = bool(quality_flags & 8)

    def apply_sharpening(self, frame, strength=1.5):
        """Apply sharpening to enhance details"""
        if not self.enable_sharpening:
            return frame
        
        blurred = cv2.GaussianBlur(frame, (0, 0), 1.0)
        sharpened = cv2.addWeighted(frame, 1.0 + strength, blurred, -strength, 0)
        sharpened = np.clip(sharpened, 0, 255).astype(np.uint8)
        return sharpened
    
    def apply_edge_upscale(self, frame):
        """Enhance edges during upscaling for better quality"""
        if not self.enable_edge_upscale:
            return frame
        
        # Detect edges using Canny
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        edges = cv2.Canny(gray, 50, 150)
        
        # Dilate edges slightly for better enhancement
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (2, 2))
        edges = cv2.dilate(edges, kernel, iterations=1)
        
        # Convert edge mask to 3 channels
        edges_3ch = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)
        
        # Blend with original to enhance edges
        enhanced = cv2.addWeighted(frame, 0.85, edges_3ch, 0.15, 0)
        return np.clip(enhanced, 0, 255).astype(np.uint8)

    def enhance_contrast(self, frame):
        """Enhance contrast and saturation"""
        if not self.enable_contrast:
            return frame
        
        lab = cv2.cvtColor(frame, cv2.COLOR_BGR2Lab)
        l, a, b = cv2.split(lab)
        
        clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(8, 8))
        l = clahe.apply(l)
        
        # Increase saturation by scaling a and b channels (preserve signed values)
        a = np.clip((a.astype(np.float32) - 128) * 1.2 + 128, 0, 255).astype(np.uint8)
        b = np.clip((b.astype(np.float32) - 128) * 1.2 + 128, 0, 255).astype(np.uint8)
        
        lab = cv2.merge([l, a, b])
        enhanced = cv2.cvtColor(lab, cv2.COLOR_Lab2BGR)
        return np.clip(enhanced, 0, 255).astype(np.uint8)
    
    def upscale_frame(self, frame, target_width, target_height):
        """High-quality upscaling with edge enhancement"""
        # Use INTER_CUBIC for better quality than LINEAR
        upscaled = cv2.resize(frame, (target_width, target_height), interpolation=cv2.INTER_CUBIC)
        
        # Apply edge upscaling if enabled
        if self.enable_edge_upscale:
            upscaled = self.apply_edge_upscale(upscaled)
        
        return upscaled
    
    def apply_quality_enhancements(self, frame):
        """Apply all quality enhancements to a frame"""
        if self.enable_sharpening:
            frame = self.apply_sharpening(frame, 1.0)
        
        if self.enable_contrast:
            frame = self.enhance_contrast(frame)
        
        if self.enable_noise_reduction:
            frame = self.reduce_noise(frame)
        
        return frame

    def reduce_noise(self, frame):
        """Reduce noise for cleaner output"""
        if not self.enable_noise_reduction:
            return frame
        
        # Use bilateral filter for noise reduction while preserving edges
        denoised = cv2.bilateralFilter(frame, 9, 75, 75)
        return denoised

    def interpolate_frames(self, frame1, frame2, num_frames):
        """Interpolate intermediate frames using optical flow"""
        gray1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY)
        gray2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2GRAY)
        
        # Calculate dense optical flow
        flow = cv2.calcOpticalFlowFarneback(gray1, gray2, None, 0.5, 3, 15, 3, 5, 1.2, 0)
        
        interpolated_frames = []
        
        for i in range(1, num_frames + 1):
            t = i / (num_frames + 1)
            
            # Create interpolated frame using backward warping
            h, w = frame1.shape[:2]
            x, y = np.meshgrid(np.arange(w), np.arange(h))
            
            # Calculate motion vectors
            fx = flow[:, :, 0]
            fy = flow[:, :, 1]
            
            # Warp coordinates
            x1 = np.clip(x + t * fx, 0, w - 1).astype(np.float32)
            y1 = np.clip(y + t * fy, 0, h - 1).astype(np.float32)
            
            x2 = np.clip(x - (1 - t) * fx, 0, w - 1).astype(np.float32)
            y2 = np.clip(y - (1 - t) * fy, 0, h - 1).astype(np.float32)
            
            # Remap frames
            warped1 = cv2.remap(frame1, x1, y1, cv2.INTER_LINEAR)
            warped2 = cv2.remap(frame2, x2, y2, cv2.INTER_LINEAR)
            
            # Blend frames
            interpolated = cv2.addWeighted(warped1, 1 - t, warped2, t, 0)
            interpolated_frames.append(interpolated)
        
        return interpolated_frames

    def process_video(self, input_path, output_path):
        """Process video with frame interpolation"""
        
        print(f"\nVIDEO FRAME INTERPOLATION PROCESSOR")
        print(f"=" * 50)
        print(f"Input:  {input_path}")
        print(f"Output: {output_path}")
        print(f"Slow Motion: {self.slow_factor}x")
        print(f"Resolution Scale: {self.resolution_scale}x")
        print(f"Quality Enhancements Enabled:")
        print(f"  Sharpening: {'Yes' if self.enable_sharpening else 'No'}")
        print(f"  Contrast: {'Yes' if self.enable_contrast else 'No'}")
        print(f"  Noise Reduction: {'Yes' if self.enable_noise_reduction else 'No'}")
        print(f"  Edge Upscale: {'Yes' if self.enable_edge_upscale else 'No'}")
        print()
        
        if not os.path.exists(input_path):
            print(f"ERROR: Input file not found: {input_path}")
            return False
        
        # Open input video
        cap = cv2.VideoCapture(input_path)
        if not cap.isOpened():
            print(f"ERROR: Failed to open input video")
            return False
        
        # Get video properties
        fps = cap.get(cv2.CAP_PROP_FPS)
        width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
        total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
        
        print(f"Input Video Info:")
        print(f"  Resolution: {width}x{height}")
        print(f"  Frame Rate: {fps:.2f} fps")
        print(f"  Total Frames: {total_frames}")
        print()
        
        # Calculate output properties
        new_fps = fps  # Keep original FPS for slow motion effect
        new_width = int(width * self.resolution_scale)
        new_height = int(height * self.resolution_scale)
        
        # Ensure even dimensions for codec compatibility
        new_width = (new_width // 2) * 2
        new_height = (new_height // 2) * 2
        
        print(f"Output Video Info:")
        print(f"  Resolution: {new_width}x{new_height}")
        print(f"  Frame Rate: {new_fps:.2f} fps (original)")
        print(f"  Effective Playback Speed: 1/{self.slow_factor}x (SLOW MOTION)")
        print()
        
        # Create output directory
        os.makedirs(os.path.dirname(output_path) or '.', exist_ok=True)
        
        # Initialize video writer
        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
        out = cv2.VideoWriter(output_path, fourcc, new_fps, (new_width, new_height))
        
        if not out.isOpened():
            print(f"ERROR: Failed to create output video writer")
            cap.release()
            return False
        
        # Process frames
        ret, prev_frame = cap.read()
        if not ret:
            print(f"ERROR: Failed to read first frame")
            cap.release()
            out.release()
            return False
        
        # Resize and enhance if needed
        if self.resolution_scale != 1.0:
            prev_frame = self.upscale_frame(prev_frame, new_width, new_height)
        
        # Apply quality enhancements to original frame
        prev_frame = self.apply_quality_enhancements(prev_frame)
        
        frame_count = 1
        
        try:
            while True:
                ret, curr_frame = cap.read()
                if not ret:
                    break
                
                # Resize and enhance if needed
                if self.resolution_scale != 1.0:
                    curr_frame = self.upscale_frame(curr_frame, new_width, new_height)
                
                # Resize for optical flow if resolution is different
                if prev_frame.shape != curr_frame.shape:
                    curr_frame = cv2.resize(curr_frame, (prev_frame.shape[1], prev_frame.shape[0]))
                
                # Write original frame (already enhanced)
                out.write(prev_frame)
                
                # Generate and write interpolated frames
                print(f"Processing frames {frame_count}-{frame_count + self.slow_factor - 1}...", end='\r')
                
                interpolated = self.interpolate_frames(prev_frame, curr_frame, self.slow_factor - 1)
                for interp_frame in interpolated:
                    # Apply quality enhancements to interpolated frame
                    interp_frame = self.apply_quality_enhancements(interp_frame)
                    out.write(interp_frame)
                
                # Apply quality enhancements to current frame before it becomes prev_frame
                curr_frame = self.apply_quality_enhancements(curr_frame)
                prev_frame = curr_frame
                frame_count += 1
        
        finally:
            cap.release()
            out.release()
        
        print(f"\nProcessing complete! Generated {frame_count * self.slow_factor} frames")
        print(f"Output saved to: {output_path}")
        return True

def main():
    if len(sys.argv) < 3:
        print("Usage: python video_processor.py <input> <output> [slowFactor] [resolutionScale] [qualityFlags]")
        sys.exit(1)
    
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    
    slow_factor = int(sys.argv[3]) if len(sys.argv) > 3 else 2
    resolution_scale = float(sys.argv[4]) if len(sys.argv) > 4 else 1.0
    quality_flags = int(sys.argv[5]) if len(sys.argv) > 5 else 15
    
    processor = VideoInterpolationProcessor(slow_factor, resolution_scale, quality_flags)
    success = processor.process_video(input_path, output_path)
    
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
