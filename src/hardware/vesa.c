#include "vesa.h"
#include <string.h>
#include <stdio.h>

VESAVbeInfoBlock far *vesaVbeInfo = 0;
VESAVbeModeInfoBlock far *vesaVbeModeInfo = 0;
unsigned int vesaModeSegment = 0;
int vbeModeInfoBlock = 0;
union REGS regs;
struct SREGS sregs;

void vesaUpdateVBEInfo()
{
    if (vesaVbeInfo == 0)
    {
        memset(&regs, 0, sizeof(union REGS));
        memset(&sregs, 0, sizeof(struct SREGS));

        unsigned int segment;
        dosMemAlloc(sizeof(VESAVbeInfoBlock), &segment);
        vesaVbeInfo = (VESAVbeInfoBlock far *)MK_FP(segment, 0);

        if (!vesaVbeInfo)
        {
            printf("Failed to allocate real-mode memory\n");
            return;
        }

        // Initialize block
        _fmemset(vesaVbeInfo, 0, sizeof(VESAVbeInfoBlock));
        _fmemcpy(vesaVbeInfo->VESASignature, "VBE2", 4);

        regs.w.ax = 0x4F00;
        regs.w.di = 0;
        sregs.es = segment;

        int86x(0x10, &regs, &regs, &sregs);

        if (regs.w.ax != 0x004F)
        {
            printf("VESA called failed %i\n", regs.w.ax);
            dosMemFree(segment);
            vesaVbeInfo = 0;
            return;
        }
    }
}

void vesaUpdateVBEModeInfo(int mode)
{
    if (vbeModeInfoBlock != mode)
    {
        if (vesaVbeModeInfo)
        {
            dosMemFree(vesaModeSegment);
            vesaVbeModeInfo = 0;
            vesaModeSegment = 0;
        }

        dosMemAlloc(sizeof(VESAVbeModeInfoBlock), &vesaModeSegment);
        vesaVbeModeInfo = (VESAVbeModeInfoBlock far *)MK_FP(vesaModeSegment, 0);

        if (!vesaVbeModeInfo)
        {
            printf("Failed to allocate real-mode memory\n");
            return;
        }

        // Initialize block
        _fmemset(vesaVbeModeInfo, 0, sizeof(VESAVbeModeInfoBlock));

        regs.w.ax = 0x4F01;
        regs.w.cx = mode;
        regs.w.di = 0;
        sregs.es = vesaModeSegment;

        int86x(0x10, &regs, &regs, &sregs);

        if (regs.w.ax != 0x004F)
        {
            if (regs.w.ax != 335)
                printf("VESA call failed mode %04Xh, error %i\n", mode, regs.w.ax);
            dosMemFree(vesaModeSegment);
            vesaVbeModeInfo = 0;
            return;
        }
        vbeModeInfoBlock = mode;
    }
}
