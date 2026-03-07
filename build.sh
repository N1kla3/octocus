#!/usr/bin/env bash
cmake . --preset=LinuxDebug && cmake --build ./LinuxDebug  --target=octocus && cd LinuxDebug && cp compile_commands.json ../ && ./octocus
