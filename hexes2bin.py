# objcopy --input-target=ihex --output-target=binary
"""
Goes into every subdirectory of built/ and converts all hex files with "combined" in the name to binary files
called firmware.bin.
"""

import os
import subprocess
import sys

def convert_hex_to_bin(hex_file):
    """
    Converts a hex file to a binary file using objcopy.
    """
    # Get the name of the hex file without the extension
    base_name = os.path.splitext(hex_file)[0]
    # Create the name of the binary file "firmware.bin"
    bin_file = os.path.join(os.path.dirname(hex_file), "firmware.bin")
    # Run objcopy to convert the hex file to a binary file
    subprocess.run(["objcopy", "--input-target=ihex", "--output-target=binary", hex_file, bin_file], check=True)

    hexname = os.path.basename(hex_file)
    print(f"Converted {hexname} -> {bin_file}")

def main():
    # Get the current working directory
    cwd = os.getcwd()
    # Get the path to the built directory
    built_dir = os.path.join(cwd, "built")
    # Loop through all subdirectories of the built directory
    for root, dirs, files in os.walk(built_dir):
        for file in files:
            if file.endswith(".hex") and file.startswith("combined-"):
                # Get the full path to the hex file
                hex_file = os.path.join(root, file)
                # Change to relative path
                rel_path = os.path.relpath(hex_file, cwd)
                # Convert the hex file to a binary file
                convert_hex_to_bin(rel_path)

if __name__ == "__main__":
    # Check if objcopy is installed
    try:
        subprocess.run(["objcopy", "--version"], check=True)
    except FileNotFoundError:
        print("objcopy not found. Please install it and try again.")
        sys.exit(1)
    
    main()
