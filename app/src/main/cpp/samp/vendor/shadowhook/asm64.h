#pragma once

#include <stdint.h>

// This thing has been written by RusJJ a.k.a. [-=KILL MAN=-]
// https://github.com/RusJJ

namespace ARMv8 {

    union CMPBits // CMP is an alias for SUBS
    {
        inline static uint32_t Create(uint32_t _imm, uint32_t _regn, bool isXreg)
        {
            // isXreg - is register X or W?
            CMPBits val; val.addr = isXreg ? 0xF100001F : 0x7100001F;

            val.regn = _regn;
            val.imm = _imm;

            return val.addr;
        }
        struct
        {
            uint32_t regd : 5; // register compare with (31 if CMP, otherwise it will became SUBS)
            uint32_t regn : 5; // register compare what?
            uint32_t imm : 12; // value is in range [0; 4095]
            uint32_t pad : 10; // descriptor of instruction
        };
        uint32_t addr;
    };

    union CMNBits // CMN is an alias for ADDS
    {
        inline static uint32_t Create(uint32_t _imm, uint32_t _regn, bool isXreg)
        {
            // isXreg - is register X or W?
            CMPBits val; val.addr = isXreg ? 0xB100001F : 0x3100001F;

            val.regn = _regn;
            val.imm = _imm;

            return val.addr;
        }
        struct
        {
            uint32_t regd : 5; // register compare with (31 if CMN, otherwise it will became ADDS)
            uint32_t regn : 5; // register compare what?
            uint32_t imm : 12; // value is in range [0; 4095]
            uint32_t pad : 10; // descriptor of instruction
        };
        uint32_t addr;
    };

    union MOVBits // A real MOV (not ORR or ADDS)
    {
        inline static uint32_t Create(uint32_t _imm, uint32_t _reg, bool isXreg)
        {
            // isXreg - is register X or W?
            MOVBits val; val.addr = isXreg ? 0xD2800000 : 0x52800000;

            val.reg = _reg;
            val.imm = _imm;

            return val.addr;
        }
        struct
        {
            uint32_t reg : 5;
            int32_t  imm : 16;
            uint32_t pad : 11; // descriptor of instruction
        };
        uint32_t addr;
    };

    union FMOVBits // FMOV (incomplete)
    {
        inline static uint32_t Create(float _imm, uint32_t _reg, bool isDoublePrec)
        {
            FMOVBits val; val.addr = isDoublePrec ? 0x1E601000 : 0x1E201000;

            val.reg = _reg;
            val.imm = _imm;

            return val.addr;
        }
        inline float GetValue()
        {
            uint32_t val;
            if(imm < 64)
            {
                val = 0x40000000 | (imm*0x100000)>>1;
                return *(float*)&val;
            }
            else if(imm < 128)
            {
                val = 0x3E000000 | ((imm-64)*0x100000)>>1;
                return *(float*)&val;
            }
            else if(imm < 192)
            {
                val = 0xC0000000 | ((imm-128)*0x100000)>>1;
                return *(float*)&val;
            }
            else
            {
                val = 0xBE000000 | ((imm-192)*0x100000)>>1;
                return *(float*)&val;
            }
        }
        struct
        {
            uint32_t reg  : 5;
            uint32_t pad1 : 8;
            uint32_t imm  : 8;
            uint32_t pad2 : 11;
        };
        uint32_t addr;
    };

    union BBits
    {
        inline static uint32_t Create(uintptr_t from, uintptr_t to, bool isBL = false)
        {
            BBits val; val.addr = isBL ? 0x94000000 : 0x14000000;

            val.imm = (uint32_t)(to - from) >> 2;

            return val.addr;
        }
        inline uintptr_t GetAddrTo(uintptr_t from)
        {
            return from + (imm << 2);
        }
        struct
        {
            int32_t  imm : 26;
            uint32_t pad  : 5;
            uint32_t isBL : 1;
        };
        uint32_t addr;
    };

    union ADRBits // Not ready
    {
        inline static uint32_t Create(uint32_t _imm, uint32_t _reg, bool isXreg)
        {
            // isXreg - is register X or W?
            ADRBits val; val.addr = isXreg ? 0xD2800000 : 0x52800000;

            val.reg = _reg;
            val.immhi = _imm;

            return val.addr;
        }
        struct
        {
            uint32_t reg :   5;
            uint32_t immhi : 19;
            uint32_t pad1 :  5;
            uint32_t immlo : 2;
            uint32_t isADRP :  1;
        };
        uint32_t addr;
    };

    union MOVRegBits
    {
        inline static uint32_t CreateMOV(uint32_t _from, uint32_t _to, bool isXreg)
        {
            // isXreg - is register X or W?
            MOVRegBits val; val.addr = isXreg ? 0xAA0003E0 : 0x2A0003E0;

            val.Rm = _from;
            val.Rd = _to;
            val.N = 0;

            return val.addr;
        }
        inline static uint32_t CreateMVN(uint32_t _from, uint32_t _to, bool isXreg)
        {
            // isXreg - is register X or W?
            MOVRegBits val; val.addr = isXreg ? 0xAA0003E0 : 0x2A0003E0;

            val.Rm = _from;
            val.Rd = _to;
            val.N = 1;

            return val.addr;
        }
        struct
        {
            uint32_t Rd : 5;
            uint32_t Rn : 5;
            uint32_t pad2 : 6;
            uint32_t Rm : 5;
            uint32_t N : 1;
            uint32_t shift : 2;
            uint32_t pad : 7;
            uint32_t isXReg : 1;
        };
        uint32_t addr;
    };

};