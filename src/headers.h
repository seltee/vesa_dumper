#ifndef HEADERS_H
#define HEADERS_H

typedef struct MainHeader
{
    char signature[4];
    unsigned long version;
    unsigned long sizeOfMainBlock;
    unsigned long sizeOfModeBlock;
} MainHeader;

typedef struct VesaMainInfo
{
    unsigned short VESAVersion; // 0x200 for Vesa 2.0
    unsigned short totalMemory;
    unsigned char capabilities[4];
} VesaMainInfo;

typedef struct ModeHeader
{
    unsigned long mode;
} ModeHeader;

typedef struct VesaModeInfo
{
    unsigned short modeAttributes; // Mode attributes
    unsigned char winAAttributes;  // Window A attributes
    unsigned char winBAttributes;  // Window B attributes

    unsigned short winGranularity; // Window granularity in KB
    unsigned short winSize;        // Window size in KB

    unsigned short winASegment; // Window A start segment
    unsigned short winBSegment; // Window B start segment

    unsigned short bytesPerScanLine; // Bytes per scan line
    unsigned short xResolution;      // Horizontal resolution in pixels or chars

    unsigned short yResolution; // Vertical resolution in pixels or chars
    unsigned char xCharSize;    // Character cell width in pixels
    unsigned char yCharSize;    // Character cell height in pixels

    unsigned char numberOfPlanes; // Number of memory planes
    unsigned char bitsPerPixel;   // Bits per pixel
    unsigned char numberOfBanks;  // Number of banks
    unsigned char memoryModel;    // Memory model type

    unsigned char bankSize;           // Bank size in KB
    unsigned char numberOfImagePages; // Number of images
    unsigned char redMaskSize;        // Size of direct color red mask in bits
    unsigned char redFieldPosition;   // Bit position of lsb of red mask

    unsigned char greenMaskSize;      // Size of direct color green mask in bits
    unsigned char greenFieldPosition; // Bit position of lsb of green mask
    unsigned char blueMaskSize;       // Size of direct color blue mask in bits
    unsigned char blueFieldPosition;  // Bit position of lsb of blue mask

    unsigned char rsvdMaskSize;        // Size of direct color reserved mask in bits
    unsigned char rsvdFieldPosition;   // Bit position of lsb of reserved mask
    unsigned char directColorModeInfo; // Direct color mode attributes
    unsigned char reserved1;

    unsigned long physBasePtr; // Physical address for linear framebuffer

    unsigned long offScreenMemOffset; // Pointer to start of offscreen memory

    unsigned short offScreenMemSize; // Size of offscreen memory in KB
    unsigned short usingPalette;     // Is using palette
} VesaModeInfo;

#endif
