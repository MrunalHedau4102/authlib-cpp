# Publishing to Conan

Conan is a popular C++ package manager. This guide explains how to publish AuthLib to Conan.

## Prerequisites

1. Create a Conan account: https://conan.io
2. Install Conan: `pip install conan`
3. Verify installation: `conan --version`

## Setup Conan Profile

```bash
# Create default profile
conan profile detect --force

# View your profile
cat ~/.conan2/profiles/default
```

## Create conanfile.py

A `conanfile.py` is already in the root directory. It defines:
- Package name, version, author
- Dependencies (OpenSSL, nlohmann_json, jwt-cpp)
- Build instructions
- Export patterns

## Steps to Publish

### 1. Build and Test Locally

```bash
# Create a build
conan create . --build=missing

# Run tests
conan create . --build=missing -tf ""
```

### 2. Prepare for Upload

Update version in `conanfile.py`:
```python
name = "authlib"
version = "1.0.0"
```

### 3. Add Conan Remote (ConantCenter)

```bash
# Add ConanCenter
conan remote add conancenter https://center.conan.io --index=0

# Verify
conan remote list
```

### 4. Create Conan Package

```bash
# Export the recipe
conan export . authlib/1.0.0@

# Create package (required before upload)
conan create . authlib/1.0.0@
```

### 5. Upload to Conan Repository

For **ConanCenter** (official):
- Submit a pull request: https://github.com/conan-io/conan-center-index
- Include `conanfile.py` and test package

For **your own Conan Server**:
```bash
# Authenticate (if required)
conan remote login conancenter <username>

# Upload
conan upload authlib/1.0.0@ --all --remote=conancenter
```

## Using the Published Package

Once published, users can use it in their `conanfile.txt`:

```
[requires]
authlib/1.0.0

[generators]
CMakeDeps
CMakeToolchain
```

Or in `conanfile.py`:
```python
def requirements(self):
    self.requires("authlib/1.0.0")
```

## Verify Publication

```bash
# Search on ConanCenter
conan search authlib/1.0.0@

# Install
conan install authlib/1.0.0@
```

## Troubleshooting

- **Build failure**: Run `conan create . --build=missing -vv` for verbose output
- **Dependency issues**: Ensure all dependencies are available in ConanCenter
- **Upload errors**: Check authentication with `conan remote login`
- **Version exists**: Increment version number for new releases

## Best Practices

- Keep `conanfile.py` in sync with CMakeLists.txt
- Pin dependency versions appropriately
- Include C++ standard requirements
- Provide comprehensive test_package
- Document platform-specific requirements
