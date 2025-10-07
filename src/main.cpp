#include "FDTD.hpp"
#include "Renderer.hpp"
#include "Config.hpp"
#include <raylib.h>
#include <iostream>

int main() {
    std::cout << "Starting High-Resolution Magnetic Field Simulator - FEMM Clone with Raylib..." << std::endl;
    
    Config cfg;
    
    // Try to load config from file first, with fallback to hardcoded values
    std::cout << "Attempting to load organized magnet configuration..." << std::endl;
    auto cfg_opt = Config::loadFromFile("em2d_sfml/assets/config.json");
    if (cfg_opt) {
        cfg = *cfg_opt;
        std::cout << "Loaded configuration successfully!" << std::endl;
        std::cout << "Scenario: " << cfg.scenario << std::endl;
        std::cout << "Grid: " << cfg.grid.nx << "x" << cfg.grid.ny << std::endl;
        std::cout << "Magnets configured: " << cfg.magnets.size() << std::endl;
    } else {
        std::cout << "Config file not found, using high-resolution defaults" << std::endl;
        // Fallback to hardcoded high-resolution config
        cfg.grid.nx = 512;
        cfg.grid.ny = 512;
        cfg.grid.dx = 0.001;
        cfg.grid.dy = 0.001;
        cfg.vis.color_range = 1.8;
        cfg.vis.field = "B";
        cfg.scenario = "default_fallback";
    }

    std::cout << "Initializing magnetic field simulation with grid " << cfg.grid.nx << "x" << cfg.grid.ny << std::endl;
    FDTD sim(cfg.grid.nx, cfg.grid.ny, cfg.grid.dx, cfg.grid.dy);

    // Add materials from config
    std::cout << "Adding " << cfg.materials.size() << " material blocks" << std::endl;
    for (auto &m : cfg.materials) {
        sim.addMaterialBlock(m.x0, m.y0, m.w, m.h, m.eps_r);
    }

    // Add sources from config
    std::cout << "Adding " << cfg.sources.size() << " sources" << std::endl;
    for (auto &s : cfg.sources) {
        sim.addSource(s);
    }

    // Add magnets from config
    std::cout << "Adding " << cfg.magnets.size() << " configured magnets" << std::endl;
    for (auto &m : cfg.magnets) {
        sim.addMagnet(m);
    }

    // Initialize Raylib with larger window for high-res display
    const int window_width = 1200;
    const int window_height = 900;
    
    InitWindow(window_width, window_height, "High-Res Magnetic Field Simulator - FEMM Clone (Raylib)");
    SetTargetFPS(60); // Higher FPS for smooth interaction
    
    std::cout << "Raylib window initialized: " << window_width << "x" << window_height << std::endl;
    
    std::cout << "Initializing high-resolution magnetic field renderer" << std::endl;
    Renderer renderer(cfg.grid.nx, cfg.grid.ny, cfg.vis.color_range);
    
    std::cout << "Creating organized magnetic field pattern..." << std::endl;
    std::cout << "Scenario: " << cfg.scenario << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  UP/DOWN arrows = Adjust color range (coarse)" << std::endl;
    std::cout << "  LEFT/RIGHT arrows = Fine-tune color range" << std::endl;
    std::cout << "  R = Reset color range to default" << std::endl;
    std::cout << "  ESC = Quit" << std::endl;
    std::cout << "Enhanced Color Legend:" << std::endl;
    std::cout << "  Deep Blue/Purple = Very strong South pole field" << std::endl;
    std::cout << "  Blue = Strong South field" << std::endl;
    std::cout << "  Cyan = Medium South field" << std::endl;
    std::cout << "  Green = Neutral/weak field" << std::endl;
    std::cout << "  Yellow = Medium North field" << std::endl;
    std::cout << "  Orange = Strong North field" << std::endl;
    std::cout << "  Red = Very strong North pole field" << std::endl;

    // Initialize the organized magnetic field pattern
    sim.step();

    std::cout << "Starting interactive organized magnetic field visualization!" << std::endl;

    // Variables for adjustable color bounds
    const float default_color_range = static_cast<float>(cfg.vis.color_range);
    
    // Main game loop with interactive controls
    while (!WindowShouldClose()) {
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
            std::cout << "Color range reset to default: " << current_range << std::endl;
        }
        
        // Update renderer color range if changed
        if (range_changed) {
            renderer.setColorRange(current_range);
        }
        
        // Render the organized magnetic field
        renderer.render(sim.getEz());
    }

    // Cleanup Raylib
    CloseWindow();
    
    std::cout << "High-resolution organized magnetic field simulation ended!" << std::endl;
    return 0;
}
