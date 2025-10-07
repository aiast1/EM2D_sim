#pragma once

#include "Config.hpp"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
