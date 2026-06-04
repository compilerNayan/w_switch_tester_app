#!/usr/bin/env python3
"""
Generate C++ preprocessor macros from device_config.ini
Reads the INI file and outputs compiler flags (-D macros)
Currently generates SWITCH_N_* macros, but can be extended for other device types
"""

import configparser
import sys
import os
from pathlib import Path

def parse_device_config(ini_path):
    """Parse device_config.ini and return a list of (device_id, relay_pin, physical_pin) tuples
    Currently parses Switch_N sections, but can be extended for other device types
    """
    config = configparser.ConfigParser()
    
    if not os.path.exists(ini_path):
        print(f"Warning: {ini_path} not found. No device macros will be generated.", file=sys.stderr)
        return []
    
    config.read(ini_path)
    devices = []
    
    # Parse Switch sections (can be extended for other device types like Regulator, etc.)
    for section_name in config.sections():
        if not section_name.startswith('Switch_'):
            continue
        
        try:
            device_id = int(section_name.replace('Switch_', ''))
            relay_pin = config.getint(section_name, 'relay_pin')
            physical_pin = config.getint(section_name, 'physical_pin')
            devices.append(('switch', device_id, relay_pin, physical_pin))
        except (ValueError, configparser.NoOptionError) as e:
            print(f"Warning: Skipping invalid section {section_name}: {e}", file=sys.stderr)
            continue
    
    return sorted(devices, key=lambda x: x[1])  # Sort by device_id

def generate_macro_flags(devices, format='cmake'):
    """Generate compiler flags (-D macros) from device configurations
    
    Args:
        devices: List of (device_type, device_id, relay_pin, physical_pin) tuples
        format: 'cmake' for CMake (space-separated), 'pio' for PlatformIO (newline-separated)
    """
    flags = []
    for device_type, device_id, relay_pin, physical_pin in devices:
        if device_type == 'switch':
            # Generate SWITCH_N_* macros
            if format == 'cmake':
                flags.append(f"-DSWITCH_{device_id}_RELAY_PIN_INDEX={relay_pin}")
                flags.append(f"-DSWITCH_{device_id}_PHYSICAL_DEVICE_PIN_INDEX={physical_pin}")
            else:
                flags.append(f"-DSWITCH_{device_id}_RELAY_PIN_INDEX={relay_pin}")
                flags.append(f"-DSWITCH_{device_id}_PHYSICAL_DEVICE_PIN_INDEX={physical_pin}")
        # Future: Add other device types here (e.g., regulator, etc.)
    
    return flags

def main():
    import sys
    
    # Get the project root directory (where device_config.ini is located)
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    ini_path = project_root / 'device_config.ini'
    
    # Determine format from command line argument or default to 'cmake'
    format_type = sys.argv[1] if len(sys.argv) > 1 else 'cmake'
    
    devices = parse_device_config(ini_path)
    flags = generate_macro_flags(devices, format=format_type)
    
    if format_type == 'pio':
        # PlatformIO format: newline-separated
        print('\n'.join(flags))
    else:
        # CMake format: space-separated (for shell consumption)
        print(' '.join(flags))

if __name__ == '__main__':
    main()
