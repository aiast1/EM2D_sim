# Getting Started with Magnetic Field Simulator

Welcome to the Magnetic Field Simulator! This guide will help you get up and running quickly.

## ?? Quick Setup (5 minutes)

### 1. Prerequisites Check
Ensure you have:
- **Windows 10/11** (or Linux/macOS - see README for details)
- **Git** installed
- **vcpkg** package manager

### 2. Clone and Build
```bash
# Clone the repository
git clone https://github.com/yourusername/magnetic-field-simulator.git
cd magnetic-field-simulator

# Install dependencies (one-time setup)
vcpkg install raylib:x64-windows nlohmann-json:x64-windows

# Build the project
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run the simulator
./build/em2d_sfml/em2d.exe
```

### 3. First Run
When you run the simulator, you should see:
- A 1200×900 window with the title "High-Res Magnetic Field Simulator"
- A colorful magnetic field pattern with 9 organized magnets
- Interactive controls displayed at the bottom

## ?? Basic Controls

### Essential Controls
- **UP Arrow**: Increase color sensitivity (see more field detail)
- **DOWN Arrow**: Decrease color sensitivity (focus on strong fields)
- **LEFT/RIGHT Arrows**: Fine-tune sensitivity
- **R Key**: Reset to default settings
- **ESC**: Exit the program

### What the Colors Mean
- **?? Red**: Strong North magnetic field
- **?? Yellow**: Medium North field  
- **?? Green**: Neutral/weak field
- **?? Cyan**: Medium South field
- **?? Blue**: Strong South magnetic field

## ?? Understanding the Visualization

### Default Configuration
The simulator starts with 9 magnets arranged in a symmetric pattern:
- **Center magnet**: Strong North pole (red core)
- **Side magnets**: South poles creating field lines
- **Corner magnets**: Diagonal orientations for complex patterns

### Field Exploration Tips
1. **Start with default settings** - See the overall field structure
2. **Increase sensitivity** (UP key) - Reveal weak field regions between magnets
3. **Decrease sensitivity** (DOWN key) - Focus on strong pole regions only
4. **Use fine controls** (LEFT/RIGHT) - Find the perfect sensitivity for your analysis

## ?? Customization

### Modifying Magnet Arrangements
Edit `em2d_sfml/assets/config.json` to change the magnet configuration:

```json
{
  "magnets": [
    {
      "name": "my_magnet",
      "x": 256,        // Position (0-512)
      "y": 256,        // Position (0-512) 
      "moment_x": 0.0, // East-West orientation
      "moment_y": 1.0, // North-South orientation (1.0 = North)
      "strength": 2.0  // Magnet strength
    }
  ]
}
```

### Common Magnet Orientations
- **North pole**: `"moment_y": 1.0, "moment_x": 0.0`
- **South pole**: `"moment_y": -1.0, "moment_x": 0.0`
- **East pole**: `"moment_x": 1.0, "moment_y": 0.0`  
- **Northeast**: `"moment_x": 0.707, "moment_y": 0.707`

## ?? Common Use Cases

### Educational Physics
- **Demonstrate magnetic field concepts** to students
- **Show field superposition** from multiple magnets
- **Visualize pole interactions** (attraction/repulsion)

### Engineering Analysis
- **Quick field pattern analysis** for magnetic devices
- **Motor/generator field visualization** 
- **Magnetic shielding studies**

### Research & Development
- **Rapid prototyping** of magnet arrangements
- **Field strength optimization**
- **Professional presentations** with high-quality visuals

## ?? Troubleshooting

### Common Issues

**Black screen or no visualization:**
- Check that config.json exists in `em2d_sfml/assets/`
- Verify graphics drivers are updated
- Try running from the correct directory

**Build errors:**
- Ensure vcpkg dependencies are installed correctly
- Check CMake version (3.8+ required)
- Verify C++20 compiler support

**Performance issues:**
- Reduce grid resolution in config.json (nx/ny values)
- Close other graphics-intensive applications
- Try Release build instead of Debug

### Getting Help
- Check the full **README.md** for detailed documentation
- Look at **CONTRIBUTING.md** for development info
- Open a GitHub issue for bugs or feature requests

## ?? Next Steps

### Learn More
1. **Read the full README** - Comprehensive feature documentation
2. **Explore configurations** - Try different magnet arrangements
3. **Study the physics** - Learn about magnetic dipole equations
4. **Contribute** - Add features or improve the simulator

### Advanced Features
- **JSON configuration system** - Create custom scenarios
- **Interactive controls** - Real-time field exploration  
- **Professional visualization** - FEMM-quality rendering
- **Physics accuracy** - Proper electromagnetic equations

---

**Enjoy exploring the invisible world of magnetic fields!** ???

For more details, see the complete documentation in **README.md**.