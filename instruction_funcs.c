#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

// Courtesy of http://fms.komkon.org/comp/CPUs/8080.txt for
// list of instruction types

//Instruction implementations
void nop(cpu_state *state, instruction *inst) {

}
void hlt(cpu_state *state, instruction *inst) {

}

// 8 bit transfers
void mov(cpu_state *state, instruction *inst) {}  
void mvi(cpu_state *state, instruction *inst) {}
void lda(cpu_state *state, instruction *inst) {}
void sta(cpu_state *state, instruction *inst) {}
void ldax(cpu_state *state, instruction *inst){}
void stax(cpu_state *state, instruction *inst){}

// 16 bit transfers
void lhld(cpu_state *state, instruction *inst){}
void shld(cpu_state *state, instruction *inst){}
void lxi(cpu_state *state, instruction *inst) {}
void push(cpu_state *state, instruction *inst){}
void pop(cpu_state *state, instruction *inst) {}
void xthl(cpu_state *state, instruction *inst){} 
void sphl(cpu_state *state, instruction *inst){}
void pchl(cpu_state *state, instruction *inst){}
void xchg(cpu_state *state, instruction *inst){}

// 8 bit arithmetic
void add(cpu_state *state, instruction *inst){} 
void sub(cpu_state *state, instruction *inst){}
void inr(cpu_state *state, instruction *inst){}
void dcr(cpu_state *state, instruction *inst){}
void cmp(cpu_state *state, instruction *inst){}
void ana(cpu_state *state, instruction *inst){}
void ora(cpu_state *state, instruction *inst){}
void xra(cpu_state *state, instruction *inst){}

void adi(cpu_state *state, instruction *inst){}
void sui(cpu_state *state, instruction *inst){}
void cpi(cpu_state *state, instruction *inst){}
void ani(cpu_state *state, instruction *inst){}
void ori(cpu_state *state, instruction *inst){}
void xri(cpu_state *state, instruction *inst){}

void daa(cpu_state *state, instruction *inst){}
void adc(cpu_state *state, instruction *inst){}
void aci(cpu_state *state, instruction *inst){}
void sbb(cpu_state *state, instruction *inst){}
void sbi(cpu_state *state, instruction *inst){}

// 16 bit arithmetic
void dad(cpu_state *state, instruction *inst){}
void inx(cpu_state *state, instruction *inst){}
void dcx(cpu_state *state, instruction *inst){}

// Jumps, calls, returns
void jmp(cpu_state *state, instruction *inst){}
void call(cpu_state *state, instruction *inst){}
void ret(cpu_state *state, instruction *inst){} 

void jnz(cpu_state *state, instruction *inst){} 
void cnz(cpu_state *state, instruction *inst){}
void rnz(cpu_state *state, instruction *inst){}
void jz(cpu_state *state, instruction *inst){}
void cz(cpu_state *state, instruction *inst){}
void rz(cpu_state *state, instruction *inst){}
void jnc(cpu_state *state, instruction *inst){}
void cnc(cpu_state *state, instruction *inst){}
void rnc(cpu_state *state, instruction *inst){}
void jc(cpu_state *state, instruction *inst){}
void cc(cpu_state *state, instruction *inst){}
void rc(cpu_state *state, instruction *inst){}
void jpo(cpu_state *state, instruction *inst){}
void cpo(cpu_state *state, instruction *inst){}
void rpo(cpu_state *state, instruction *inst){}
void jpe(cpu_state *state, instruction *inst){}
void cpe(cpu_state *state, instruction *inst){}
void rpe(cpu_state *state, instruction *inst){}
void jp(cpu_state *state, instruction *inst){}
void cp(cpu_state *state, instruction *inst){}
void rp(cpu_state *state, instruction *inst){}
void jm(cpu_state *state, instruction *inst){}
void cm(cpu_state *state, instruction *inst){}
void rm(cpu_state *state, instruction *inst){}

// Rotations
void ral(cpu_state *state, instruction *inst){}
void rar(cpu_state *state, instruction *inst){}
void rlc(cpu_state *state, instruction *inst){}
void rrc(cpu_state *state, instruction *inst){}

// Other
void in(cpu_state *state, instruction *inst){}
void out(cpu_state *state, instruction *inst){}

void cmc(cpu_state *state, instruction *inst){}
void stc(cpu_state *state, instruction *inst){}
void cma(cpu_state *state, instruction *inst){}

void di(cpu_state *state, instruction *inst) {}
void ei(cpu_state *state, instruction *inst) {}

void rst0(cpu_state *state, instruction *inst){} 
void rst1(cpu_state *state, instruction *inst){}
void rst2(cpu_state *state, instruction *inst){}
void rst3(cpu_state *state, instruction *inst){}
void rst4(cpu_state *state, instruction *inst){}
void rst5(cpu_state *state, instruction *inst){}
void rst6(cpu_state *state, instruction *inst){}
void rst7(cpu_state *state, instruction *inst){}

// Assembler directives
void org(cpu_state *state, instruction *inst){}
void end(cpu_state *state, instruction *inst){}
void equ(cpu_state *state, instruction *inst){}

void set(cpu_state *state, instruction *inst){}
void _if(cpu_state *state, instruction *inst){}
void _endif(cpu_state *state, instruction *inst){}
void db(cpu_state *state, instruction *inst){}
void dw(cpu_state *state, instruction *inst){}
void ds(cpu_state *state, instruction *inst){}

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

	printf("\n");
}

