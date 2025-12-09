//
// Created by x1y2z on 20.03.2023.
// Updated: Added libsamp.so back to Possible Offsets (32/64)
//

#ifndef LIVERUSSIA_CSTACKTRACE_H
#define LIVERUSSIA_CSTACKTRACE_H

#include <stdio.h>
#include <dlfcn.h>
#include <unwind.h>
#include "../main.h"

// ================================================================================================
//  MACROS FOR CRASH STATES (Registers Dump)
// ================================================================================================

#if VER_x32
    // 32-Bit Architecture
    #define PRINT_CRASH_STATES(context) \
        FLog("========== REGISTER STATES (32-Bit) =========="); \
        FLog("R0:  0x%08X  R1:  0x%08X  R2:  0x%08X  R3:  0x%08X", \
            (context)->uc_mcontext.arm_r0, (context)->uc_mcontext.arm_r1, (context)->uc_mcontext.arm_r2, (context)->uc_mcontext.arm_r3); \
        FLog("R4:  0x%08X  R5:  0x%08X  R6:  0x%08X  R7:  0x%08X", \
            (context)->uc_mcontext.arm_r4, (context)->uc_mcontext.arm_r5, (context)->uc_mcontext.arm_r6, (context)->uc_mcontext.arm_r7); \
        FLog("R8:  0x%08X  R9:  0x%08X  R10: 0x%08X  FP:  0x%08X", \
            (context)->uc_mcontext.arm_r8, (context)->uc_mcontext.arm_r9, (context)->uc_mcontext.arm_r10, (context)->uc_mcontext.arm_fp); \
        FLog("IP:  0x%08X  SP:  0x%08X  LR:  0x%08X  PC:  0x%08X", \
            (context)->uc_mcontext.arm_ip, (context)->uc_mcontext.arm_sp, (context)->uc_mcontext.arm_lr, (context)->uc_mcontext.arm_pc); \
        FLog(" "); \
        FLog("Possible Offsets:"); \
        FLog("  libGTASA.so (32-Bit) + 0x%08X (PC)", (context)->uc_mcontext.arm_pc - g_libGTASA); \
        FLog("  libGTASA.so (32-Bit) + 0x%08X (LR)", (context)->uc_mcontext.arm_lr - g_libGTASA); \
        FLog("  libsamp.so  (32-Bit) + 0x%08X (PC)", (context)->uc_mcontext.arm_pc - g_libSAMP); \
        FLog("  libsamp.so  (32-Bit) + 0x%08X (LR)", (context)->uc_mcontext.arm_lr - g_libSAMP); \
        CheckCrashRange((uintptr_t)((context)->uc_mcontext.arm_pc - g_libGTASA)); // Check GTA Range

#else
    // 64-Bit Architecture
    #define PRINT_CRASH_STATES(context) \
        FLog("========== REGISTER STATES (64-Bit) =========="); \
        FLog("x0:  0x%016llx  x1:  0x%016llx  x2:  0x%016llx  x3:  0x%016llx", \
            (context)->uc_mcontext.regs[0], (context)->uc_mcontext.regs[1], (context)->uc_mcontext.regs[2], (context)->uc_mcontext.regs[3]); \
        FLog("x4:  0x%016llx  x5:  0x%016llx  x6:  0x%016llx  x7:  0x%016llx", \
            (context)->uc_mcontext.regs[4], (context)->uc_mcontext.regs[5], (context)->uc_mcontext.regs[6], (context)->uc_mcontext.regs[7]); \
        FLog("x8:  0x%016llx  x9:  0x%016llx  x10: 0x%016llx  x11: 0x%016llx", \
            (context)->uc_mcontext.regs[8], (context)->uc_mcontext.regs[9], (context)->uc_mcontext.regs[10], (context)->uc_mcontext.regs[11]); \
        FLog("x12: 0x%016llx  x13: 0x%016llx  x14: 0x%016llx  x15: 0x%016llx", \
            (context)->uc_mcontext.regs[12], (context)->uc_mcontext.regs[13], (context)->uc_mcontext.regs[14], (context)->uc_mcontext.regs[15]); \
        FLog("x16: 0x%016llx  x17: 0x%016llx  x18: 0x%016llx  x19: 0x%016llx", \
            (context)->uc_mcontext.regs[16], (context)->uc_mcontext.regs[17], (context)->uc_mcontext.regs[18], (context)->uc_mcontext.regs[19]); \
        FLog("x20: 0x%016llx  x21: 0x%016llx  x22: 0x%016llx  x23: 0x%016llx", \
            (context)->uc_mcontext.regs[20], (context)->uc_mcontext.regs[21], (context)->uc_mcontext.regs[22], (context)->uc_mcontext.regs[23]); \
        FLog("x24: 0x%016llx  x25: 0x%016llx  x26: 0x%016llx  x27: 0x%016llx", \
            (context)->uc_mcontext.regs[24], (context)->uc_mcontext.regs[25], (context)->uc_mcontext.regs[26], (context)->uc_mcontext.regs[27]); \
        FLog("x28: 0x%016llx  FP:  0x%016llx  LR:  0x%016llx  SP:  0x%016llx", \
            (context)->uc_mcontext.regs[28], (context)->uc_mcontext.regs[29], (context)->uc_mcontext.regs[30], (context)->uc_mcontext.sp); \
        FLog("PC:  0x%016llx", (context)->uc_mcontext.pc); \
        FLog(" "); \
        FLog("Possible Offsets:"); \
        FLog("  libGTASA.so (64-Bit) + 0x%llx (PC)", (unsigned long long)((context)->uc_mcontext.pc - g_libGTASA)); \
        FLog("  libGTASA.so (64-Bit) + 0x%llx (LR)", (unsigned long long)((context)->uc_mcontext.regs[30] - g_libGTASA)); \
        FLog("  libsamp.so  (64-Bit) + 0x%llx (PC)", (unsigned long long)((context)->uc_mcontext.pc - g_libSAMP)); \
        FLog("  libsamp.so  (64-Bit) + 0x%llx (LR)", (unsigned long long)((context)->uc_mcontext.regs[30] - g_libSAMP)); \
        CheckCrashRange((uintptr_t)((context)->uc_mcontext.pc - g_libGTASA)); // Check GTA Range
#endif

void FLog(const char* fmt, ...);
extern int g_iLastRenderedObject;

// Helper function to check address ranges (Start -> End)
static void CheckCrashRange(uintptr_t offset) {
    #if VER_x32
        // 32-Bit Range: 0x00000000 -> 0x00F13188
        if (offset >= 0x00000000 && offset <= 0x00F13188) {
            FLog("!!! CRASH INSIDE VALID 32-BIT GTA RANGE (0x0 - 0xF13188) !!!");
        } else {
            FLog("!!! CRASH OUTSIDE KNOWN 32-BIT RANGE !!!");
        }
    #else
        // 64-Bit Range: 0x0000000000000000 -> 0x00000000011F3B00
        if (offset >= 0x0 && offset <= 0x11F3B00) {
            FLog("!!! CRASH INSIDE VALID 64-BIT GTA RANGE (0x0 - 0x11F3B00) !!!");
        } else {
            FLog("!!! CRASH OUTSIDE KNOWN 64-BIT RANGE !!!");
        }
    #endif
}

// ================================================================================================
//  CSTACKTRACE CLASS
// ================================================================================================

class CStackTrace
{
public:
    static void printBacktrace()
    {
        FLog("Last rendered object: %d", g_iLastRenderedObject);
        FLog("------------ START BACKTRACE ------------");
        FLog(" ");
        PrintStackTrace();
        FLog("------------ END BACKTRACE ------------");
    }

private:
    static _Unwind_Reason_Code TraceFunction(_Unwind_Context* context, void* arg) {
        uintptr_t pc = _Unwind_GetIP(context);
        
        uintptr_t rel_samp = pc - g_libSAMP;
        uintptr_t rel_gta  = pc - g_libGTASA;

        Dl_info info;
        const char* symName = "(no symbol)";
        if (dladdr(reinterpret_cast<void*>(pc), &info) && info.dli_sname != nullptr) {
            symName = info.dli_sname;
        }

        #if VER_x32
            FLog("[adr: 0x%08X | samp: +0x%X | gta: +0x%X] %s", pc, rel_samp, rel_gta, symName);
            // Optional: Mark if in valid range
            if (rel_gta >= 0x0 && rel_gta <= 0x00F13188) {
                 // FLog("    ^-- [Valid GTA Range]"); // Uncomment if needed
            }
        #else
            FLog("[adr: 0x%016llx | samp: +0x%llx | gta: +0x%llx] %s", 
                (unsigned long long)pc, (unsigned long long)rel_samp, (unsigned long long)rel_gta, symName);
            // Optional: Mark if in valid range
            if (rel_gta >= 0x0 && rel_gta <= 0x11F3B00) {
                 // FLog("    ^-- [Valid GTA Range]"); // Uncomment if needed
            }
        #endif

        return _URC_NO_REASON;
    }

    static void PrintStackTrace() {
        _Unwind_Backtrace(TraceFunction, nullptr);
    }
};

#endif //LIVERUSSIA_CSTACKTRACE_H