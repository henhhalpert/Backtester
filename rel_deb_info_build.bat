@echo off
echo [1/2] Configuring with CMake (RelWithDebInfo mode)...
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_PREFIX_PATH=C:/vcpkg/installed/x64-windows -DCMAKE_BUILD_TYPE=RelWithDebInfo

if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

echo [2/2] Building the project in RelWithDebInfo mode...
cmake --build build --config RelWithDebInfo -- /p:VcpkgEnableManifest=true

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo RelWithDebInfo build completed successfully.
