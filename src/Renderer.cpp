#include "Renderer.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <execution>
#include <cmath>
#include <chrono>

Renderer::Renderer(int nx_, int ny_, double color_range_)
: nx(nx_), ny(ny_), color_range(color_range_), texture_needs_update(true) {
    std::cout << "Creating ultra-high resolution Raylib renderer for " << nx << "x" << ny 
              << " grid (" << (nx*ny) << " pixels) with color range " << color_range << std::endl;
    
    // Create an image - optimized for large resolutions
    image = GenImageColor(nx, ny, BLACK);
    
    // Create texture from image
    texture = LoadTextureFromImage(image);
    
    // Enable high-quality texture filtering for ultra-smooth antialiasing
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    
    // Additional quality settings for high resolution
    SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
    
    std::cout << "? Ultra-high resolution renderer initialized with bilinear antialiasing" << std::endl;
    
    // Memory usage estimation
    size_t image_memory = nx * ny * 4; // 4 bytes per RGBA pixel
    std::cout << "?? Image buffer memory: " << (image_memory / 1024 / 1024) << " MB" << std::endl;
}

Renderer::~Renderer() {
    // Clean up Raylib resources
    UnloadTexture(texture);
    UnloadImage(image);
    std::cout << "?? Raylib resources cleaned up" << std::endl;
}

void Renderer::setColorRange(double new_range) {
    color_range = new_range;
    // Reduced logging for better performance in interactive mode
    static int log_counter = 0;
    if (++log_counter % 10 == 0) {  // Log every 10th change
        std::cout << "?? Color range: " << std::fixed << std::setprecision(2) << color_range << std::endl;
    }
}

Color Renderer::mapValue(float v) {
    // Normalize to color range with optimized clamping
    float normalized = v / static_cast<float>(color_range);
    normalized = std::clamp(normalized, -1.0f, 1.0f);
    
    // Enhanced ultra-high resolution color mapping with smoother gradients
    const float abs_norm = std::abs(normalized);
    
    // Ultra-fine zero field detection for high resolution detail
    if (abs_norm < 0.005f) {
        // Very near zero: sophisticated dark blue-green gradient
        unsigned char intensity = static_cast<unsigned char>(32 + 32 * abs_norm / 0.005f);
        return Color{0, intensity, static_cast<unsigned char>(intensity + 16), 255};
    }
    
    if (normalized < -0.85f) {
        // Ultra-strong negative field: Deep blue to violet
        float t = (abs_norm - 0.85f) / 0.15f;
        unsigned char red = static_cast<unsigned char>(32 + 96 * t);
        unsigned char green = static_cast<unsigned char>(16 * t);
        unsigned char blue = 255;
        return Color{red, green, blue, 255};
    } else if (normalized < -0.6f) {
        // Very strong negative field: Blue to deep blue
        float t = (abs_norm - 0.6f) / 0.25f;
        unsigned char red = static_cast<unsigned char>(8 * t);
        unsigned char green = static_cast<unsigned char>(8 * t);
        unsigned char blue = static_cast<unsigned char>(180 + 75 * t);
        return Color{red, green, blue, 255};
    } else if (normalized < -0.3f) {
        // Strong negative field: Cyan to blue transition
        float t = (abs_norm - 0.3f) / 0.3f;
        unsigned char red = 0;
        unsigned char green = static_cast<unsigned char>(128 * (1.0f - t));
        unsigned char blue = static_cast<unsigned char>(128 + 127 * t);
        return Color{red, green, blue, 255};
    } else if (normalized < -0.1f) {
        // Medium negative field: Green-cyan to cyan transition
        float t = (abs_norm - 0.1f) / 0.2f;
        unsigned char red = 0;
        unsigned char green = static_cast<unsigned char>(64 + 64 * t);
        unsigned char blue = static_cast<unsigned char>(96 + 32 * t);
        return Color{red, green, blue, 255};
    } else if (normalized < 0.1f) {
        // Near zero field: Enhanced neutral field visualization
        float t = abs_norm / 0.1f;
        unsigned char base_intensity = static_cast<unsigned char>(48 + 48 * t);
        return Color{base_intensity, static_cast<unsigned char>(base_intensity + 16), base_intensity, 255};
    } else if (normalized < 0.3f) {
        // Medium positive field: Green to yellow transition
        float t = (abs_norm - 0.1f) / 0.2f;
        unsigned char red = static_cast<unsigned char>(64 + 96 * t);
        unsigned char green = static_cast<unsigned char>(128 + 64 * t);
        unsigned char blue = static_cast<unsigned char>(32 * (1.0f - t));
        return Color{red, green, blue, 255};
    } else if (normalized < 0.6f) {
        // Strong positive field: Yellow to orange transition
        float t = (abs_norm - 0.3f) / 0.3f;
        unsigned char red = static_cast<unsigned char>(160 + 95 * t);
        unsigned char green = static_cast<unsigned char>(192 * (1.0f - 0.4f * t));
        unsigned char blue = 0;
        return Color{red, green, blue, 255};
    } else if (normalized < 0.85f) {
        // Very strong positive field: Orange to red
        float t = (abs_norm - 0.6f) / 0.25f;
        unsigned char red = 255;
        unsigned char green = static_cast<unsigned char>(128 * (1.0f - t));
        unsigned char blue = static_cast<unsigned char>(32 * t);
        return Color{red, green, blue, 255};
    } else {
        // Ultra-strong positive field: Red to bright red-white
        float t = (abs_norm - 0.85f) / 0.15f;
        unsigned char red = 255;
        unsigned char green = static_cast<unsigned char>(64 * t);
        unsigned char blue = static_cast<unsigned char>(64 * t);
        return Color{red, green, blue, 255};
    }
}

void Renderer::render(const std::vector<float> &Ez) {
    static int frame_count = 0;
    static int performance_samples = 0;
    static double total_render_time = 0.0;
    frame_count++;
    
    auto render_start = std::chrono::high_resolution_clock::now();
    
    // Reduced debug output for better performance
    if (frame_count <= 3 || frame_count % 300 == 0) {
        auto minmax = std::minmax_element(Ez.begin(), Ez.end());
        float min_val = *minmax.first;
        float max_val = *minmax.second;
        std::cout << "???  Frame " << frame_count << ": Field range [" << std::fixed 
                  << std::setprecision(3) << min_val << ", " << max_val << "]" << std::endl;
    }
    
    // Ultra-high resolution pixel update with optimization
    const int total_pixels = nx * ny;
    int significant_pixels = 0;
    
    // Optimized pixel processing
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            const float v = Ez[j*nx + i];
            const Color color = mapValue(v);
            ImageDrawPixel(&image, i, j, color);
            
            // Count significant field pixels for statistics
            if (std::abs(v) > 0.01f) {
                significant_pixels++;
            }
        }
    }
    
    // Performance monitoring
    auto pixel_update_end = std::chrono::high_resolution_clock::now();
    
    // Update texture with new image data
    UpdateTexture(texture, image.data);
    
    // Enhanced ultra-high resolution rendering
    BeginDrawing();
    
    // Professional dark theme optimized for high resolution
    ClearBackground(Color{12, 12, 24, 255});
    
    // Adaptive window sizing and positioning
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    
    // Enhanced UI layout for ultra-high resolution
    int ui_height = 180; // More space for enhanced UI
    int display_height = window_height - ui_height;
    
    // Calculate optimal scaling with aspect ratio preservation
    float scale_x = static_cast<float>(window_width - 40) / nx; // Margins
    float scale_y = static_cast<float>(display_height - 40) / ny;
    float scale = std::min(scale_x, scale_y);
    
    // Center the ultra-high resolution texture
    float offset_x = (window_width - nx * scale) / 2.0f;
    float offset_y = (display_height - ny * scale) / 2.0f + 70;
    
    // Draw with ultra-high quality antialiasing
    DrawTextureEx(texture, Vector2{offset_x, offset_y}, 0.0f, scale, WHITE);
    
    // Enhanced professional UI for ultra-high resolution
    DrawText("Ultra-High Resolution Magnetic Field Simulator - FEMM Clone", 10, 10, 28, WHITE);
    
    // Enhanced status display
    std::ostringstream status_text;
    status_text << "?? Color Range: " << std::fixed << std::setprecision(3) << color_range 
                << " | ?? Resolution: " << nx << "×" << ny << " (" << total_pixels << " pixels)"
                << " | ?? Active Field: " << significant_pixels << " points ("
                << std::setprecision(1) << (100.0 * significant_pixels / total_pixels) << "%)";
    DrawText(status_text.str().c_str(), 10, 40, 16, LIGHTGRAY);
    
    // Performance indicator
    if (performance_samples > 0) {
        double avg_render_time = total_render_time / performance_samples;
        double estimated_fps = 1000.0 / avg_render_time;
        
        std::ostringstream perf_text;
        perf_text << "? Performance: " << std::fixed << std::setprecision(1) 
                  << estimated_fps << " FPS (" << avg_render_time << "ms/frame)";
        DrawText(perf_text.str().c_str(), window_width - 300, 40, 16, 
                 estimated_fps > 25 ? GREEN : (estimated_fps > 15 ? YELLOW : RED));
    }
    
    // Enhanced ultra-high resolution color legend
    int legend_y = window_height - 140;
    DrawText("?? Ultra-High Resolution Field Strength Legend:", 10, legend_y, 18, WHITE);
    
    // Professional color scale bar with enhanced gradation
    int bar_width = std::min(600, window_width - 200);
    int bar_height = 40;
    int bar_x = (window_width - bar_width) / 2;
    int bar_y = legend_y + 30;
    
    // Enhanced border with gradient
    DrawRectangleLines(bar_x - 3, bar_y - 3, bar_width + 6, bar_height + 6, WHITE);
    DrawRectangleLines(bar_x - 2, bar_y - 2, bar_width + 4, bar_height + 4, LIGHTGRAY);
    
    // Ultra-smooth color gradient bar
    for (int i = 0; i < bar_width; ++i) {
        float t = (static_cast<float>(i) / bar_width) * 2.0f - 1.0f; // Map to [-1, 1]
        Color bar_color = mapValue(t * static_cast<float>(color_range));
        DrawRectangle(bar_x + i, bar_y, 1, bar_height, bar_color);
    }
    
    // Enhanced scale labels with precise values
    std::ostringstream south_label, north_label, center_label;
    south_label << "S (-" << std::fixed << std::setprecision(2) << color_range << ")";
    north_label << "N (+" << std::fixed << std::setprecision(2) << color_range << ")";
    
    DrawText(south_label.str().c_str(), bar_x - 60, bar_y + 12, 14, Color{0, 255, 255, 255}); // CYAN
    DrawText(north_label.str().c_str(), bar_x + bar_width + 10, bar_y + 12, 14, Color{255, 100, 100, 255});
    DrawText("0", bar_x + bar_width/2 - 8, bar_y + 45, 14, WHITE);
    
    // Field strength indicators
    int quarter = bar_width / 4;
    DrawText("Weak", bar_x + quarter - 20, bar_y + 45, 12, LIGHTGRAY);
    DrawText("Weak", bar_x + 3*quarter - 20, bar_y + 45, 12, LIGHTGRAY);
    DrawText("Strong", bar_x + quarter/2 - 25, bar_y + 45, 12, LIGHTGRAY);
    DrawText("Strong", bar_x + bar_width - quarter/2 - 25, bar_y + 45, 12, LIGHTGRAY);
    
    // Enhanced control instructions
    DrawText("?? Controls: ???? (coarse ±0.05) | ???? (fine ±0.02) | ?? R (reset) | ? ESC (quit)", 
             10, window_height - 30, 16, WHITE);
    
    // Ultra-high resolution quality indicator
    std::ostringstream quality_text;
    quality_text << "? Ultra-HD Quality: Bilinear Antialiasing | Scale: " 
                 << std::setprecision(2) << scale << "x";
    DrawText(quality_text.str().c_str(), 10, window_height - 55, 14, Color{255, 215, 0, 255}); // GOLD
    
    EndDrawing();
    
    // Performance measurement
    auto render_end = std::chrono::high_resolution_clock::now();
    auto render_duration = std::chrono::duration_cast<std::chrono::microseconds>(render_end - render_start);
    double render_time_ms = render_duration.count() / 1000.0;
    
    total_render_time += render_time_ms;
    performance_samples++;
    
    // Performance statistics every 10 seconds
    if (frame_count % 600 == 0 && performance_samples > 100) {
        double avg_frame_time = total_render_time / performance_samples;
        std::cout << "?? Ultra-HD Performance: " << std::fixed << std::setprecision(1)
                  << (1000.0 / avg_frame_time) << " FPS, " << avg_frame_time 
                  << "ms/frame (" << total_pixels << " pixels)" << std::endl;
    }
}
