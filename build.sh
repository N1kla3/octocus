#!/usr/bin/env bash
cmake . --preset=LinuxDebug&& cmake --build ./LinuxDebug  --target=EntityTest && cd LinuxDebug && ./EntityTest
