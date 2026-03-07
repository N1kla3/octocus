#!/usr/bin/env bash
cmake . --preset=LinuxDebug && cmake --build ./LinuxDebug  --target=octocus_server && cd LinuxDebug && cp compile_commands.json ../ && ./octocus_server
