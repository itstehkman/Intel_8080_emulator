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
    /*print_inst(state, inst);
    print_state(state);
    printf("\n");*/
}

void print_state(cpu_state *state, FILE *fp) {
    fprintf(fp, "\t");  // A, B, C, D, H, E, L, F, SP, M, PSW
    fprintf(fp, "A: %02x B: %02x C: %02x D: %02x E: %02x H: %02x L: %02x SP: %04x",
           state->regs[A], state->regs[B], state->regs[C], state->regs[D], state->regs[E], state->regs[H],
           state->regs[L], state->sp);
    
    fprintf(fp, " %c%c%c%c%c",
           char_if_bit_else_period(state, S_shift, 's'),
           char_if_bit_else_period(state, Z_shift, 'z'),
           char_if_bit_else_period(state, A_shift, 'a'),
           char_if_bit_else_period(state, P_shift, 'p'),
           char_if_bit_else_period(state, C_shift, 'c'));
}

// PRINT
void print_inst(cpu_state *state, instruction *inst, FILE *fp) {
    fprintf(fp, "%04x ", state->pc);
    
    instruction_t inst_t = inst->inst_t;
    switch (inst_t) {
        case MOV:
            fprintf(fp, "MOV");
            break;
        case MVI:
           fprintf(fp, "MVI");
            break;
        case LDA:
           fprintf(fp, "LDA");
            break;
        case STA:
           fprintf(fp, "STA");
            break;
        case LDAX:
           fprintf(fp, "LDAX");
            break;
        case STAX:
           fprintf(fp, "STAX");
            break;
        case LHLD:
           fprintf(fp, "LHLD");
            break;
        case SHLD:
           fprintf(fp, "SHLD");
            break;
        case LXI:
           fprintf(fp, "LXI");
            break;
        case PUSH:
           fprintf(fp, "PUSH");
            break;
        case POP:
           fprintf(fp, "POP");
            break;
        case XTHL:
           fprintf(fp, "XTHL");
            break;
        case SPHL:
           fprintf(fp, "SPHL");
            break;
        case PCHL:
           fprintf(fp, "PCHL");
            break;
        case XCHG:
           fprintf(fp, "XCHG");
            break;
        case ADD:
           fprintf(fp, "ADD");
            break;
        case SUB:
           fprintf(fp, "SUB");
            break;
        case INR:
           fprintf(fp, "INR");
            break;
        case DCR:
           fprintf(fp, "DCR");
            break;
        case CMP:
           fprintf(fp, "CMP");
            break;
        case ANA:
           fprintf(fp, "ANA");
            break;
        case ORA:
           fprintf(fp, "ORA");
            break;
        case XRA:
           fprintf(fp, "XRA");
            break;
        case ADI:
           fprintf(fp, "ADI");
            break;
        case SUI:
           fprintf(fp, "SUI");
            break;
        case CPI:
           fprintf(fp, "CPI");
            break;
        case ANI:
           fprintf(fp, "ANI");
            break;
        case ORI:
           fprintf(fp, "ORI");
            break;
        case XRI:
           fprintf(fp, "XRI");
            break;
        case DAA:
           fprintf(fp, "DAA");
            break;
        case ADC:
           fprintf(fp, "ADC");
            break;
        case ACI:
           fprintf(fp, "ACI");
            break;
        case SBB:
           fprintf(fp, "SBB");
            break;
        case SBI:
           fprintf(fp, "SBI");
            break;
        case DAD:
           fprintf(fp, "DAD");
            break;
        case INX:
           fprintf(fp, "INX");
            break;
        case DCX:
           fprintf(fp, "DCX");
            break;
        case JMP:
           fprintf(fp, "JMP");
            break;
        case CALL:
           fprintf(fp, "CALL");
            break;
        case RET:
           fprintf(fp, "RET");
            break;
        case JNZ:
           fprintf(fp, "JNZ");
            break;
        case CNZ:
           fprintf(fp, "CNZ");
            break;
        case RNZ:
           fprintf(fp, "RNZ");
            break;
        case JZ:
           fprintf(fp, "JZ");
            break;
        case CZ:
           fprintf(fp, "CZ");
            break;
        case RZ:
           fprintf(fp, "RZ");
            break;
        case JNC:
           fprintf(fp, "JNC");
            break;
        case CNC:
           fprintf(fp, "CNC");
            break;
        case RNC:
           fprintf(fp, "RNC");
            break;
        case JC:
           fprintf(fp, "JC");
            break;
        case CC:
           fprintf(fp, "CC");
            break;
        case RC:
           fprintf(fp, "RC");
            break;
        case JPO:
           fprintf(fp, "JPO");
            break;
        case CPO:
           fprintf(fp, "CPO");
            break;
        case RPO:
           fprintf(fp, "RPO");
            break;
        case JPE:
           fprintf(fp, "JPE");
            break;
        case CPE:
           fprintf(fp, "CPE");
            break;
        case RPE:
           fprintf(fp, "RPE");
            break;
        case JP:
           fprintf(fp, "JP");
            break;
        case CP:
           fprintf(fp, "CP");
            break;
        case RP:
           fprintf(fp, "RP");
            break;
        case JM:
           fprintf(fp, "JM");
            break;
        case CM:
           fprintf(fp, "CM");
            break;
        case RM:
           fprintf(fp, "RM");
            break;
        case RAL:
           fprintf(fp, "RAL");
            break;
        case RAR:
           fprintf(fp, "RAR");
            break;
        case RLC:
           fprintf(fp, "RLC");
            break;
        case RRC:
           fprintf(fp, "RRC");
            break;
        case IN:
           fprintf(fp, "IN");
            break;
        case OUT:
           fprintf(fp, "OUT");
            break;
        case CMC:
           fprintf(fp, "CMC");
            break;
        case STC:
           fprintf(fp, "STC");
            break;
        case CMA:
           fprintf(fp, "CMA");
            break;
        case HLT:
           fprintf(fp, "HLT");
            break;
        case NOP:
           fprintf(fp, "NOP");
            break;
        case DI:
           fprintf(fp, "DI");
            break;
        case EI:
           fprintf(fp, "EI");
            break;
        case RST0:
           fprintf(fp, "RST0");
            break;
        case RST1:
           fprintf(fp, "RST1");
            break;
        case RST2:
           fprintf(fp, "RST2");
            break;
        case RST3:
           fprintf(fp, "RST3");
            break;
        case RST4:
           fprintf(fp, "RST4");
            break;
        case RST5:
           fprintf(fp, "RST5");
            break;
        case RST6:
           fprintf(fp, "RST6");
            break;
        case RST7:
           fprintf(fp, "RST7");
            break;
        case ORG:
           fprintf(fp, "ORG");
            break;
        case END:
           fprintf(fp, "END");
            break;
        case EQU:
           fprintf(fp, "EQU");
            break;
        case SET:
           fprintf(fp, "SET");
            break;
        case IF:
           fprintf(fp, "IF");
            break;
        case ENDIF:
           fprintf(fp, "ENDIF");
            break;
        case DB:
           fprintf(fp, "DB");
            break;
        case DW:
           fprintf(fp, "DW");
            break;
        case DS:
           fprintf(fp, "DS");
            break;
        default:
           fprintf(fp, "unimplemented");
            break;
    }
    
    for (int i = 0; i < 2; i++) {
        switch (inst->regs[i]) {
            case A:
               fprintf(fp, " A");
                break;
            case B:
               fprintf(fp, " B");
                break;
            case C:
               fprintf(fp, " C");
                break;
            case D:
               fprintf(fp, " D");
                break;
            case H:
               fprintf(fp, " H");
                break;
            case E:
               fprintf(fp, " E");
                break;
            case L:
               fprintf(fp, " L");
                break;
            case SP:
               fprintf(fp, " SP");
                break;
            case M:
               fprintf(fp, " M");
                break;
            case PSW:
               fprintf(fp, " PSW");
                break;
            case none:
               fprintf(fp, "");
                break;
            default:
               fprintf(fp, " ?");
                break;
        }
    }
   fprintf(fp, " ");
    
    for (int i = inst->num_bytes - 1; i > 0; i--) {
       fprintf(fp, "%02x", (unsigned char)state->memory[state->pc+i]);
    }

}
