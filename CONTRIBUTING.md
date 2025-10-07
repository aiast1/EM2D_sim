# Contributing to Magnetic Field Simulator

Thank you for your interest in contributing to the Magnetic Field Simulator! This document provides guidelines for contributing to the project.

## ?? Getting Started

### Prerequisites
- C++20 compatible compiler (MSVC 2022, GCC 10+, or Clang 12+)
- CMake 3.8 or higher
- vcpkg for dependency management
- Git

### Setting up the Development Environment

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/magnetic-field-simulator.git
   cd magnetic-field-simulator
   ```

2. **Install dependencies**
   ```bash
   vcpkg install raylib:x64-windows nlohmann-json:x64-windows
   ```

3. **Build the project**
   ```bash
   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ```

## ??? Development Guidelines

### Code Style
- **Modern C++20**: Use modern C++ features where appropriate
- **Const correctness**: Mark variables and methods const when possible
- **Clear naming**: Use descriptive variable and function names
- **Minimal dependencies**: Avoid adding unnecessary external libraries

### Project Structure
```
src/
??? main.cpp           # Application entry point
??? FDTD.hpp/.cpp     # Magnetic field computation
??? Renderer.hpp/.cpp # Raylib-based visualization
??? Config.hpp/.cpp   # JSON configuration system
??? Source.hpp        # (Being consolidated)
```

### Commit Message Format
```
type(scope): brief description

Detailed explanation if needed

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit first line to 72 characters
- Reference issues and pull requests liberally
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

## ?? Contribution Ideas

### Easy (Good First Issues)
- Add new predefined magnet configurations
- Improve documentation and code comments
- Add keyboard shortcuts for common operations
- Create additional example configurations

### Medium
- Implement field line visualization
- Add export functionality for field data
- Create preset scenarios (motor, transformer, etc.)
- Improve color scheme options

### Advanced
- 3D magnetic field visualization
- Time-varying/animated magnetic fields
- Advanced materials with non-linear properties
- Performance optimizations for larger grids

## ?? Testing

### Manual Testing
1. Build the project in Debug mode
2. Run with different configurations
3. Test interactive controls
4. Verify field patterns match expected physics

### Adding Test Cases
- Create new JSON configurations in `test_configs/`
- Document expected behavior
- Test edge cases (extreme values, empty configs)

## ?? Documentation

### Code Documentation
- Use clear, descriptive comments
- Document complex physics calculations
- Explain non-obvious design decisions

### README Updates
- Keep installation instructions current
- Update feature lists
- Add screenshots of new functionality

## ?? Bug Reports

### Before Submitting
1. Search existing issues
2. Try with the latest code
3. Minimal reproduction case

### Bug Report Template
```markdown
**Describe the bug**
A clear description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. See error

**Expected behavior**
What you expected to happen.

**Screenshots**
If applicable, add screenshots.

**Environment:**
- OS: [e.g. Windows 10]
- Compiler: [e.g. MSVC 2022]
- CMake version: [e.g. 3.21]
```

## ? Feature Requests

### Feature Request Template
```markdown
**Is your feature request related to a problem?**
A clear description of the problem.

**Describe the solution you'd like**
A clear description of what you want to happen.

**Describe alternatives you've considered**
Any alternative solutions or features.

**Additional context**
Screenshots, mockups, or other context.
```

## ?? Pull Request Process

1. **Fork** the repository
2. **Create** a feature branch from `main`
3. **Make** your changes
4. **Test** thoroughly
5. **Update** documentation
6. **Submit** pull request

### Pull Request Checklist
- [ ] Code follows project style guidelines
- [ ] Changes are tested and working
- [ ] Documentation is updated
- [ ] Commit messages are clear
- [ ] No merge conflicts with main branch

## ?? Getting Help

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For questions and ideas
- **Code Review**: All contributions are reviewed

## ?? License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to the Magnetic Field Simulator! ???