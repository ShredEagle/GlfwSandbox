# Glfw Sandbox

Prototype and investigate GLFW usage.

## Usage

### Prerequisites

* Conan (`pip3 install conan`)

### Build

    git clone --recurse https://github.com/ShredEagle/glfwsandbox.git
    cd glfwsandbox
    mkdir build && cd build
    conan install --build=missing ../conan/
    conan build -sf .. ../conan/

### Run

    ./src/apps/gsync_windowed[/Debug]/gsync_windowed.exe

Or replace `gsync_windowed` by another application.
