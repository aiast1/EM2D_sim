#pragma once

#include <string>
#include <vector>
#include <optional>

struct GridConfig {
    int nx = 256;
    int ny = 256;
    double dx = 0.002;
    double dy = 0.002;
};

struct MaterialBlock {
    int x0 = 0, y0 = 0, w = 10, h = 10;
    double eps_r = 1.0;
};

struct SourceConfig {
    std::string type = "gaussian"; // gaussian or cw
    int x = 0;
    int y = 0;
    double amplitude = 1.0;
    double t0 = 50.0;
    double spread = 20.0;
    double freq_hz = 1e8;
};

struct MagnetConfig {
    int x = 0;
    int y = 0;
    double moment_x = 0.0;  // Magnetic moment X component
    double moment_y = 1.0;  // Magnetic moment Y component
    double strength = 1.0;  // Magnet strength
    std::string name = "magnet"; // Optional name for identification
};

struct VisualConfig {
    std::string field = "Ez";
    double color_range = 1.0;
};

struct Config {
    GridConfig grid;
    int max_steps = 10000;
    std::vector<MaterialBlock> materials;
    std::vector<SourceConfig> sources;
    std::vector<MagnetConfig> magnets; // New: magnet configurations
    VisualConfig vis;
    std::string scenario = "default"; // New: scenario name

    static std::optional<Config> loadFromFile(const std::string &path);
};
