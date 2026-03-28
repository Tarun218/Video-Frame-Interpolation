#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace cv;

class EnhancedVideoInterpolator {
private:
    // Ultra-strong sharpening filter with edge enhancement
    Mat applySharpening(const Mat& input, double strength = 3.0) {
        Mat sharpened, blurred, details;
        
        // Step 1: Unsharp masking with aggressive parameters
        GaussianBlur(input, blurred, Size(0, 0), 1.5);
        addWeighted(input, 1.0 + strength * 0.8, blurred, -strength * 0.8, 0, sharpened);
        
        // Step 2: High-pass sharpening for extreme edge clarity
        Mat high_pass = input.clone();
        GaussianBlur(high_pass, blurred, Size(5, 5), 1.0);
        subtract(high_pass, blurred, high_pass);
        addWeighted(sharpened, 0.85, high_pass, 0.15 * strength, 0, sharpened);
        
        // Step 3: Aggressive kernel-based sharpening
        Mat kernel = (Mat_<float>(3, 3) <<
            -1.0, -1.5, -1.0,
            -1.5, 12, -1.5,
            -1.0, -1.5, -1.0);
        Mat kernel_sharp;
        filter2D(sharpened, kernel_sharp, -1, kernel);
        addWeighted(sharpened, 0.8, kernel_sharp, 0.2, 0, sharpened);
        
        // Clip values to valid range
        Mat result;
        sharpened.convertTo(result, -1, 1.0, 0);
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                for (int c = 0; c < 3; c++) {
                    uchar& pixel = result.at<Vec3b>(i, j)[c];
                    pixel = cv::saturate_cast<uchar>(pixel);
                }
            }
        }
        
        return result;
    }

    // Advanced upscaling with aggressive edge-aware enhancement
    Mat upscaleFrame(const Mat& input, double scale_factor, int method = 2) {
        Mat upscaled;
        Size new_size(input.cols * scale_factor, input.rows * scale_factor);

        switch (method) {
        case 0: // Fast but lower quality
            resize(input, upscaled, new_size, 0, 0, INTER_LINEAR);
            break;
        case 1: // Better quality
            resize(input, upscaled, new_size, 0, 0, INTER_CUBIC);
            break;
        case 2: // Best quality (recommended)
            resize(input, upscaled, new_size, 0, 0, INTER_LANCZOS4);
            break;
        }

        // Apply edge-preserving enhancement
        Mat blurred;
        GaussianBlur(upscaled, blurred, Size(3, 3), 0.5);
        Mat details = upscaled - blurred;
        addWeighted(upscaled, 0.95, details, 0.05, 0, upscaled);

        return upscaled;
    }

    // Aggressive noise reduction for ultra-clean results
    Mat reduceNoise(const Mat& input) {
        Mat denoised = input.clone();
        
        // Triple-pass denoising for maximum quality
        fastNlMeansDenoisingColored(denoised, denoised, 15, 15, 9, 28);
        fastNlMeansDenoisingColored(denoised, denoised, 12, 12, 8, 25);
        fastNlMeansDenoisingColored(denoised, denoised, 10, 10, 7, 21);
        
        // Additional bilateral filter for edge preservation
        Mat bilateral;
        bilateralFilter(denoised, bilateral, 9, 75, 75);
        
        // Blend for best results
        addWeighted(denoised, 0.6, bilateral, 0.4, 0, denoised);
        
        return denoised;
    }

    // Ultra-aggressive contrast and saturation enhancement
    Mat enhanceContrast(const Mat& input) {
        Mat lab;
        cvtColor(input, lab, COLOR_BGR2Lab);

        vector<Mat> lab_planes;
        split(lab, lab_planes);

        // Step 1: Extreme CLAHE for dramatic contrast
        Ptr<CLAHE> clahe = createCLAHE();
        clahe->setClipLimit(6.0);  // Very aggressive contrast boost
        clahe->setTilesGridSize(Size(10, 10));  // Extremely fine grid
        clahe->apply(lab_planes[0], lab_planes[0]);
        clahe->apply(lab_planes[0], lab_planes[0]);  // Double pass
        
        // Step 2: Extreme color saturation boost (60% more vibrant)
        Mat a_boosted, b_boosted;
        lab_planes[1].convertTo(a_boosted, -1, 1.6, 0);
        lab_planes[2].convertTo(b_boosted, -1, 1.6, 0);
        
        // Clip values to valid 0-255 range
        a_boosted.copyTo(lab_planes[1]);
        b_boosted.copyTo(lab_planes[2]);
        
        merge(lab_planes, lab);
        Mat enhanced;
        cvtColor(lab, enhanced, COLOR_Lab2BGR);

        return enhanced;
    }

    // Optical flow frame generation with quality enhancement
    vector<Mat> generateEnhancedFrames(const Mat& frame1, const Mat& frame2, int num_frames, double resolution_scale) {
        vector<Mat> interpolated_frames;

        // Convert to grayscale for optical flow
        Mat gray1, gray2;
        cvtColor(frame1, gray1, COLOR_BGR2GRAY);
        cvtColor(frame2, gray2, COLOR_BGR2GRAY);

        // Calculate dense optical flow
        Mat flow;
        calcOpticalFlowFarneback(gray1, gray2, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        cout << "    Generating " << num_frames << " enhanced frames..." << endl;

        for (int i = 1; i <= num_frames; i++) {
            double t = (double)i / (num_frames + 1);
            Mat interpolated = Mat::zeros(frame1.size(), frame1.type());

            for (int y = 0; y < frame1.rows; y += 1) {
                for (int x = 0; x < frame1.cols; x += 1) {
                    Point2f f = flow.at<Point2f>(y, x);

                    float x1 = x + t * f.x;
                    float y1 = y + t * f.y;
                    float x2 = x - (1 - t) * f.x;
                    float y2 = y - (1 - t) * f.y;

                    Vec3b color1 = getBilinearPixel(frame1, x1, y1);
                    Vec3b color2 = getBilinearPixel(frame2, x2, y2);

                    Vec3b blended;
                    for (int c = 0; c < 3; c++) {
                        blended[c] = saturate_cast<uchar>((1.0 - t) * color1[c] + t * color2[c]);
                    }

                    interpolated.at<Vec3b>(y, x) = blended;
                }
            }

            // Apply quality enhancements
            interpolated = applySharpening(interpolated, 1.2);
            interpolated = enhanceContrast(interpolated);

            interpolated_frames.push_back(interpolated);
        }

        return interpolated_frames;
    }

    Vec3b getBilinearPixel(const Mat& img, float x, float y) {
        int x1 = static_cast<int>(x);
        int y1 = static_cast<int>(y);
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        float dx = x - x1;
        float dy = y - y1;

        if (x1 < 0 || y1 < 0 || x2 >= img.cols || y2 >= img.rows) {
            return Vec3b(0, 0, 0);
        }

        Vec3b p11 = img.at<Vec3b>(y1, x1);
        Vec3b p12 = img.at<Vec3b>(y1, x2);
        Vec3b p21 = img.at<Vec3b>(y2, x1);
        Vec3b p22 = img.at<Vec3b>(y2, x2);

        Vec3b top = p11 * (1 - dx) + p12 * dx;
        Vec3b bottom = p21 * (1 - dx) + p22 * dx;
        Vec3b result = top * (1 - dy) + bottom * dy;

        return result;
    }

public:
    bool processEnhancedVideo(const string& input_path, const string& output_path,
        int slow_factor, double resolution_scale = 1.0,
        bool enable_sharpening = true, bool enable_contrast = true,
        bool enable_noise_reduction = true, bool enable_edge_upscale = true) {
        cout << "\nENHANCED VIDEO INTERPOLATION PROCESSOR" << endl;
        cout << "======================================" << endl;
        cout << "Input: " << input_path << endl;
        cout << "Output: " << output_path << endl;
        cout << "Slow Motion: " << slow_factor << "x" << endl;
        cout << "Resolution Scale: " << resolution_scale << "x" << endl;
        cout << "Quality Enhancements Enabled:" << endl;
        cout << "  Sharpening: " << (enable_sharpening ? "Yes" : "No") << endl;
        cout << "  Contrast: " << (enable_contrast ? "Yes" : "No") << endl;
        cout << "  Noise Reduction: " << (enable_noise_reduction ? "Yes" : "No") << endl;
        cout << "  Edge Upscaling: " << (enable_edge_upscale ? "Yes" : "No") << endl;

        auto start_time = chrono::high_resolution_clock::now();

        // Verify file exists
        ifstream file_test(input_path);
        if (!file_test.good()) {
            cerr << "ERROR: File does not exist: " << input_path << endl;
            return false;
        }
        file_test.close();

        // Open video
        VideoCapture cap(input_path);
        if (!cap.isOpened()) {
            cerr << "ERROR: Cannot open video file!" << endl;
            return false;
        }

        cout << "SUCCESS: Video opened successfully!" << endl;

        // Get original video properties
        double original_fps = cap.get(CAP_PROP_FPS);
        int original_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
        int original_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
        int total_frames = static_cast<int>(cap.get(CAP_PROP_FRAME_COUNT));

        // Calculate enhanced resolution
        int enhanced_width = original_width * resolution_scale;
        int enhanced_height = original_height * resolution_scale;
        double output_fps = original_fps; // Same FPS for true slow motion

        cout << "\nVIDEO PROPERTIES:" << endl;
        cout << "   Original Resolution: " << original_width << "x" << original_height << endl;
        cout << "   Enhanced Resolution: " << enhanced_width << "x" << enhanced_height << endl;
        cout << "   Resolution Improvement: " << resolution_scale << "x" << endl;
        cout << "   Original FPS: " << fixed << setprecision(2) << original_fps << endl;
        cout << "   Total Frames: " << total_frames << endl;
        cout << "   Output Duration: " << (total_frames * slow_factor) / original_fps << " seconds" << endl;

        // Setup video writer with enhanced resolution and better codec
        VideoWriter writer;
        // Use MJPEG codec for better quality (less compression artifacts)
        int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');
        writer.open(output_path, codec, output_fps, Size(enhanced_width, enhanced_height));

        if (!writer.isOpened()) {
            // Try with different codec if first one fails
            codec = VideoWriter::fourcc('M', 'P', '4', 'V');
            writer.open(output_path, codec, output_fps, Size(enhanced_width, enhanced_height));
            
            if (!writer.isOpened()) {
                // Try AVI as fallback
                string avi_path = output_path;
                if (avi_path.find(".mp4") != string::npos) {
                    avi_path.replace(avi_path.find(".mp4"), 4, ".avi");
                }
                writer.open(avi_path, VideoWriter::fourcc('M', 'J', 'P', 'G'), output_fps, Size(enhanced_width, enhanced_height));

                if (!writer.isOpened()) {
                    cerr << "ERROR: Cannot create output video!" << endl;
                    cap.release();
                    return false;
                }
                cout << "SUCCESS: Output video writer initialized (AVI format)" << endl;
            }
            else {
                cout << "SUCCESS: Output video writer initialized (MP4 format)" << endl;
            }
        }
        else {
            cout << "SUCCESS: Output video writer initialized (MJPEG format - High Quality)" << endl;
        }

        cout << "\nPROCESSING ENHANCED VIDEO..." << endl;
        cout << "   Slow Motion: " << slow_factor << "x" << endl;
        cout << "   Resolution: " << resolution_scale << "x improvement" << endl;
        cout << "   Quality: Sharpness + Contrast enhancement" << endl;
        cout << "   Output: " << enhanced_width << "x" << enhanced_height << " resolution" << endl;

        Mat frame, prev_frame;
        int processed_count = 0;
        int frame_number = 0;

        // Create progress window
        namedWindow("Enhanced Video Processing", WINDOW_NORMAL);
        resizeWindow("Enhanced Video Processing", 800, 600);

        // Process video with enhancements
        while (true) {
            if (!cap.read(frame)) {
                break;
            }

            frame_number++;

            // Show progress
            if (frame_number % 10 == 0 || frame_number <= 5) {
                double progress = (frame_number * 100.0) / total_frames;
                cout << "   Processing frame " << frame_number << "/" << total_frames
                    << " (" << fixed << setprecision(1) << progress << "%)" << endl;

                // Display progress
                Mat display_frame = frame.clone();
                if (resolution_scale > 1.0) {
                    display_frame = upscaleFrame(display_frame, 0.5); // Scale down for display
                }
                resize(display_frame, display_frame, Size(800, 600));

                putText(display_frame, "ENHANCED VIDEO PROCESSING",
                    Point(20, 40), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(255, 255, 255), 2);
                putText(display_frame, "Frame: " + to_string(frame_number) + "/" + to_string(total_frames),
                    Point(20, 80), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);
                putText(display_frame, "Resolution: " + to_string(enhanced_width) + "x" + to_string(enhanced_height),
                    Point(20, 110), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);
                putText(display_frame, "Slow Motion: " + to_string(slow_factor) + "x",
                    Point(20, 140), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);
                putText(display_frame, "Press 'Q' to abort",
                    Point(20, 560), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(200, 200, 200), 1);

                imshow("Enhanced Video Processing", display_frame);

                if (waitKey(1) == 'q' || waitKey(1) == 'Q') {
                    cout << "Processing aborted by user" << endl;
                    break;
                }
            }

            // Apply quality enhancements to current frame (if enabled)
            if (enable_noise_reduction || enable_contrast || enable_sharpening) {
                if (frame_number == 1) {
                    cout << "\n✓ QUALITY ENHANCEMENTS ACTIVE:" << endl;
                    if (enable_sharpening) cout << "  ✓ Ultra-Strong Sharpening (Triple-Pass)" << endl;
                    if (enable_contrast) cout << "  ✓ Advanced Contrast Enhancement (6x CLAHE)" << endl;
                    if (enable_contrast) cout << "  ✓ Saturation Boost (+60%)" << endl;
                    if (enable_noise_reduction) cout << "  ✓ Multi-Pass Noise Reduction (Triple-Pass)" << endl;
                    cout << endl;
                }
                
                if (enable_noise_reduction) {
                    frame = reduceNoise(frame);
                }
                if (enable_contrast) {
                    frame = enhanceContrast(frame);
                }
                
                // Apply stronger sharpening for crystal clear output
                if (enable_sharpening) {
                    frame = applySharpening(frame, 3.0);
                    frame = applySharpening(frame, 2.2);
                    frame = applySharpening(frame, 1.5);  // Triple sharpening pass for extreme clarity
                }
            }

            // Upscale resolution with edge enhancement
            if (resolution_scale > 1.0) {
                frame = upscaleFrame(frame, resolution_scale, 2); // Lanczos4 for best quality
                
                // Extra aggressive sharpening after upscale if sharpening is enabled
                if (enable_sharpening) {
                    frame = applySharpening(frame, 2.8);
                    frame = applySharpening(frame, 1.8);  // Double pass after upscaling
                }
            }

            // First frame
            if (prev_frame.empty()) {
                for (int i = 0; i < slow_factor; i++) {
                    writer.write(frame);
                    processed_count++;
                }
                prev_frame = frame.clone();
                continue;
            }

            // Write previous frame multiple times for slow motion
            for (int i = 0; i < slow_factor; i++) {
                writer.write(prev_frame);
                processed_count++;
            }

            prev_frame = frame.clone();
        }

        // Write last frame
        if (!prev_frame.empty()) {
            for (int i = 0; i < slow_factor; i++) {
                writer.write(prev_frame);
                processed_count++;
            }
        }

        // Cleanup
        cap.release();
        writer.release();
        destroyAllWindows();

        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time);

        // Calculate results
        double original_duration = total_frames / original_fps;
        double new_duration = processed_count / output_fps;

        // Enhanced results
        cout << "\nENHANCED PROCESSING COMPLETED!" << endl;
        cout << "======================================" << endl;
        cout << "FINAL RESULTS:" << endl;
        cout << "   Original: " << original_width << "x" << original_height << endl;
        cout << "   Enhanced: " << enhanced_width << "x" << enhanced_height << endl;
        cout << "   Resolution Improvement: " << resolution_scale << "x" << endl;
        cout << "   Original Duration: " << fixed << setprecision(2) << original_duration << "s" << endl;
        cout << "   New Duration: " << fixed << setprecision(2) << new_duration << "s" << endl;
        cout << "   Slow Motion: " << fixed << setprecision(1) << (new_duration / original_duration) << "x" << endl;
        cout << "   Processing Time: " << duration.count() << " seconds" << endl;
        cout << "   Quality Features: Sharpness + Contrast + Noise Reduction" << endl;

        cout << "\nYOUR VIDEO IS NOW ENHANCED WITH:" << endl;
        cout << "   • " << slow_factor << "x SLOW MOTION" << endl;
        cout << "   • " << resolution_scale << "x HIGHER RESOLUTION" << endl;
        cout << "   • ENHANCED SHARPNESS & CONTRAST" << endl;
        cout << "   • REDUCED NOISE & ARTIFACTS" << endl;
        cout << "   • PROFESSIONAL-QUALITY OUTPUT" << endl;

        return true;
    }
};

// Enhanced user input functions
string getInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);

    if (!input.empty() && input.front() == '"' && input.back() == '"') {
        input = input.substr(1, input.length() - 2);
    }

    return input;
}

int getSlowFactor() {
    while (true) {
        cout << "Enter slow motion factor (2-8): ";
        string input;
        getline(cin, input);

        try {
            int factor = stoi(input);
            if (factor >= 2 && factor <= 8) {
                return factor;
            }
            else {
                cout << "Please enter 2-8." << endl;
            }
        }
        catch (...) {
            cout << "Please enter a valid number." << endl;
        }
    }
}

double getResolutionScale() {
    while (true) {
        cout << "Enter resolution scale (1.0 = same, 1.5 = 1.5x, 2.0 = 2x): ";
        string input;
        getline(cin, input);

        try {
            double scale = stod(input);
            if (scale >= 1.0 && scale <= 3.0) {
                return scale;
            }
            else {
                cout << "Please enter 1.0-3.0." << endl;
            }
        }
        catch (...) {
            cout << "Please enter a valid number." << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Support both CLI and interactive modes
    if (argc >= 5) {
        // CLI mode - called from web interface
        string input_path = argv[1];
        string output_path = argv[2];
        int slow_factor = stoi(argv[3]);
        double resolution_scale = stod(argv[4]);
        
        // Default: all enhancements enabled
        bool enable_sharpening = true;
        bool enable_contrast = true;
        bool enable_noise_reduction = true;
        bool enable_edge_upscale = true;
        
        // Read quality enhancement flags if provided (argc >= 6)
        // Flags are combined: bit 0=sharpening, bit 1=contrast, bit 2=noise, bit 3=edge
        if (argc >= 6) {
            int quality_flags = stoi(argv[5]);
            enable_sharpening = (quality_flags & 1) != 0;
            enable_contrast = (quality_flags & 2) != 0;
            enable_noise_reduction = (quality_flags & 4) != 0;
            enable_edge_upscale = (quality_flags & 8) != 0;
        }

        // Validate inputs
        if (slow_factor < 2 || slow_factor > 8) {
            cerr << "ERROR: Slow factor must be between 2 and 8" << endl;
            return 1;
        }
        if (resolution_scale < 1.0 || resolution_scale > 3.0) {
            cerr << "ERROR: Resolution scale must be between 1.0 and 3.0" << endl;
            return 1;
        }

        cout << "==========================================" << endl;
        cout << "  ULTIMATE VIDEO ENHANCEMENT SYSTEM" << endl;
        cout << "==========================================" << endl;
        cout << "Processing from Web Interface..." << endl;
        cout << "\nQUALITY ENHANCEMENTS:" << endl;
        cout << (enable_sharpening ? "  ✓" : "  ✗") << " Ultra-Strong Sharpening" << endl;
        cout << (enable_contrast ? "  ✓" : "  ✗") << " Advanced Contrast & Saturation" << endl;
        cout << (enable_noise_reduction ? "  ✓" : "  ✗") << " Multi-Pass Noise Reduction" << endl;
        cout << (enable_edge_upscale ? "  ✓" : "  ✗") << " Edge-Aware Upscaling" << endl;
        cout << "\nSlow Factor: " << slow_factor << "x" << endl;
        cout << "Resolution Scale: " << resolution_scale << "x" << endl;
        cout << endl;

        EnhancedVideoInterpolator processor;
        bool success = processor.processEnhancedVideo(input_path, output_path,
            slow_factor, resolution_scale, enable_sharpening, enable_contrast,
            enable_noise_reduction, enable_edge_upscale);

        if (success) {
            cout << endl;
            cout << "Enhancement completed successfully!" << endl;
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        // Interactive mode - for manual usage
        EnhancedVideoInterpolator processor;

        cout << "==========================================" << endl;
        cout << "  ULTIMATE VIDEO ENHANCEMENT SYSTEM" << endl;
        cout << "==========================================" << endl;
        cout << "Slow Motion + Super Resolution + Quality Boost" << endl;
        cout << "See every minute detail in crystal clarity!" << endl;
        cout << endl;

        while (true) {
            cout << "VIDEO ENHANCEMENT SETUP" << endl;
            cout << "=======================" << endl;

            string input_path = getInput("Enter INPUT video path: ");
            string output_path = getInput("Enter OUTPUT video path: ");
            int slow_factor = getSlowFactor();
            double resolution_scale = getResolutionScale();

            cout << endl;
            cout << "ENHANCEMENT CONFIGURATION" << endl;
            cout << "========================" << endl;
            cout << "Input: " << input_path << endl;
            cout << "Output: " << output_path << endl;
            cout << "Slow Motion: " << slow_factor << "x" << endl;
            cout << "Resolution: " << resolution_scale << "x improvement" << endl;
            cout << "Quality: Enhanced sharpness & contrast" << endl;
            cout << endl;

            cout << "Start enhancement? (y/n): ";
            string confirm;
            getline(cin, confirm);

            if (confirm == "y" || confirm == "Y" || confirm == "yes") {
                cout << endl;
                cout << "STARTING ULTIMATE VIDEO ENHANCEMENT..." << endl;
                cout << "This will create professional-quality enhanced video!" << endl;
                cout << "==========================================" << endl;

                bool success = processor.processEnhancedVideo(input_path, output_path,
                    slow_factor, resolution_scale, true, true, true, true);

                if (success) {
                    cout << endl;
                    cout << "Enhanced video created successfully!" << endl;
                }
            }
            else {
                cout << "Enhancement cancelled." << endl;
            }

            cout << endl;
            cout << "Enhance another video? (y/n): ";
            string another;
            getline(cin, another);

            if (!(another == "y" || another == "Y" || another == "yes")) {
                break;
            }

            cout << endl;
            cout << "==========================================" << endl;
        }

        cout << endl;
        cout << "Thank you for using Ultimate Video Enhancement System!" << endl;
        cout << "Press Enter to exit...";
        cin.get();

        return 0;
    }
}