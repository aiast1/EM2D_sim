# ?? Magnetic Field Simulator - FEMM Clone

A high-resolution, interactive magnetic field visualization tool built with C++20 and Raylib. This project provides a simplified clone of the popular FEMM (Finite Element Method Magnetics) software, allowing users to visualize magnetic dipole fields with professional-quality antialiased rendering.

![Magnetic Field Visualization](https://img.shields.io/badge/Status-Working-brightgreen)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![Raylib](https://img.shields.io/badge/Graphics-Raylib-orange)
![CMake](https://img.shields.io/badge/Build-CMake-red)

## ? Features

### ?? Professional Visualization
- **High-resolution 512×512 field computation** with bilinear antialiasing
- **FEMM-style color mapping** with 7-zone professional gradient:
  - Deep Blue/Purple ? Very strong South pole field
  - Blue ? Strong South field  
  - Cyan ? Medium South field
  - Green ? Neutral/weak field
  - Yellow ? Medium North field
  - Orange ? Strong North field
  - Red ? Very strong North pole field

### ?? Interactive Controls
- **Real-time color range adjustment** (UP/DOWN arrows for coarse, LEFT/RIGHT for fine)
- **Dynamic field sensitivity** from 0.1 to 4.0+ range
- **Reset functionality** (R key) to return to default settings
- **Smooth 60 FPS** rendering with responsive controls

### ?? Physics Accuracy
- **Realistic magnetic dipole fields** using proper B? ? (3(m?·r?)r? - m?)/r³ equations
- **Vector field superposition** from multiple magnets
- **Configurable magnet arrangements** via JSON configuration
- **Multiple magnet orientations** (North-South, East-West, diagonal)

### ?? Technical Features
- **JSON-configurable scenarios** for easy customization
- **Consolidated codebase** with minimal file overhead
- **Cross-platform compatibility** via Raylib
- **Professional antialiasing** with bilinear texture filtering

## ?? Quick Start

### Prerequisites
- **CMake 3.8+** (tested with 3.31.6)
- **C++20 compatible compiler** (MSVC, GCC, or Clang)
- **vcpkg** for dependency management
- **Git** for cloning

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/magnetic-field-simulator.git
   cd magnetic-field-simulator
   ```

2. **Install dependencies via vcpkg**
   ```bash
   vcpkg install raylib:x64-windows
   vcpkg install nlohmann-json:x64-windows
   ```

3. **Configure and build with CMake**
   ```bash
   cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ```

4. **Run the simulator**
   ```bash
   ./build/em2d_sfml/em2d.exe
   ```

## ?? Usage

### Basic Controls
- **Arrow Keys**: Adjust color sensitivity
  - `UP/DOWN`: Coarse adjustment (±0.05)
  - `LEFT/RIGHT`: Fine adjustment (±0.02)
- **R**: Reset color range to default
- **ESC**: Exit the application

### Exploring the Field
1. **Start with default settings** to see the overall field structure
2. **Increase sensitivity** (UP arrow) to see fine field details and weak regions
3. **Decrease sensitivity** (DOWN arrow) to focus on strong pole regions only
4. **Use fine controls** (LEFT/RIGHT) for precise field exploration

### Color Interpretation
- **Red regions**: Strong North pole magnetic fields
- **Blue regions**: Strong South pole magnetic fields  
- **Green regions**: Neutral or weak magnetic fields
- **Smooth gradients**: Show realistic field strength transitions

## ?? Configuration

### Magnet Configuration
Edit `em2d_sfml/assets/config.json` to customize the magnet arrangement:

```json
{
  "scenario": "custom_arrangement",
  "grid": { 
    "nx": 512, 
    "ny": 512, 
    "dx": 0.001, 
    "dy": 0.001 
  },
  "magnets": [
    {
      "name": "center_north",
      "x": 256,
      "y": 256,
      "moment_x": 0.0,
      "moment_y": 1.0,
      "strength": 2.0
    }
  ],
  "visualization": {
    "field": "B",
    "color_range": 1.8
  }
}
```

### Magnet Parameters
- **x, y**: Position in grid coordinates (0 to nx/ny)
- **moment_x, moment_y**: Magnetic moment vector components
- **strength**: Relative magnet strength (0.1 to 5.0)
- **name**: Descriptive identifier for debugging

### Common Configurations
- **North pole**: `"moment_y": 1.0, "moment_x": 0.0`
- **South pole**: `"moment_y": -1.0, "moment_x": 0.0`  
- **East pole**: `"moment_x": 1.0, "moment_y": 0.0`
- **Diagonal NE**: `"moment_x": 0.707, "moment_y": 0.707`

## ??? Project Structure

```
magnetic-field-simulator/
??? src/
?   ??? main.cpp              # Main application entry point
?   ??? FDTD.hpp/.cpp         # Magnetic field computation engine
?   ??? Renderer.hpp/.cpp     # Raylib-based visualization
?   ??? Config.hpp/.cpp       # JSON configuration system
?   ??? Source.hpp            # (Legacy - being consolidated)
??? em2d_sfml/
?   ??? assets/
?   ?   ??? config.json       # Magnet configuration file
?   ??? CMakeLists.txt        # Build configuration
??? README.md
??? .gitignore
??? CMakeLists.txt            # Root build configuration
```

## ?? Physics Background

This simulator implements realistic magnetic dipole field equations based on fundamental electromagnetic theory:

### Magnetic Dipole Field Formula
```
B?(r?) = (??/4?) × (3(m?·r?)r? - m?)/r³
```

Where:
- **B?**: Magnetic field vector
- **m?**: Magnetic dipole moment vector  
- **r?**: Position vector from dipole to field point
- **r?**: Unit vector in r? direction
- **??**: Permeability of free space

### Field Superposition
Multiple magnets create complex field patterns through vector addition of individual dipole contributions.

## ?? Educational Use

This simulator is perfect for:
- **Physics education** - Visualizing magnetic field concepts
- **Engineering coursework** - Understanding electromagnetic field behavior  
- **Research** - Quick magnetic field pattern analysis
- **Presentation** - Professional-quality field visualizations

## ??? Development

### Building from Source
```bash
# Debug build
cmake -S . -B build-debug -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug

# Release build
cmake -S . -B build-release -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

### Dependencies
- **Raylib**: Graphics and windowing
- **nlohmann/json**: JSON configuration parsing
- **C++20 STL**: Modern C++ features

### Code Style
- **C++20** modern features encouraged
- **const correctness** enforced
- **Clear variable naming** with descriptive identifiers
- **Minimal external dependencies**

## ?? Performance

- **Computation**: 512×512 = 262,144 field points calculated per frame
- **Rendering**: 60 FPS with bilinear antialiasing
- **Memory**: ~4MB for field storage + textures
- **Startup**: < 1 second initialization on modern hardware

## ?? Contributing

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. **Commit** your changes (`git commit -m 'Add amazing feature'`)
4. **Push** to the branch (`git push origin feature/amazing-feature`)
5. **Open** a Pull Request

### Contribution Ideas
- Additional magnet arrangement presets
- Field line visualization
- 3D magnetic field extension
- Export functionality for field data
- Animation/time-varying fields

## ?? License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ?? Acknowledgments

- **FEMM** (David Meeker) - Inspiration for field visualization style
- **Raylib** (Ramon Santamaria) - Excellent graphics framework
- **nlohmann/json** - Simple JSON parsing
- **Electromagnetic theory** - Maxwell's equations and magnetic dipole physics

## ?? Contact

- **GitHub Issues**: For bug reports and feature requests
- **Discussions**: For general questions and ideas

---

**Visualize the invisible world of magnetic fields with professional-quality rendering!** ???