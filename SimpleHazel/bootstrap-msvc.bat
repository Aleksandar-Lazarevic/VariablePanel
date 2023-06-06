if not exist vcpkg\ (
git clone https://github.com/Microsoft/vcpkg.git
CALL .\vcpkg\bootstrap-vcpkg.bat
)

cmake -B msvc -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake 
cmake --build msvc
pause