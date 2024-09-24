call vcvarsall.bat x86_amd64 && cmake . --preset DebugBuild && cmake --build .\DebugBuild && cd DebugBuild && call EntityTest.exe
