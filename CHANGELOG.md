# Changelog

All notable changes to the Magnetic Field Simulator project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial release of Magnetic Field Simulator
- High-resolution 512×512 magnetic field computation
- Interactive color range adjustment with arrow keys
- JSON-configurable magnet arrangements
- Professional FEMM-style color mapping
- Bilinear antialiasing for smooth visualization
- Real-time field sensitivity controls

### Features
- **Realistic Physics**: Magnetic dipole field equations with proper 1/r³ decay
- **Interactive Controls**: UP/DOWN for coarse adjustment, LEFT/RIGHT for fine tuning
- **Professional Visualization**: 7-zone color gradient from blue (South) to red (North)
- **High Performance**: 60 FPS rendering with efficient field computation
- **Configurable**: JSON-based magnet configuration system
- **Cross-Platform**: Built with Raylib for Windows, Linux, and macOS support

### Technical Details
- C++20 modern codebase with consolidated architecture
- CMake build system with vcpkg dependency management
- Raylib graphics framework with texture filtering
- nlohmann/json for configuration parsing
- Professional antialiasing with bilinear texture filtering

## [1.0.0] - 2025-01-XX

### Added
- Initial stable release
- Complete magnetic field visualization system
- Documentation and examples
- MIT license for open source distribution

---

## Version History

### Development Milestones

**v0.1.0 - Initial Prototype**
- Basic magnetic field computation
- Simple SFML rendering (later replaced with Raylib)
- Hardcoded magnet positions

**v0.2.0 - Interactive Features**  
- Added keyboard controls for color adjustment
- Implemented JSON configuration system
- Improved field computation accuracy

**v0.3.0 - Professional Visualization**
- Switched to Raylib for better graphics
- Added antialiasing and texture filtering
- Implemented FEMM-style color mapping
- Consolidated codebase architecture

**v0.4.0 - Polish and Documentation**
- Added comprehensive README
- Created robust .gitignore
- Implemented proper project structure
- Added contribution guidelines

**v1.0.0 - Stable Release**
- Feature-complete magnetic field simulator
- Professional-quality visualization
- Complete documentation
- Ready for GitHub publication

---

## Future Roadmap

### Planned Features
- **v1.1.0**: Field line visualization
- **v1.2.0**: Export functionality for field data
- **v1.3.0**: Additional magnet arrangement presets
- **v2.0.0**: 3D magnetic field visualization
- **v2.1.0**: Time-varying magnetic fields
- **v2.2.0**: Advanced materials simulation

### Long-term Vision
- Complete FEMM alternative for educational use
- Advanced electromagnetic simulation capabilities
- Integration with physics education platforms
- Mobile/web versions for broader accessibility

---

## Contributors

- **Initial Development**: Core magnetic field simulator implementation
- **Physics Accuracy**: Proper dipole field equations and superposition
- **Visualization**: Professional FEMM-style rendering system
- **Architecture**: Modern C++20 codebase design

Thank you to all contributors who have helped make this project possible! ???