#!/usr/bin/env bash
cmake . --preset LinuxDebug && cmake --build ./LinuxDebug && cd LinuxDebug && ./EntityTest
