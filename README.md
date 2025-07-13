
# DOS VESA Video Modes COM Dumper Tool

This tool runs in real mode DOS and produces two files containing VESA video modes data:
- A readable .TXT file for debugging purposes
- A binary .BIN file that contains detailed information about all available video modes

# How to use

1. Copy the *bin/VESAINFO.COM* file from the bin folder into your application's bin directory.

2. In your application, run the tool using:

int result = system("VESAINFO.COM");

3. If the result is 0, the tool ran successfully and will create a file named **VESAINFO.BIN**

# Format

**VESAINFO.BIN** is binary file with the following structure:

- **MainHeader**
- **VESAMainInfo**
- Multiple:
    - **ModeHeader**
    - **VESAModeInfo**
- Ending with **ModeHeader** where mode set to 0

The structure definitions can be found in this repository at: *src/headers.h*

# Notes on Data

**VESAMainInfo** contains general VESA data including the VESA version. Version of 0x0200 or higher is required for linear framebuffer support.

Each **ModeHeader** contains the video mode number (e.g., 0x101, 0x4101, etc.)

Each **VESAModeInfo** contains parameters such as resolution, color depth, mode attributes, and memory layout.

# Why it's needed

Calling BIOS interrupts (like VESA functions) from protected mode can be unstable or unsupported on some systems — especially under Windows 98 or certain DOS extenders.

This tool queries video modes information in real mode. It provides much higher compatability.

# Tested systems

- DosBox 0.74
- Lenovo T42 Windows 98 SE
- Lenovo T42 Dos 7

