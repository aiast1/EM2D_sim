#include "FDTD.hpp"
#include "Renderer.hpp"
#include "Config.hpp"
#include <raylib.h>
#include <iostream>
#include <chrono>
#include <iomanip>

int main() {
    std::cout << "Starting Ultra-High Resolution Magnetic Field Simulator - FEMM Clone with Raylib..." << std::endl;
    
    Config cfg;
    
    // Try to load config from file first, with fallback to hardcoded values
    std::cout << "Attempting to load ultra-high resolution magnet configuration..." << std::endl;
    auto cfg_opt = Config::loadFromFile("em2d_sfml/assets/config.json");
    if (cfg_opt) {
        cfg = *cfg_opt;
        std::cout << "Loaded configuration successfully!" << std::endl;
        std::cout << "Scenario: " << cfg.scenario << std::endl;
        std::cout << "Ultra-High Resolution Grid: " << cfg.grid.nx << "x" << cfg.grid.ny << " (" 
                  << (cfg.grid.nx * cfg.grid.ny) << " field points)" << std::endl;
        std::cout << "Magnets configured: " << cfg.magnets.size() << std::endl;
        
        // Performance warning for very high resolutions
        if (cfg.grid.nx * cfg.grid.ny > 800000) {
            std::cout << "??  WARNING: Ultra-high resolution detected!" << std::endl;
            std::cout << "   This will require significant CPU and memory resources." << std::endl;
            std::cout << "   For best performance, close other applications." << std::endl;
        }
    } else {
        std::cout << "Config file not found, using optimized high-resolution defaults" << std::endl;
        // Fallback to optimized high-resolution config
        cfg.grid.nx = 768;  // Good balance of quality and performance
        cfg.grid.ny = 768;
        cfg.grid.dx = 0.0007;
        cfg.grid.dy = 0.0007;
        cfg.vis.color_range = 1.6;
        cfg.vis.field = "B";
        cfg.scenario = "optimized_high_resolution_fallback";
    }

    std::cout << "Initializing magnetic field simulation..." << std::endl;
    
    // Performance timing
    auto start_time = std::chrono::high_resolution_clock::now();
    
    FDTD sim(cfg.grid.nx, cfg.grid.ny, cfg.grid.dx, cfg.grid.dy);

    // Add materials from config
    if (!cfg.materials.empty()) {
        std::cout << "Adding " << cfg.materials.size() << " material blocks" << std::endl;
        for (auto &m : cfg.materials) {
            sim.addMaterialBlock(m.x0, m.y0, m.w, m.h, m.eps_r);
        }
    }

    // Add sources from config
    if (!cfg.sources.empty()) {
        std::cout << "Adding " << cfg.sources.size() << " sources" << std::endl;
        for (auto &s : cfg.sources) {
            sim.addSource(s);
        }
    }

    // Add magnets from config
    std::cout << "Adding " << cfg.magnets.size() << " configured magnets" << std::endl;
    for (auto &m : cfg.magnets) {
        sim.addMagnet(m);
    }

    // Adaptive window sizing based on resolution
    int window_width = 1400;   // Larger window for ultra-high res
    int window_height = 1000;  // Maintain aspect ratio
    
    // Adjust window size if resolution is very high
    if (cfg.grid.nx >= 1024) {
        window_width = 1600;
        window_height = 1100;
        std::cout << "Using expanded window size for ultra-high resolution display" << std::endl;
    }
    
    InitWindow(window_width, window_height, "Ultra-High Resolution Magnetic Field Simulator - FEMM Clone (Raylib)");
    
    // Adaptive FPS based on resolution
    int target_fps = 60;
    if (cfg.grid.nx * cfg.grid.ny > 800000) {
        target_fps = 30;  // Lower FPS for very high resolutions
        std::cout << "Using 30 FPS for ultra-high resolution (performance optimization)" << std::endl;
    } else {
        std::cout << "Using 60 FPS for optimal interactive experience" << std::endl;
    }
    SetTargetFPS(target_fps);
    
    std::cout << "Raylib window initialized: " << window_width << "x" << window_height << std::endl;
    
    std::cout << "Initializing ultra-high resolution magnetic field renderer" << std::endl;
    Renderer renderer(cfg.grid.nx, cfg.grid.ny, cfg.vis.color_range);
    
    std::cout << "Creating ultra-detailed magnetic field pattern..." << std::endl;
    std::cout << "Scenario: " << cfg.scenario << std::endl;
    std::cout << "\n?? Interactive Controls:" << std::endl;
    std::cout << "  ????  UP/DOWN arrows = Adjust color range (coarse ±0.05)" << std::endl;
    std::cout << "  ????  LEFT/RIGHT arrows = Fine-tune color range (±0.02)" << std::endl;
    std::cout << "  ??  R = Reset color range to default" << std::endl;
    std::cout << "  ?  ESC = Quit application" << std::endl;
    std::cout << "\n?? Ultra-High Resolution Color Legend:" << std::endl;
    std::cout << "  ?? Deep Blue/Purple = Very strong South pole field" << std::endl;
    std::cout << "  ?? Blue = Strong South field" << std::endl;
    std::cout << "  ?? Cyan = Medium South field" << std::endl;
    std::cout << "  ?? Green = Neutral/weak field" << std::endl;
    std::cout << "  ?? Yellow = Medium North field" << std::endl;
    std::cout << "  ?? Orange = Strong North field" << std::endl;
    std::cout << "  ?? Red = Very strong North pole field" << std::endl;

    // Initialize the ultra-detailed magnetic field pattern
    std::cout << "\nComputing ultra-high resolution magnetic field..." << std::endl;
    sim.step();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "? Field computation completed in " << duration.count() << "ms" << std::endl;

    std::cout << "\n?? Starting interactive ultra-high resolution magnetic field visualization!" << std::endl;
    std::cout << "?? Tip: Use UP/DOWN arrows to explore different field sensitivity levels" << std::endl;

    // Variables for adjustable color bounds
    const float default_color_range = static_cast<float>(cfg.vis.color_range);
    
    // Performance monitoring
    int frame_count = 0;
    double total_frame_time = 0.0;
    
    // Main game loop with interactive controls
    while (!WindowShouldClose()) {
        auto frame_start = std::chrono::high_resolution_clock::now();
        
        // Handle input for adjustable color bounds
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        
        float current_range = static_cast<float>(renderer.getColorRange());
        bool range_changed = false;
        
        // Adjust color range with arrow keys
        if (IsKeyDown(KEY_UP)) {
            current_range += 0.05f; // Smooth adjustment
            range_changed = true;
        }
        if (IsKeyDown(KEY_DOWN)) {
            current_range -= 0.05f;
            if (current_range < 0.1f) current_range = 0.1f; // Minimum range
            range_changed = true;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            current_range += 0.02f; // Fine adjustment
            range_changed = true;
        }
        if (IsKeyDown(KEY_LEFT)) {
            current_range -= 0.02f; // Fine adjustment
            if (current_range < 0.1f) current_range = 0.1f; // Minimum range
            range_changed = true;
        }
        if (IsKeyPressed(KEY_R)) {
            current_range = default_color_range;
            range_changed = true;
            std::cout << "?? Color range reset to default: " << current_range << std::endl;
        }
        
        // Update renderer color range if changed
        if (range_changed) {
            renderer.setColorRange(current_range);
        }
        
        // Render the ultra-high resolution magnetic field
        renderer.render(sim.getEz());
        
        // Performance monitoring
        frame_count++;
        auto frame_end = std::chrono::high_resolution_clock::now();
        auto frame_duration = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
        total_frame_time += frame_duration.count() / 1000.0; // Convert to milliseconds
        
        // Display performance info every 5 seconds
        if (frame_count % (target_fps * 5) == 0) {
            double avg_frame_time = total_frame_time / frame_count;
            double current_fps = 1000.0 / avg_frame_time;
            std::cout << "?? Performance: Avg " << std::fixed << std::setprecision(1) 
                      << current_fps << " FPS (" << avg_frame_time << "ms/frame)" << std::endl;
        }
    }

    // Cleanup Raylib
    CloseWindow();
    
    std::cout << "\n? Ultra-high resolution magnetic field simulation ended successfully!" << std::endl;
    std::cout << "?? Final Stats:" << std::endl;
    std::cout << "   Total frames rendered: " << frame_count << std::endl;
    std::cout << "   Resolution: " << cfg.grid.nx << "x" << cfg.grid.ny 
              << " (" << (cfg.grid.nx * cfg.grid.ny) << " field points)" << std::endl;
    std::cout << "   Magnets simulated: " << cfg.magnets.size() << std::endl;
    
    if (frame_count > 0) {
        double avg_fps = frame_count / (total_frame_time / 1000.0);
        std::cout << "   Average performance: " << std::fixed << std::setprecision(1) << avg_fps << " FPS" << std::endl;
    }
    
    return 0;
}
