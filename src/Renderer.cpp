#include "Renderer.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

Renderer::Renderer(int nx_, int ny_, double color_range_)
: nx(nx_), ny(ny_), color_range(color_range_), texture_needs_update(true) {
    std::cout << "Creating Raylib renderer for " << nx << "x" << ny << " grid with color range " << color_range << std::endl;
    
    // Create an image - much simpler in Raylib!
    image = GenImageColor(nx, ny, BLACK);
    
    // Create texture from image
    texture = LoadTextureFromImage(image);
    
    // Enable texture filtering for antialiasing/smoothing
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    std::cout << "Enabled bilinear filtering for smooth antialiasing" << std::endl;
    
    std::cout << "Raylib renderer initialized successfully!" << std::endl;
}

Renderer::~Renderer() {
    // Clean up Raylib resources
    UnloadTexture(texture);
    UnloadImage(image);
}

void Renderer::setColorRange(double new_range) {
    color_range = new_range;
    std::cout << "Color range updated to: " << color_range << std::endl;
}

Color Renderer::mapValue(float v) {
    // Normalize to color range
    float normalized = v / static_cast<float>(color_range);
    normalized = std::max(-1.0f, std::min(1.0f, normalized));
    
    // Enhanced FEMM-style magnetic field visualization with smoother gradients
    float abs_norm = std::abs(normalized);
    
    if (std::abs(normalized) < 0.01f) {
        // Very near zero: dark blue-green (like FEMM's zero field)
        return Color{0, 48, 64, 255};
    }
    
    if (normalized < -0.8f) {
        // Very strong negative field: Deep blue to purple
        float t = (abs_norm - 0.8f) / 0.2f;
        unsigned char red = static_cast<unsigned char>(64 * t);
        unsigned char blue = 255;
        return Color{red, 0, blue, 255};
    } else if (normalized < -0.4f) {
        // Strong negative field: Blue
        float t = (abs_norm - 0.4f) / 0.4f;
        unsigned char intensity = static_cast<unsigned char>(128 + 127 * t);
        return Color{0, 0, intensity, 255};
    } else if (normalized < -0.1f) {
        // Medium negative field: Blue to cyan transition
        float t = (abs_norm - 0.1f) / 0.3f;
        unsigned char blue = 255;
        unsigned char green = static_cast<unsigned char>(64 + 128 * t);
        return Color{0, green, blue, 255};
    } else if (normalized < 0.1f) {
        // Near zero field: Cyan to green (neutral field region)  
        float t = abs_norm / 0.1f;
        unsigned char red = 0;
        unsigned char green = static_cast<unsigned char>(64 + 96 * t);
        unsigned char blue = static_cast<unsigned char>(64 * (1.0f - t));
        return Color{red, green, blue, 255};
    } else if (normalized < 0.4f) {
        // Medium positive field: Green to yellow transition
        float t = (abs_norm - 0.1f) / 0.3f;
        unsigned char red = static_cast<unsigned char>(128 * t);
        unsigned char green = static_cast<unsigned char>(160 + 95 * t);
        unsigned char blue = 0;
        return Color{red, green, blue, 255};
    } else if (normalized < 0.8f) {
        // Strong positive field: Yellow to orange
        float t = (abs_norm - 0.4f) / 0.4f;
        unsigned char red = static_cast<unsigned char>(128 + 127 * t);
        unsigned char green = static_cast<unsigned char>(255 * (1.0f - 0.3f * t));
        unsigned char blue = 0;
        return Color{red, green, blue, 255};
    } else {
        // Very strong positive field: Orange to red
        float t = (abs_norm - 0.8f) / 0.2f;
        unsigned char red = 255;
        unsigned char green = static_cast<unsigned char>(180 * (1.0f - t));
        unsigned char blue = static_cast<unsigned char>(32 * t);
        return Color{red, green, blue, 255};
    }
}

void Renderer::render(const std::vector<float> &Ez) {
    static int frame_count = 0;
    frame_count++;
    
    // Debug info for first few frames only
    if (frame_count <= 3) {
        auto minmax = std::minmax_element(Ez.begin(), Ez.end());
        float min_val = *minmax.first;
        float max_val = *minmax.second;
        std::cout << "Frame " << frame_count << ": Magnetic field range [" << min_val << ", " << max_val << "]" << std::endl;
    }
    
    // Update image pixels - Raylib makes this super easy!
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            float v = Ez[j*nx + i]; // Ez now contains magnetic field data
            Color color = mapValue(v);
            ImageDrawPixel(&image, i, j, color);
        }
    }
    
    // Update texture with new image data
    UpdateTexture(texture, image.data);
    
    // Render everything
    BeginDrawing();
    
    ClearBackground(Color{16, 16, 32, 255}); // Dark blue-gray background like FEMM
    
    // Draw the magnetic field texture, scaled to fit the window
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    
    // Calculate scale to fit the texture in window, leaving room for UI
    int display_height = window_height - 150; // More space for enhanced UI
    float scale_x = static_cast<float>(window_width - 20) / nx; // Small margin
    float scale_y = static_cast<float>(display_height) / ny;
    float scale = std::min(scale_x, scale_y);
    
    // Center the texture
    float offset_x = (window_width - nx * scale) / 2.0f;
    float offset_y = (display_height - ny * scale) / 2.0f + 60; // Offset for title
    
    // Draw with antialiasing enabled by the bilinear filter
    DrawTextureEx(texture, Vector2{offset_x, offset_y}, 0.0f, scale, WHITE);
    
    // Enhanced FEMM-style UI
    DrawText("High-Resolution Magnetic Field Simulator - FEMM Clone", 10, 10, 24, WHITE);
    
    // Display current color range and controls
    std::ostringstream range_text;
    range_text << "Color Range: " << std::fixed << std::setprecision(2) << color_range 
               << " (UP/DOWN: coarse, LEFT/RIGHT: fine, R: reset) - Antialiased";
    DrawText(range_text.str().c_str(), 10, 35, 16, LIGHTGRAY);
    
    // Resolution info
    std::ostringstream res_text;
    res_text << "Resolution: " << nx << "x" << ny << " pixels (Bilinear Filtered)";
    DrawText(res_text.str().c_str(), window_width - 250, 35, 16, LIGHTGRAY);
    
    // Enhanced color legend with more detail
    int legend_y = window_height - 120;
    DrawText("Field Strength Legend:", 10, legend_y, 16, WHITE);
    
    // Draw enhanced color scale bar with more gradations
    int bar_width = 400;
    int bar_height = 30;
    int bar_x = 200;
    int bar_y = legend_y + 5;
    
    // Draw border around color bar
    DrawRectangleLines(bar_x - 2, bar_y - 2, bar_width + 4, bar_height + 4, WHITE);
    
    for (int i = 0; i < bar_width; ++i) {
        float t = (static_cast<float>(i) / bar_width) * 2.0f - 1.0f; // Map to [-1, 1]
        Color bar_color = mapValue(t * static_cast<float>(color_range));
        DrawRectangle(bar_x + i, bar_y, 1, bar_height, bar_color);
    }
    
    // Enhanced scale labels with numeric values
    std::ostringstream south_label, north_label, center_label;
    south_label << "S (-" << std::fixed << std::setprecision(1) << color_range << ")";
    north_label << "N (+" << std::fixed << std::setprecision(1) << color_range << ")";
    center_label << "0";
    
    DrawText(south_label.str().c_str(), bar_x - 45, bar_y + 8, 12, WHITE);
    DrawText(north_label.str().c_str(), bar_x + bar_width + 5, bar_y + 8, 12, WHITE);
    DrawText(center_label.str().c_str(), bar_x + bar_width/2 - 5, bar_y + 35, 12, LIGHTGRAY);
    
    // Add field strength indicators
    DrawText("Weak", bar_x + bar_width/4 - 15, bar_y + 35, 10, LIGHTGRAY);
    DrawText("Weak", bar_x + 3*bar_width/4 - 15, bar_y + 35, 10, LIGHTGRAY);
    
    // Instructions
    DrawText("Controls: UP/DOWN (coarse), LEFT/RIGHT (fine), R (reset), ESC (quit)", 
             10, window_height - 25, 14, LIGHTGRAY);
    
    EndDrawing();
}
