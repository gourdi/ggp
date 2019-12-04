$SrcDir = (get-item $PSScriptRoot).FullName
$RootDir = (get-item $PSScriptRoot).parent.FullName

# vcpkg
$VcpkgDir = Join-Path -Path $RootDir -ChildPath "vcpkg"
Remove-Item -Recurse -Force $VcpkgDir

git clone https://github.com/Microsoft/vcpkg.git $VcpkgDir

$Bootstrap = Join-Path -Path $VcpkgDir -ChildPath "bootstrap-vcpkg.bat"
& $Bootstrap

$VcpkgExe = Join-Path -Path $VcpkgDir -ChildPath "vcpkg.exe"
& $VcpkgExe install sdl2 libjpeg-turbo libpng sdl2 glfw3 glad --triplet x64-windows

$VcpkgToolchainPath = Join-Path -Path $VcpkgDir -ChildPath "\scripts\buildsystems\vcpkg.cmake"

# Generate the project.
$BuildDir = Join-Path -Path $RootDir -ChildPath "build"
Remove-Item -Recurse -Force $BuildDir
New-Item -ItemType Directory -Path $BuildDir

$ToolchainArg = '"-DCMAKE_TOOLCHAIN_FILE=' + $VcpkgToolchainPath + '"'
cmake -S $SrcDir -B $BuildDir -G  "Visual Studio 16 2019" -A x64 $ToolchainArg