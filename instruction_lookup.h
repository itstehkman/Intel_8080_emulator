#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdio.h>

#include "structs_and_consts.h"
#include "instruction_funcs.h"

// You can index into the lookup table with the instruction in hex (ie inst_lookup[0xAF])

static const instruction inst_lookup[0x100] = {
{NOP, nop, {0,0}, 1, 4}, {LXI, lxi, {B,0}, 3, 10}, {STAX, stax, {B, 0}, 1, 7}, {INX, inx, {B, 0}, 1, 5},
{INR, inr, {B, 0}, 1, 5}, {DCR, dcr, {B, 0}, 1, 5}, {MVI, mvi, {B,0}, 2, 7}, {RLC, rlc, {0,0}, 1, 4}, 
{NOP, nop, {0,0}, 1, 4}, {DAD, dad, {B, 0}, 1, 10}, {LDAX, ldax, {B, 0}, 1, 7}, {DCX, dcx, {B, 0}, 1, 5}, 
{INR, inr, {C, 0}, 1, 5}, {DCR, dcr, {C, 0}, 1, 5}, {MVI, mvi, {C,0}, 2,7}, {RRC, rrc, {0,0}, 1, 4},

{NOP, nop, {0,0}, 1, 4}, {LXI, lxi, {D,0}, 3, 10}, {STAX, stax, {D, 0}, 1, 7}, {INX, inx, {D, 0}, 1, 5},
{INR, inr, {D, 0}, 1, 5}, {DCR, dcr, {D, 0}, 1, 5}, {MVI, mvi, {D,0}, 2, 7}, {RAL, ral, {0,0}, 1, 4}, 
{NOP, nop, {0,0}, 1, 4}, {DAD, dad, {D, 0}, 1, 10}, {LDAX, ldax, {D, 0}, 1, 7}, {DCX, dcx, {D, 0}, 1, 5}, 
{INR, inr, {E, 0}, 1, 5}, {DCR, dcr, {E, 0}, 1, 5}, {MVI, mvi, {E,0}, 2,7}, {RAR, rar, {0,0}, 1, 4},

{NOP, nop, {0,0}, 1, 4}, {LXI, lxi, {H,0}, 3, 10}, {SHLD, shld, {0, 0}, 3, 16}, {INX, inx, {H, 0}, 1, 5},
{INR, inr, {H, 0}, 1, 5}, {DCR, dcr, {H, 0}, 1, 5}, {MVI, mvi, {H,0}, 2, 7}, {DAA, daa, {0,0}, 1, 4}, 
{NOP, nop, {0,0}, 1, 4}, {DAD, dad, {H, 0}, 1, 10}, {LHLD, lhld, {0, 0}, 3, 16}, {DCX, dcx, {H, 0}, 1, 5}, 
{INR, inr, {L, 0}, 1, 5}, {DCR, dcr, {L, 0}, 1, 5}, {MVI, mvi, {L,0}, 2,7}, {CMA, cma, {0,0}, 1, 4},

{NOP, nop, {0,0}, 1, 4}, {LXI, lxi, {SP,0}, 3, 10}, {STA, sta, {0, 0}, 3, 13}, {INX, inx, {SP, 0}, 1, 5},
{INR, inr, {H, 0}, 1, 5}, {DCR, dcr, {H, 0}, 1, 5}, {MVI, mvi, {H,0}, 2, 7}, {STC, stc, {0,0}, 1, 4}, 
{NOP, nop, {0,0}, 1, 4}, {DAD, dad, {SP, 0}, 1, 10}, {LDA, lda, {0, 0}, 3, 13}, {DCX, dcx, {SP, 0}, 1, 5}, 
{INR, inr, {A, 0}, 1, 5}, {DCR, dcr, {A, 0}, 1, 5}, {MVI, mvi, {A,0}, 2,7}, {CMC, cmc, {0,0}, 1, 4},

{MOV, mov, {B,B}, 1, 5}, {MOV, mov, {B,C}, 1, 5},{MOV, mov, {B,D}, 1, 5},{MOV, mov, {B,E}, 1, 5},
{MOV, mov, {B,H}, 1, 5}, {MOV, mov, {B,L}, 1, 5},{MOV, mov, {B,M}, 1, 7},{MOV, mov, {B,A}, 1, 5},
{MOV, mov, {C,B}, 1, 5}, {MOV, mov, {C,C}, 1, 5},{MOV, mov, {C,D}, 1, 5},{MOV, mov, {C,E}, 1, 5},
{MOV, mov, {C,H}, 1, 5}, {MOV, mov, {C,L}, 1, 5},{MOV, mov, {C,M}, 1, 7},{MOV, mov, {C,A}, 1, 5},

{MOV, mov, {D,B}, 1, 5}, {MOV, mov, {D,C}, 1, 5},{MOV, mov, {D,D}, 1, 5},{MOV, mov, {D,E}, 1, 5},
{MOV, mov, {D,H}, 1, 5}, {MOV, mov, {D,L}, 1, 5},{MOV, mov, {D,M}, 1, 7},{MOV, mov, {D,A}, 1, 5},
{MOV, mov, {E,B}, 1, 5}, {MOV, mov, {E,C}, 1, 5},{MOV, mov, {E,D}, 1, 5},{MOV, mov, {E,E}, 1, 5},
{MOV, mov, {E,H}, 1, 5}, {MOV, mov, {E,L}, 1, 5},{MOV, mov, {E,M}, 1, 7},{MOV, mov, {E,A}, 1, 5},

{MOV, mov, {H,B}, 1, 5}, {MOV, mov, {H,C}, 1, 5},{MOV, mov, {H,D}, 1, 5},{MOV, mov, {H,E}, 1, 5},
{MOV, mov, {H,H}, 1, 5}, {MOV, mov, {H,L}, 1, 5},{MOV, mov, {H,M}, 1, 7},{MOV, mov, {H,A}, 1, 5},
{MOV, mov, {L,B}, 1, 5}, {MOV, mov, {L,C}, 1, 5},{MOV, mov, {L,D}, 1, 5},{MOV, mov, {L,E}, 1, 5},
{MOV, mov, {L,H}, 1, 5}, {MOV, mov, {L,L}, 1, 5},{MOV, mov, {L,M}, 1, 7},{MOV, mov, {L,A}, 1, 5},

{MOV, mov, {M,B}, 1, 7}, {MOV, mov, {M,C}, 1, 7},{MOV, mov, {M,D}, 1, 7},{MOV, mov, {M,E}, 1, 7},
{MOV, mov, {M,H}, 1, 7}, {MOV, mov, {M,L}, 1, 5},{HLT, hlt, {0,0}, 1, 7},{MOV, mov, {M,A}, 1, 7},
{MOV, mov, {A,B}, 1, 5}, {MOV, mov, {A,C}, 1, 5},{MOV, mov, {A,D}, 1, 5},{MOV, mov, {A,E}, 1, 5},
{MOV, mov, {A,H}, 1, 5}, {MOV, mov, {A,L}, 1, 5},{MOV, mov, {A,M}, 1, 7},{MOV, mov, {A,A}, 1, 5}, 

{ADD, add, {B, 0}, 1, 4}, {ADD, add, {C, 0}, 1, 4}, {ADD, add, {D, 0}, 1, 4}, {ADD, add, {E, 0}, 1, 4},
{ADD, add, {H, 0}, 1, 4}, {ADD, add, {L, 0}, 1, 4}, {ADD, add, {M, 0}, 1, 7}, {ADD, add, {A, 0}, 1, 4},
{ADC, adc, {B, 0}, 1, 4}, {ADC, adc, {C, 0}, 1, 4}, {ADC, adc, {D, 0}, 1, 4}, {ADC, adc, {E, 0}, 1, 4},
{ADC, adc, {H, 0}, 1, 4}, {ADC, adc, {L, 0}, 1, 4}, {ADC, adc, {M, 0}, 1, 7}, {ADC, adc, {A, 0}, 1, 4},

{SUB, sub, {B, 0}, 1, 4}, {SUB, sub, {C, 0}, 1, 4}, {SUB, add, {D, 0}, 1, 4}, {SUB, sub, {E, 0}, 1, 4},
{SUB, sub, {H, 0}, 1, 4}, {SUB, sub, {L, 0}, 1, 4}, {SUB, sub, {M, 0}, 1, 7}, {SUB, sub, {A, 0}, 1, 4},
{SBB, sbb, {B, 0}, 1, 4}, {SBB, sbb, {C, 0}, 1, 4}, {SBB, sbb, {D, 0}, 1, 4}, {SBB, sbb, {E, 0}, 1, 4},
{SBB, sbb, {H, 0}, 1, 4}, {SBB, sbb, {L, 0}, 1, 4}, {SBB, sbb, {M, 0}, 1, 7}, {SBB, sbb, {A, 0}, 1, 4},

{ANA, ana, {B, 0}, 1, 4}, {ANA, ana, {C, 0}, 1, 4}, {ANA, ana, {D, 0}, 1, 4}, {ANA, ana, {E, 0}, 1, 4},
{ANA, ana, {H, 0}, 1, 4}, {ANA, ana, {L, 0}, 1, 4}, {ANA, ana, {M, 0}, 1, 7}, {ANA, ana, {A, 0}, 1, 4},
{XRA, xra, {B, 0}, 1, 4}, {XRA, xra, {C, 0}, 1, 4}, {XRA, xra, {D, 0}, 1, 4}, {XRA, xra, {E, 0}, 1, 4},
{XRA, xra, {H, 0}, 1, 4}, {XRA, xra, {L, 0}, 1, 4}, {XRA, xra, {M, 0}, 1, 7}, {XRA, xra, {A, 0}, 1, 4},

{ORA, ora, {B, 0}, 1, 4}, {ORA, ora, {C, 0}, 1, 4}, {ORA, ora, {D, 0}, 1, 4}, {ORA, ora, {E, 0}, 1, 4},
{ORA, ora, {H, 0}, 1, 4}, {ORA, ora, {L, 0}, 1, 4}, {ORA, ora, {M, 0}, 1, 7}, {ORA, ora, {A, 0}, 1, 4},
{CMP, cmp, {B, 0}, 1, 4}, {CMP, cmp, {C, 0}, 1, 4}, {CMP, cmp, {D, 0}, 1, 4}, {CMP, cmp, {E, 0}, 1, 4},
{CMP, cmp, {H, 0}, 1, 4}, {CMP, cmp, {L, 0}, 1, 4}, {CMP, cmp, {M, 0}, 1, 7}, {CMP, cmp, {A, 0}, 1, 4},

{RNZ, rnz, {0,0}, 1, 11}, {POP, pop, {B,0}, 1, 10}, {JNZ, jnz, {0,0}, 3, 10}, {JMP, jmp, {0,0}, 3, 10},
{CNZ, cnz, {0,0}, 3, 17}, {PUSH, push, {B, 0}, 1, 11}, {ADI, adi, {0,0}, 2, 7}, {RST0, rst0, {0,0}, 1, 11},
{RZ, rz, {0,0}, 1, 11}, {RET, ret, {0,0}, 1, 10}, {JZ, jz, {0,0}, 3, 10}, {JMP, jmp, {0,0}, 3, 10},
{CZ, cz, {0,0}, 3, 17}, {CALL, call, {0,0}, 3, 17}, {ACI, aci, {0,0}, 2, 7}, {RST1, rst1, {0,0}, 1, 11},

{RNC, rnc, {0,0}, 1, 11}, {POP, pop, {D,0}, 1, 10}, {JNC, jnc, {0,0}, 3, 10}, {OUT, out, {0,0}, 2, 10},
{CNC, cnc, {0,0}, 3, 17}, {PUSH, push, {D, 0}, 1, 11}, {SUI, sui, {0,0}, 2, 7}, {RST2, rst2, {0,0}, 1, 11},
{RC, rc, {0,0}, 1, 11}, {RET, ret, {0,0}, 1, 10}, {JC, jc, {0,0}, 3, 10}, {IN, in, {0,0}, 2, 10},
{CC, cc, {0,0}, 3, 17}, {CALL, call, {0,0}, 3, 17}, {SBI, sbi, {0,0}, 2, 7}, {RST3, rst3, {0,0}, 1, 11},

{RPO, rpo, {0,0}, 1, 11}, {POP, pop, {H,0}, 1, 10}, {JPO, jpo, {0,0}, 3, 10}, {XTHL, xthl, {0,0}, 1, 18},
{CPO, cpo, {0,0}, 3, 17}, {PUSH, push, {H, 0}, 1, 11}, {ANI, ani, {0,0}, 2, 7}, {RST4, rst4, {0,0}, 1, 11},
{RPE, rpe, {0,0}, 1, 11}, {PCHL, pchl, {0,0}, 1, 5}, {JPE, jpe, {0,0}, 3, 10}, {XCHG, xchg, {0,0}, 1, 5},
{CPE, cpe, {0,0}, 3, 17}, {CALL, call, {0,0}, 3, 17}, {XRI, xri, {0,0}, 2, 7}, {RST5, rst5, {0,0}, 1, 11},

{RP, rp, {0,0}, 1, 11}, {POP, pop, {PSW,0}, 1, 10}, {JP, jp, {0,0}, 3, 10}, {DI, di, {0,0}, 1, 4},
{CP, cp, {0,0}, 3, 17}, {PUSH, push, {PSW, 0}, 1, 11}, {ORI, ori, {0,0}, 2, 7}, {RST6, rst6, {0,0}, 1, 11},
{RM, rm, {0,0}, 1, 11}, {SPHL, sphl, {0,0}, 1, 5}, {JM, jm, {0,0}, 3, 10}, {EI, ei, {0,0}, 1, 4},
{CM, cm, {0,0}, 3, 17}, {CALL, call, {0,0}, 3, 17}, {CPI, cpi, {0,0}, 2, 7}, {RST7, rst7, {0,0}, 1, 11}

};

#endif
