if not exist vcpkg\ (
git clone https://github.com/Microsoft/vcpkg.git
CALL .\vcpkg\bootstrap-vcpkg.bat
)

git submodule init
git submodule update

cmake -B msvc -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake 
cmake --build msvc


@echo [92m #######################################################[0m
@echo [92m If this command succeeded then you will have a new[0m
@echo [92m project inside msvc/EditorCore.sln[0m
@echo [92m #######################################################[0m

pause