#pragma once

#include <vector>
#include <cstddef>
#include <cmath>
#include <iostream>
#include "Config.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Consolidated Source class (previously in Source.hpp)
class Source {
public:
    Source() = default;
    Source(const SourceConfig &c) : conf(c) {
        std::cout << "Source created: type=" << c.type << " at (" << c.x << "," << c.y 
                  << ") amplitude=" << c.amplitude << std::endl;
    }
    
    float value(double t) const {
        float result = 0.0f;
        if (conf.type == "gaussian") {
            // For gaussian, assume t is the time step number
            double arg = (t - conf.t0) / conf.spread;
            result = static_cast<float>(conf.amplitude * std::exp(-arg*arg));
        } else if (conf.type == "cw") {
            result = static_cast<float>(conf.amplitude * std::sin(2.0*M_PI*conf.freq_hz*t));
        } else if (conf.type == "static") {
            // Static source for testing - always returns the same value
            result = static_cast<float>(conf.amplitude);
        }
        
        return result;
    }
    
    void reset() {}

    SourceConfig conf;
};

class FDTD {
public:
    FDTD(int nx, int ny, double dx, double dy);
    void reset();
    void step();

    void addMaterialBlock(int x0, int y0, int w, int h, double eps_r);
    void addSource(const SourceConfig &sconf);
    void addMagnet(const MagnetConfig &mconf); // New: add magnet configuration

    const std::vector<float>& getEz() const { return Ez; }

private:
    int nx, ny;
    double dx, dy;
    double dt;

    std::vector<float> Ez;
    std::vector<float> Hx;
    std::vector<float> Hy;
    std::vector<float> eps_r;

    std::vector<Source> sources;
    std::vector<MagnetConfig> magnet_configs; // New: store magnet configurations

    void applySources(int nstep);
    inline int idx(int i, int j) const { return j*nx + i; }
};
