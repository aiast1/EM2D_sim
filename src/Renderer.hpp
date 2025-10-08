#pragma once

#include <raylib.h>
#include <vector>
#include <chrono>

// Ultra-High Resolution Magnetic Field Renderer
// Version: 2.0 - Professional FEMM-style visualization
// Features: 1024x1024 resolution, bilinear antialiasing, adaptive performance

class Renderer {
public:
    Renderer(int nx, int ny, double color_range = 1.0);
    ~Renderer();
    void render(const std::vector<float> &Ez);
    void setColorRange(double new_range); // New method for adjustable bounds
    double getColorRange() const { return color_range; }

private:
    int nx, ny;
    double color_range;
    Image image;
    Texture2D texture;
    bool texture_needs_update;

    Color mapValue(float v);
};
