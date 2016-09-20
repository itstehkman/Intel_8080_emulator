//
//  print.c
//  8080_emulator
//
//  Created by Kareem Moussa on 7/23/16.
//  Copyright © 2016 Kareem Moussa. All rights reserved.
//
#import <stdio.h>
#import "structs_and_consts.h"

char char_if_bit_else_period(cpu_state *state, uint8_t bit_shift, char display) {
    uint8_t flag = get_flagbit(state, bit_shift);
    return flag ? display : '.';
}

void print_debug(cpu_state *state, instruction *inst) {
    print_inst(state, inst);
    print_state(state);
    printf("\n");
}

void print_state(cpu_state *state) {
    printf("\t");  // A, B, C, D, H, E, L, F, SP, M, PSW
    printf("A: %02x B: %02x C: %02x D: %02x E: %02x H: %02x L: %02x SP: %04x",
           state->regs[A], state->regs[B], state->regs[C], state->regs[D], state->regs[E], state->regs[H],
           state->regs[L], state->sp);
    
    printf(" %c%c%c%c%c",
           char_if_bit_else_period(state, S_shift, 's'),
           char_if_bit_else_period(state, Z_shift, 'z'),
           char_if_bit_else_period(state, A_shift, 'a'),
           char_if_bit_else_period(state, P_shift, 'p'),
           char_if_bit_else_period(state, C_shift, 'c'));
}

// PRINT
void print_inst(cpu_state *state, instruction *inst) {
    printf("%04x ", state->pc);
    
    instruction_t inst_t = inst->inst_t;
    switch (inst_t) {
        case MOV:
            printf("MOV");
            break;
        case MVI:
            printf("MVI");
            break;
        case LDA:
            printf("LDA");
            break;
        case STA:
            printf("STA");
            break;
        case LDAX:
            printf("LDAX");
            break;
        case STAX:
            printf("STAX");
            break;
        case LHLD:
            printf("LHLD");
            break;
        case SHLD:
            printf("SHLD");
            break;
        case LXI:
            printf("LXI");
            break;
        case PUSH:
            printf("PUSH");
            break;
        case POP:
            printf("POP");
            break;
        case XTHL:
            printf("XTHL");
            break;
        case SPHL:
            printf("SPHL");
            break;
        case PCHL:
            printf("PCHL");
            break;
        case XCHG:
            printf("XCHG");
            break;
        case ADD:
            printf("ADD");
            break;
        case SUB:
            printf("SUB");
            break;
        case INR:
            printf("INR");
            break;
        case DCR:
            printf("DCR");
            break;
        case CMP:
            printf("CMP");
            break;
        case ANA:
            printf("ANA");
            break;
        case ORA:
            printf("ORA");
            break;
        case XRA:
            printf("XRA");
            break;
        case ADI:
            printf("ADI");
            break;
        case SUI:
            printf("SUI");
            break;
        case CPI:
            printf("CPI");
            break;
        case ANI:
            printf("ANI");
            break;
        case ORI:
            printf("ORI");
            break;
        case XRI:
            printf("XRI");
            break;
        case DAA:
            printf("DAA");
            break;
        case ADC:
            printf("ADC");
            break;
        case ACI:
            printf("ACI");
            break;
        case SBB:
            printf("SBB");
            break;
        case SBI:
            printf("SBI");
            break;
        case DAD:
            printf("DAD");
            break;
        case INX:
            printf("INX");
            break;
        case DCX:
            printf("DCX");
            break;
        case JMP:
            printf("JMP");
            break;
        case CALL:
            printf("CALL");
            break;
        case RET:
            printf("RET");
            break;
        case JNZ:
            printf("JNZ");
            break;
        case CNZ:
            printf("CNZ");
            break;
        case RNZ:
            printf("RNZ");
            break;
        case JZ:
            printf("JZ");
            break;
        case CZ:
            printf("CZ");
            break;
        case RZ:
            printf("RZ");
            break;
        case JNC:
            printf("JNC");
            break;
        case CNC:
            printf("CNC");
            break;
        case RNC:
            printf("RNC");
            break;
        case JC:
            printf("JC");
            break;
        case CC:
            printf("CC");
            break;
        case RC:
            printf("RC");
            break;
        case JPO:
            printf("JPO");
            break;
        case CPO:
            printf("CPO");
            break;
        case RPO:
            printf("RPO");
            break;
        case JPE:
            printf("JPE");
            break;
        case CPE:
            printf("CPE");
            break;
        case RPE:
            printf("RPE");
            break;
        case JP:
            printf("JP");
            break;
        case CP:
            printf("CP");
            break;
        case RP:
            printf("RP");
            break;
        case JM:
            printf("JM");
            break;
        case CM:
            printf("CM");
            break;
        case RM:
            printf("RM");
            break;
        case RAL:
            printf("RAL");
            break;
        case RAR:
            printf("RAR");
            break;
        case RLC:
            printf("RLC");
            break;
        case RRC:
            printf("RRC");
            break;
        case IN:
            printf("IN");
            break;
        case OUT:
            printf("OUT");
            break;
        case CMC:
            printf("CMC");
            break;
        case STC:
            printf("STC");
            break;
        case CMA:
            printf("CMA");
            break;
        case HLT:
            printf("HLT");
            break;
        case NOP:
            printf("NOP");
            break;
        case DI:
            printf("DI");
            break;
        case EI:
            printf("EI");
            break;
        case RST0:
            printf("RST0");
            break;
        case RST1:
            printf("RST1");
            break;
        case RST2:
            printf("RST2");
            break;
        case RST3:
            printf("RST3");
            break;
        case RST4:
            printf("RST4");
            break;
        case RST5:
            printf("RST5");
            break;
        case RST6:
            printf("RST6");
            break;
        case RST7:
            printf("RST7");
            break;
        case ORG:
            printf("ORG");
            break;
        case END:
            printf("END");
            break;
        case EQU:
            printf("EQU");
            break;
        case SET:
            printf("SET");
            break;
        case IF:
            printf("IF");
            break;
        case ENDIF:
            printf("ENDIF");
            break;
        case DB:
            printf("DB");
            break;
        case DW:
            printf("DW");
            break;
        case DS:
            printf("DS");
            break;
        default:
            printf("unimplemented");
            break;
    }
    
    for (int i = 0; i < 2; i++) {
        switch (inst->regs[i]) {
            case A:
                printf(" A");
                break;
            case B:
                printf(" B");
                break;
            case C:
                printf(" C");
                break;
            case D:
                printf(" D");
                break;
            case H:
                printf(" H");
                break;
            case E:
                printf(" E");
                break;
            case L:
                printf(" L");
                break;
            case SP:
                printf(" SP");
                break;
            case M:
                printf(" M");
                break;
            case PSW:
                printf(" PSW");
                break;
            case none:
                printf("");
                break;
            default:
                printf(" ?");
                break;
        }
    }
    printf(" ");
    
    for (int i = inst->num_bytes - 1; i > 0; i--) {
        printf("%02x", (unsigned char)state->memory[state->pc+i]);
    }

}
