#include "hardware/vesa.h"
#include "utils.h"
#include "setup.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

char buffer[1024];
ModeHeader modeHeader;
VesaMainInfo vesaMainInfo;
MainHeader mainHeader;
VesaModeInfo vesaModeInfo;

void writeModeInfo(FILE *fileTXT, FILE *fileBIN, int mode)
{
    vesaUpdateVBEModeInfo(mode);

    if (vesaVbeModeInfo)
    {
        sprintf(buffer, "=Mode %04xh info=\n", mode);
        fputs(buffer, fileTXT);
        if (vesaVbeModeInfo)
        {
            sprintf(buffer, "Linear Buffer %s\n", (vesaVbeModeInfo->ModeAttributes & 0x80) ? "available" : "disabled");
            fputs(buffer, fileTXT);
            sprintf(buffer, "Memory Model %i\n", vesaVbeModeInfo->MemoryModel);
            fputs(buffer, fileTXT);
            sprintf(buffer, "Bank Size %i\n", vesaVbeModeInfo->BankSize);
            fputs(buffer, fileTXT);
            sprintf(buffer, "Number of Banks %i\n", vesaVbeModeInfo->NumberOfBanks);
            fputs(buffer, fileTXT);
            sprintf(buffer, "X Resolution %i\n", vesaVbeModeInfo->XResolution);
            fputs(buffer, fileTXT);
            sprintf(buffer, "Y Resolution %i\n", vesaVbeModeInfo->YResolution);
            fputs(buffer, fileTXT);
            sprintf(buffer, "Bytes per Scanline %i\n", vesaVbeModeInfo->BytesPerScanLine);
            fputs(buffer, fileTXT);
            sprintf(buffer, "Bits per Pixel %i\n", vesaVbeModeInfo->BitsPerPixel);
            fputs(buffer, fileTXT);
            sprintf(buffer, "Using Palette %s\n", (mode & 0x10) ? "No" : "Yes");
            fputs(buffer, fileTXT);
            sprintf(buffer, "Off Screen Memory Size %i\n", vesaVbeModeInfo->OffScreenMemSize);
            fputs(buffer, fileTXT);
            sprintf(buffer, "Base Pointer %08lX\n", vesaVbeModeInfo->PhysBasePtr);
            fputs(buffer, fileTXT);
            sprintf(buffer, "\n", vesaVbeModeInfo->PhysBasePtr);
            fputs(buffer, fileTXT);

            modeHeader.mode = mode;
            fwrite(&modeHeader, sizeof(ModeHeader), 1, fileBIN);

            memset(&vesaModeInfo, 0, sizeof(VesaModeInfo));
            vesaModeInfo.modeAttributes = vesaVbeModeInfo->ModeAttributes;
            vesaModeInfo.winAAttributes = vesaVbeModeInfo->WinAAttributes;
            vesaModeInfo.winBAttributes = vesaVbeModeInfo->WinBAttributes;
            vesaModeInfo.winGranularity = vesaVbeModeInfo->WinGranularity;
            vesaModeInfo.winSize = vesaVbeModeInfo->WinSize;
            vesaModeInfo.winASegment = vesaVbeModeInfo->WinASegment;
            vesaModeInfo.winBSegment = vesaVbeModeInfo->WinBSegment;
            vesaModeInfo.bytesPerScanLine = vesaVbeModeInfo->BytesPerScanLine;
            vesaModeInfo.xResolution = vesaVbeModeInfo->XResolution;
            vesaModeInfo.yResolution = vesaVbeModeInfo->YResolution;
            vesaModeInfo.xCharSize = vesaVbeModeInfo->XCharSize;
            vesaModeInfo.yCharSize = vesaVbeModeInfo->YCharSize;
            vesaModeInfo.numberOfPlanes = vesaVbeModeInfo->NumberOfPlanes;
            vesaModeInfo.bitsPerPixel = vesaVbeModeInfo->BitsPerPixel;
            vesaModeInfo.numberOfBanks = vesaVbeModeInfo->NumberOfBanks;
            vesaModeInfo.memoryModel = vesaVbeModeInfo->MemoryModel;
            vesaModeInfo.bankSize = vesaVbeModeInfo->BankSize;
            vesaModeInfo.numberOfImagePages = vesaVbeModeInfo->NumberOfImagePages;
            vesaModeInfo.redMaskSize = vesaVbeModeInfo->RedMaskSize;
            vesaModeInfo.redFieldPosition = vesaVbeModeInfo->RedFieldPosition;
            vesaModeInfo.greenMaskSize = vesaVbeModeInfo->GreenMaskSize;
            vesaModeInfo.greenFieldPosition = vesaVbeModeInfo->GreenFieldPosition;
            vesaModeInfo.blueMaskSize = vesaVbeModeInfo->BlueMaskSize;
            vesaModeInfo.blueFieldPosition = vesaVbeModeInfo->BlueFieldPosition;
            vesaModeInfo.rsvdMaskSize = vesaVbeModeInfo->RsvdMaskSize;
            vesaModeInfo.rsvdFieldPosition = vesaVbeModeInfo->RsvdFieldPosition;
            vesaModeInfo.directColorModeInfo = vesaVbeModeInfo->DirectColorModeInfo;
            vesaModeInfo.physBasePtr = vesaVbeModeInfo->PhysBasePtr;
            vesaModeInfo.offScreenMemOffset = vesaVbeModeInfo->OffScreenMemOffset;
            vesaModeInfo.offScreenMemSize = vesaVbeModeInfo->OffScreenMemSize;
            vesaModeInfo.usingPalette = (mode & 0x10) ? 0 : 1;
            fwrite(&vesaModeInfo, sizeof(VesaModeInfo), 1, fileBIN);
        }
    }
}

void writeModeInfoFull(FILE *fileTXT, FILE *fileBIN, int mode)
{
    writeModeInfo(fileTXT, fileBIN, mode);
    writeModeInfo(fileTXT, fileBIN, mode | 0x4000);
}

int main(void)
{
    vesaUpdateVBEInfo();
    int major = vesaVbeInfo->VESAVersion >> 8;
    int minor = vesaVbeInfo->VESAVersion & 0xFF;

    if (!vesaVbeInfo)
    {
        return 1;
    }

    FILE *fileTXT = fopen("VESAINFO.TXT", "w");
    FILE *fileBIN = fopen("VESAINFO.BIN", "wb");

    // Header
    memcpy(mainHeader.signature, "VSIF", 4);
    mainHeader.version = 1;
    mainHeader.sizeOfMainBlock = sizeof(VesaMainInfo);
    mainHeader.sizeOfModeBlock = sizeof(VesaModeInfo);
    fwrite(&mainHeader, sizeof(MainHeader), 1, fileBIN);

    // Vesa main information
    vesaMainInfo.VESAVersion = (major << 8) | minor;
    vesaMainInfo.totalMemory = vesaVbeInfo->TotalMemory;
    memcpy(vesaMainInfo.capabilities, vesaVbeInfo->Capabilities, 4);
    fwrite(&vesaMainInfo, sizeof(VesaMainInfo), 1, fileBIN);

    sprintf(buffer, "=Main info=\n");
    fputs(buffer, fileTXT);
    sprintf(buffer, "Vesa Version %04xh\n", (major << 8) | minor);
    fputs(buffer, fileTXT);
    sprintf(buffer, "Total VRAM %i\n", vesaVbeInfo->TotalMemory);
    fputs(buffer, fileTXT);
    sprintf(buffer, "\n", vesaVbeModeInfo->PhysBasePtr);
    fputs(buffer, fileTXT);

    writeModeInfoFull(fileTXT, fileBIN, 0x100);
    writeModeInfoFull(fileTXT, fileBIN, 0x101);
    writeModeInfoFull(fileTXT, fileBIN, 0x103);
    writeModeInfoFull(fileTXT, fileBIN, 0x104);
    writeModeInfoFull(fileTXT, fileBIN, 0x105);
    writeModeInfoFull(fileTXT, fileBIN, 0x106);
    writeModeInfoFull(fileTXT, fileBIN, 0x107);
    writeModeInfoFull(fileTXT, fileBIN, 0x10D);
    writeModeInfoFull(fileTXT, fileBIN, 0x10E);
    writeModeInfoFull(fileTXT, fileBIN, 0x10F);
    writeModeInfoFull(fileTXT, fileBIN, 0x110);
    writeModeInfoFull(fileTXT, fileBIN, 0x111);
    writeModeInfoFull(fileTXT, fileBIN, 0x112);
    writeModeInfoFull(fileTXT, fileBIN, 0x113);
    writeModeInfoFull(fileTXT, fileBIN, 0x114);
    writeModeInfoFull(fileTXT, fileBIN, 0x115);
    writeModeInfoFull(fileTXT, fileBIN, 0x116);
    writeModeInfoFull(fileTXT, fileBIN, 0x117);
    writeModeInfoFull(fileTXT, fileBIN, 0x118);
    writeModeInfoFull(fileTXT, fileBIN, 0x119);
    writeModeInfoFull(fileTXT, fileBIN, 0x11A);
    writeModeInfoFull(fileTXT, fileBIN, 0x11B);

    modeHeader.mode = 0;
    fwrite(&modeHeader, sizeof(ModeHeader), 1, fileBIN);

    fclose(fileTXT);
    fclose(fileBIN);

    return 0;
}
