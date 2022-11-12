This repos contains example Actions for [SonarCloud](https://www.sonarsource.com/products/sonarcloud/)
source code scan of a simple C program, using CMake.  
The program itself is not important. It is a very simple example of using shared memory,
implemented using POSIX and Win32 API.  
Actions examples are implemented for Linux, MacOS and Windows.

Actions in this repo are currently disabled and the project in SonarCloud doesn't exist (is deleted).

**Note to myself:** If you want to run the scan, set up the project in SonarCloud  
and **update file `sonar-project.properties`!**  
The SonarCloud application is uninstalled and revoked from this repo -> you will need to log in to SonarCloud,
set up an organization, and select this repo for as a project to scan.

If you need to build the source for testing, run following commands for Unix/Windows systems:

Unix:
```shell
cmake -S . -B build
cd build
make
```

Windows:
```powershell
cmake -G "Visual Studio 17 2022" -A x64 -S . -B build
cmake --build build --config Release
```
Please note the argument of `-G` and `-A` parameters might be different, depending on your compiler and OS architecture.
