#ifndef VESA_H
#define VESA_H
#include "setup.h"
#include "utils.h"
#include "headers.h"

typedef struct
{
    char VESASignature[4]; // 'VESA' signature
    unsigned short VESAVersion;
    unsigned long OEMStringPtr;
    unsigned char Capabilities[4];
    unsigned long VideoModePtr;
    unsigned short TotalMemory;
    unsigned short OemSoftwareRev;
    unsigned long OemVendorNamePtr;
    unsigned long OemProductNamePtr;
    unsigned long OemProductRevPtr;
    char Reserved[222];
    char OemData[256];
} VesaVbeInfoBlock;

typedef struct VbeModeInfoBlock
{
    unsigned short ModeAttributes;   // Mode attributes
    unsigned char WinAAttributes;    // Window A attributes
    unsigned char WinBAttributes;    // Window B attributes
    unsigned short WinGranularity;   // Window granularity in KB
    unsigned short WinSize;          // Window size in KB
    unsigned short WinASegment;      // Window A start segment
    unsigned short WinBSegment;      // Window B start segment
    unsigned long WinFuncPtr;        // Real mode pointer to window function
    unsigned short BytesPerScanLine; // Bytes per scan line

    unsigned short XResolution;       // Horizontal resolution in pixels or chars
    unsigned short YResolution;       // Vertical resolution in pixels or chars
    unsigned char XCharSize;          // Character cell width in pixels
    unsigned char YCharSize;          // Character cell height in pixels
    unsigned char NumberOfPlanes;     // Number of memory planes
    unsigned char BitsPerPixel;       // Bits per pixel
    unsigned char NumberOfBanks;      // Number of banks
    unsigned char MemoryModel;        // Memory model type
    unsigned char BankSize;           // Bank size in KB
    unsigned char NumberOfImagePages; // Number of images
    unsigned char Reserved1;          // Reserved for page function

    unsigned char RedMaskSize;         // Size of direct color red mask in bits
    unsigned char RedFieldPosition;    // Bit position of lsb of red mask
    unsigned char GreenMaskSize;       // Size of direct color green mask in bits
    unsigned char GreenFieldPosition;  // Bit position of lsb of green mask
    unsigned char BlueMaskSize;        // Size of direct color blue mask in bits
    unsigned char BlueFieldPosition;   // Bit position of lsb of blue mask
    unsigned char RsvdMaskSize;        // Size of direct color reserved mask in bits
    unsigned char RsvdFieldPosition;   // Bit position of lsb of reserved mask
    unsigned char DirectColorModeInfo; // Direct color mode attributes

    unsigned long PhysBasePtr;        // Physical address for linear framebuffer
    unsigned long OffScreenMemOffset; // Pointer to start of offscreen memory
    unsigned short OffScreenMemSize;  // Size of offscreen memory in KB

    unsigned char Reserved2[206]; // Reserved, must be zero
} VesaVbeModeInfoBlock;

extern VesaVbeInfoBlock far *vesaVbeInfo;
extern VesaVbeModeInfoBlock far *vesaVbeModeInfo;
extern int vbeModeInfoBlock;

void vesaUpdateVBEInfo();
void vesaUpdateVBEModeInfo(int mode);
int vesaGetVesaVersion();
int vesaIsLinearBufferAvailable(int mode);
unsigned char *vesaGetLinearBufferAdress(int mode);

#endif
