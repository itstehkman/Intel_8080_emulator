#ifndef CONSTS_H
#define CONSTS_H

#include <stdio.h>
#include <time.h>

/** TIMER SPECIFC **/

static clock_t time_since_sleep = 0;
static unsigned short inst_since_sleep = 1;

/** CONSTS **/

#define NUM_ARGS 2
#define NUM_REGS 8
#define MEM_SIZE 0x10000  //16 bit addresses
#define ROM_START 0x0
#define NSECS_PER_CYCLE 500  // 2 MHz

/** STRUCTS **/

typedef enum {
  MOV, MVI, LDA, STA, LDAX, STAX, LHLD, SHLD, LXI, PUSH, POP, XTHL, SPHL,
	PCHL, XCHG, ADD, SUB, INR, DCR, CMP, ANA, ORA, XRA, ADI, SUI, CPI, ANI,
	ORI, XRI, DAA, ADC, ACI, SBB, SBI, DAD, INX, DCX, JMP, CALL, RET, JNZ,
	CNZ, RNZ, JZ, CZ, RZ, JNC, CNC, RNC, JC, CC, RC, JPO, CPO, RPO, JPE,
	CPE, RPE, JP, CP, RP, JM, CM, RM, RAL, RAR, RLC, RRC, IN, OUT, CMC, STC,
	CMA, HLT, NOP, DI, EI, RST0, RST1, RST2, RST3, RST4, RST5, RST6, RST7,
	ORG, END, EQU, SET, IF, ENDIF, DB, DW, DS
} instruction_t;

typedef enum {
  none, A, B, C, D, H, E, L, F, SP, M, PSW
} reg;

typedef struct cpu_state {
  char regs[NUM_REGS];
	char memory[MEM_SIZE];
	unsigned int rom_size;
  unsigned int sp;
  unsigned int pc;
} cpu_state;

typedef struct instruction instruction;

typedef void (*inst_func)(cpu_state *, instruction *);

typedef struct instruction {
  instruction_t inst_t;
	inst_func inst_func;
  reg regs[2];
  char num_bytes;
	char num_cycles;
} instruction;

/** FUNCTION DECLARATIONS **/

/*
 *  Loads the block of bytes of the rom into the cpu_state, given the filepath.
 *  Returns: 1 if successful load, 0 otherwise
 *  Errors: if file doesn't exist or there's a problem reading memory, 
 *	the state->rom will be NULL, so check for that.
 */
char load_rom (struct cpu_state *state, const char *filepath);

/*
 * Given the cpu_state, use pc to read an instruction from the 
 * rom and update the pc.
 */
instruction fetch_decode (struct cpu_state *state);

void print_inst(cpu_state *state, instruction *inst);
void emulate_cycle(cpu_state *state);
void lineup_in_cycle(cpu_state *state, void (*emulate_func)(cpu_state *state));
#endif
