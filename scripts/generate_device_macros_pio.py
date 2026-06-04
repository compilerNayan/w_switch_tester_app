#!/usr/bin/env python3
"""
PlatformIO pre-build script wrapper
Calls the main generate_device_macros.py script and adds flags to PlatformIO build_flags
"""

import subprocess
import sys
from pathlib import Path

Import("env")

# Get the project root directory
project_dir = env.get("PROJECT_DIR")
script_path = Path(project_dir) / 'scripts' / 'generate_device_macros.py'

# Call the main script and get output
try:
    result = subprocess.run(
        [sys.executable, str(script_path), 'pio'],
        cwd=project_dir,
        capture_output=True,
        text=True,
        check=False
    )
    
    if result.returncode == 0 and result.stdout.strip():
        # Parse newline-separated flags
        flags = [line.strip() for line in result.stdout.strip().split('\n') if line.strip()]
        if flags:
            env.Append(BUILD_FLAGS=flags)
            print(f"Added {len(flags)} device macro definitions from device_config.ini")
        else:
            print("No device macros generated (device_config.ini not found or empty)")
    else:
        if result.stderr:
            print(f"Warning: {result.stderr.strip()}")
except Exception as e:
    print(f"Error running generate_device_macros.py: {e}")
