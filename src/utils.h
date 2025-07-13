#ifndef UTILS_H
#define UTILS_H
#include "setup.h"
#include <i86.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

extern union REGS utilsRegs;
extern struct SREGS utilsSRegs;

inline void far *dosMemAlloc(size_t amount, unsigned *segmentOut)
{
    utilsRegs.w.ax = 0x4800;
    utilsRegs.w.bx = (amount + 15) >> 4;

    int86(0x21, &utilsRegs, &utilsRegs);
    if (utilsRegs.x.cflag)
        return 0;
    if (segmentOut)
        *segmentOut = utilsRegs.w.ax;
    return (void *)(((unsigned int)utilsRegs.w.ax) << 4);
}

inline void dosMemFree(int segment)
{
    utilsRegs.w.ax = 0x4900; // DOS function: Free memory block
    utilsSRegs.es = segment;
    int86x(0x21, &utilsRegs, &utilsRegs, &utilsSRegs);
    if (utilsRegs.x.cflag)
        printf("Unable to dallocate memory\n");
}

#endif
