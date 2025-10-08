#include "FDTD.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <execution>
#include <numeric>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double mu0 = 4.0*M_PI*1e-7;
const double c0 = 3e8;
const double eps0 = 1.0/(mu0*c0*c0);

FDTD::FDTD(int nx_, int ny_, double dx_, double dy_)
: nx(nx_), ny(ny_), dx(dx_), dy(dy_) {
    // Reserve memory for better performance
    Ez.reserve(nx*ny);
    Hx.reserve(nx*ny);
    Hy.reserve(nx*ny);
    eps_r.reserve(nx*ny);
    
    Ez.assign(nx*ny, 0.0f);
    Hx.assign(nx*ny, 0.0f);
    Hy.assign(nx*ny, 0.0f);
    eps_r.assign(nx*ny, 1.0f);

    // CFL stability condition
    double dt_cfl = 1.0 / (c0 * std::sqrt(1.0/(dx*dx) + 1.0/(dy*dy)));
    dt = 0.99 * dt_cfl;

    std::cout << "FDTD initialized: " << nx << "x" << ny << " (" << (nx*ny) << " points), dt=" << dt << std::endl;
    
    // Memory usage estimation
    size_t total_memory = (Ez.capacity() + Hx.capacity() + Hy.capacity() + eps_r.capacity()) * sizeof(float);
    std::cout << "Estimated memory usage: " << (total_memory / 1024 / 1024) << " MB" << std::endl;
}

void FDTD::reset() {
    std::fill(std::execution::par_unseq, Ez.begin(), Ez.end(), 0.0f);
    std::fill(std::execution::par_unseq, Hx.begin(), Hx.end(), 0.0f);
    std::fill(std::execution::par_unseq, Hy.begin(), Hy.end(), 0.0f);
    for (auto &s: sources) s.reset();
    std::cout << "FDTD reset with parallel algorithms" << std::endl;
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
        float val = s.value(static_cast<double>(nstep));
        Ez[idx(i,j)] += val;

        // Reduced debug output for better performance
        if (nstep % 120 == 0 && nstep < 300) {
            std::cout << "Step " << nstep << ": Source at (" << i << "," << j << ") value=" << val << std::endl;
        }
    }
}

void FDTD::step() {
    static bool initialized = false;
    
    if (!initialized) {
        std::cout << "Computing ultra-high resolution magnetic field pattern from configured magnets..." << std::endl;
        
        // Clear the field first with parallel algorithm
        std::fill(std::execution::par_unseq, Ez.begin(), Ez.end(), 0.0f);
        
        if (magnet_configs.empty()) {
            std::cout << "No magnets configured - using optimized default pattern" << std::endl;
            // Enhanced fallback pattern for high resolution
            std::vector<MagnetConfig> default_magnets = {
                {nx/2, ny/2, 0.0, 1.0, 2.5, "center_north_primary"},
                {nx/3, ny/2, 0.0, -1.0, 2.0, "left_south_primary"},
                {2*nx/3, ny/2, 0.0, -1.0, 2.0, "right_south_primary"},
                {nx/2, ny/3, 1.0, 0.0, 1.8, "top_east_secondary"},
                {nx/2, 2*ny/3, -1.0, 0.0, 1.8, "bottom_west_secondary"}
            };
            magnet_configs = default_magnets;
        }
        
        std::cout << "Computing magnetic dipole fields from " << magnet_configs.size() << " magnets..." << std::endl;
        for (const auto& magnet : magnet_configs) {
            std::cout << "  - " << magnet.name << " at (" << magnet.x << "," << magnet.y 
                      << ") strength=" << magnet.strength << std::endl;
        }
        
        // Pre-compute constants for better performance
        const float scale_factor = 80.0f; // Optimized scaling for high-resolution visualization
        const float min_distance_sq = 4.0f;
        const float field_clamp_min = -5.0f;
        const float field_clamp_max = 5.0f;
        
        // Progress reporting for large computations
        const int total_points = nx * ny;
        const size_t progress_interval = static_cast<size_t>(total_points) / 20; // Report every 5%
        size_t points_computed = 0;
        
        std::cout << "Computing " << total_points << " field points..." << std::endl;
        
        // Optimized field computation with better algorithms
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                float total_field = 0.0f;
                
                // Vectorized computation for all magnetic dipoles
                for (const auto& magnet : magnet_configs) {
                    const float dx_val = static_cast<float>(i - magnet.x);
                    const float dy_val = static_cast<float>(j - magnet.y);
                    const float r_sq = dx_val*dx_val + dy_val*dy_val;
                    
                    if (r_sq > min_distance_sq) {
                        // Optimized magnetic dipole field calculation
                        const float r = std::sqrt(r_sq);
                        const float r_inv = 1.0f / r;
                        const float r_inv3 = r_inv * r_inv * r_inv;
                        
                        // Unit vector from dipole to field point
                        const float rx = dx_val * r_inv;
                        const float ry = dy_val * r_inv;
                        
                        // Magnetic dipole moment components
                        const float mx = static_cast<float>(magnet.moment_x);
                        const float my = static_cast<float>(magnet.moment_y);
                        
                        // Dot product: m?·r?
                        const float m_dot_r = mx * rx + my * ry;
                        
                        // Optimized magnetic field components: B = (3(m?·r?)r? - m?)/r³
                        const float Bx = (3.0f * m_dot_r * rx - mx) * r_inv3;
                        const float By = (3.0f * m_dot_r * ry - my) * r_inv3;
                        
                        // Field magnitude with optimized square root
                        const float field_magnitude = std::sqrt(Bx*Bx + By*By);
                        
                        // Add contribution with strength weighting
                        total_field += static_cast<float>(magnet.strength) * field_magnitude * scale_factor;
                        
                    } else {
                        // Enhanced pole field calculation for magnet locations
                        float pole_strength = 0.0f;
                        const float abs_mx = std::abs(static_cast<float>(magnet.moment_x));
                        const float abs_my = std::abs(static_cast<float>(magnet.moment_y));
                        
                        if (abs_my > abs_mx) {
                            // Primarily vertical orientation
                            pole_strength = static_cast<float>(magnet.moment_y) > 0 ? 4.0f : -4.0f;
                        } else {
                            // Primarily horizontal orientation  
                            pole_strength = static_cast<float>(magnet.moment_x) > 0 ? 4.0f : -4.0f;
                        }
                        
                        total_field += static_cast<float>(magnet.strength) * pole_strength;
                    }
                }
                
                // Enhanced field clamping for better visualization
                total_field = std::clamp(total_field, field_clamp_min, field_clamp_max);
                Ez[idx(i,j)] = total_field;
                
                // Progress reporting for large computations
                points_computed++;
                if (points_computed % progress_interval == 0) {
                    int progress_percent = (points_computed * 100) / total_points;
                    std::cout << "Progress: " << progress_percent << "% (" << points_computed 
                              << "/" << total_points << " points)" << std::endl;
                }
            }
        }
        
        // Compute field statistics for quality assessment
        auto minmax = std::minmax_element(Ez.begin(), Ez.end());
        float field_min = *minmax.first;
        float field_max = *minmax.second;
        
        // Count non-zero field points
        int nonzero_points = std::count_if(Ez.begin(), Ez.end(), [](float val) {
            return std::abs(val) > 0.01f;
        });
        
        std::cout << "? Ultra-high resolution magnetic field computation completed!" << std::endl;
        std::cout << "?? Field Statistics:" << std::endl;
        std::cout << "   Field range: [" << field_min << ", " << field_max << "]" << std::endl;
        std::cout << "   Active field points: " << nonzero_points << "/" << total_points 
                  << " (" << (100.0 * nonzero_points / total_points) << "%)" << std::endl;
        std::cout << "   Magnets: " << magnet_configs.size() << " configured" << std::endl;
        std::cout << "   Resolution: " << nx << "×" << ny << " for maximum detail visualization" << std::endl;
        
        initialized = true;
    }
    
    // Static field - no time evolution needed for magnetic visualization
}
