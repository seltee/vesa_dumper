#ifndef SETUP_H
#define SETUP_H

#ifndef __WATCOMC__ // Only for IntelliSense, not for Watcom
#define far
#define MK_FP(seg, ofs) ((void *)0) // Optional: avoid errors for MK_FP too
#define _M_I86
#endif

#pragma aux waitCPU = "hlt";

typedef struct
{
    int vv;
} Doseraptor;

#endif
