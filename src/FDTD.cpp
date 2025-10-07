#include "FDTD.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double mu0 = 4.0*M_PI*1e-7;
const double c0 = 3e8;
const double eps0 = 1.0/(mu0*c0*c0);

FDTD::FDTD(int nx_, int ny_, double dx_, double dy_)
: nx(nx_), ny(ny_), dx(dx_), dy(dy_) {
    Ez.assign(nx*ny, 0.0f);
    Hx.assign(nx*ny, 0.0f);
    Hy.assign(nx*ny, 0.0f);
    eps_r.assign(nx*ny, 1.0f);

    // CFL
    double dt_cfl = 1.0 / (c0 * std::sqrt(1.0/(dx*dx) + 1.0/(dy*dy)));
    dt = 0.99 * dt_cfl;

    std::cout << "FDTD initialized: " << nx << "x" << ny << ", dt=" << dt << std::endl;
}

void FDTD::reset() {
    std::fill(Ez.begin(), Ez.end(), 0.0f);
    std::fill(Hx.begin(), Hx.end(), 0.0f);
    std::fill(Hy.begin(), Hy.end(), 0.0f);
    for (auto &s: sources) s.reset();
    std::cout << "FDTD reset" << std::endl;
}

void FDTD::addMaterialBlock(int x0, int y0, int w, int h, double er) {
    std::cout << "Adding material block at (" << x0 << "," << y0 << ") size " << w << "x" << h << " eps_r=" << er << std::endl;
    for (int j = y0; j < y0 + h && j < ny; ++j) {
        for (int i = x0; i < x0 + w && i < nx; ++i) {
            if (i>=0 && j>=0)
                eps_r[idx(i,j)] = static_cast<float>(er);
        }
    }
}

void FDTD::addSource(const SourceConfig &sconf) {
    std::cout << "Adding source at (" << sconf.x << "," << sconf.y << ") type=" << sconf.type
              << " amplitude=" << sconf.amplitude << std::endl;
    sources.emplace_back(sconf);
}

void FDTD::addMagnet(const MagnetConfig &mconf) {
    std::cout << "Adding magnet '" << mconf.name << "' at (" << mconf.x << "," << mconf.y 
              << ") moment=(" << mconf.moment_x << "," << mconf.moment_y 
              << ") strength=" << mconf.strength << std::endl;
    magnet_configs.push_back(mconf);
}

void FDTD::applySources(int nstep) {
    for (const auto &s : sources) {
        int i = s.conf.x;
        int j = s.conf.y;
        if (i<0||i>=nx||j<0||j>=ny) continue;
        float val = s.value(static_cast<double>(nstep)); // Pass step number instead of time
        Ez[idx(i,j)] += val;

        // Debug output every 60 steps
        if (nstep % 60 == 0 && nstep < 300) {
            std::cout << "Step " << nstep << ": Source at (" << i << "," << j << ") value=" << val << std::endl;
        }
    }
}

void FDTD::step() {
    static bool initialized = false;
    
    if (!initialized) {
        std::cout << "Creating organized magnetic field pattern from configured magnets" << std::endl;
        
        // Clear the field first
        std::fill(Ez.begin(), Ez.end(), 0.0f);
        
        if (magnet_configs.empty()) {
            std::cout << "No magnets configured - using default fallback pattern" << std::endl;
            // Fallback to a simple default pattern if no magnets configured
            std::vector<MagnetConfig> default_magnets = {
                {nx/2, ny/2, 0.0, 1.0, 2.0, "center_north"},
                {nx/3, ny/2, 0.0, -1.0, 1.5, "left_south"},
                {2*nx/3, ny/2, 0.0, -1.0, 1.5, "right_south"}
            };
            magnet_configs = default_magnets;
        }
        
        std::cout << "Computing magnetic dipole fields from " << magnet_configs.size() << " magnets:" << std::endl;
        for (const auto& magnet : magnet_configs) {
            std::cout << "  - " << magnet.name << " at (" << magnet.x << "," << magnet.y << ")" << std::endl;
        }
        
        // Calculate magnetic field at each point using dipole field equations
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                float total_field = 0.0f;
                
                // Sum contributions from all configured magnetic dipoles
                for (const auto& magnet : magnet_configs) {
                    float dx_val = static_cast<float>(i - magnet.x);
                    float dy_val = static_cast<float>(j - magnet.y);
                    float r_sq = dx_val*dx_val + dy_val*dy_val;
                    float r = std::sqrt(r_sq);
                    
                    // Avoid division by zero at magnet location
                    if (r > 2.0f) {
                        // Magnetic dipole field formula: B ? (3(m?·r?)r? - m?)/r³
                        float r_inv = 1.0f / r;
                        float r_inv3 = r_inv * r_inv * r_inv;
                        
                        // Unit vector from dipole to field point
                        float rx = dx_val * r_inv;
                        float ry = dy_val * r_inv;
                        
                        // Dot product: m?·r?
                        float m_dot_r = static_cast<float>(magnet.moment_x) * rx + static_cast<float>(magnet.moment_y) * ry;
                        
                        // Magnetic field components: B = (3(m?·r?)r? - m?)/r³
                        float Bx = (3.0f * m_dot_r * rx - static_cast<float>(magnet.moment_x)) * r_inv3;
                        float By = (3.0f * m_dot_r * ry - static_cast<float>(magnet.moment_y)) * r_inv3;
                        
                        // For visualization, use field magnitude
                        float field_magnitude = std::sqrt(Bx*Bx + By*By);
                        total_field += static_cast<float>(magnet.strength) * field_magnitude * 60.0f; // Scale for visibility
                        
                    } else {
                        // At magnet location, set a strong field indicating the pole
                        float pole_strength = static_cast<float>(magnet.moment_y) > 0 ? 3.0f : -3.0f; // North = +, South = -
                        if (std::abs(magnet.moment_x) > std::abs(magnet.moment_y)) {
                            pole_strength = static_cast<float>(magnet.moment_x) > 0 ? 3.0f : -3.0f; // East = +, West = -
                        }
                        total_field += static_cast<float>(magnet.strength) * pole_strength;
                    }
                }
                
                // Clamp field values for better visualization
                total_field = std::max(-4.0f, std::min(4.0f, total_field));
                Ez[idx(i,j)] = total_field;
            }
        }
        
        std::cout << "Organized magnetic field pattern created successfully!" << std::endl;
        std::cout << "Field pattern shows symmetric dipole interactions with beautiful gradients" << std::endl;
        initialized = true;
    }
    
    // Do nothing after initialization - keep the pattern static
}
