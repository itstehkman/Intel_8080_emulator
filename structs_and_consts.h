#ifndef CONSTS_H
#define CONSTS_H

#include <stdio.h>
#include <time.h>

/** CONSTS **/

#define NUM_ARGS 2

#define NUM_REGS 12
#define NUM_INST_TYPE 94
#define MEM_SIZE 0x10000  //16 bit addresses
#define ROM_START 0x0

#define NSECS_PER_CYCLE 500 // 2 MHz
#define USECS_PER_CYCLE .5

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256

#define S_shift 7
#define Z_shift 6
#define A_shift 4
#define P_shift 2
#define C_shift 0

#define S_mask  1 << S_shift
#define Z_mask  1 << Z_shift
#define A_mask  1 << A_shift  // Unused in space invaders
#define P_mask  1 << P_shift
#define C_mask  1 << C_shift


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
    uint8_t regs[NUM_REGS];
    uint8_t memory[MEM_SIZE];
    uint32_t rom_size;
    uint16_t sp;
    uint16_t pc;
    
    uint8_t interrupt_enable;
    
    // external hardware
    uint8_t shift_left_byte;
    uint8_t shift_right_byte;
    uint8_t shift_offset;
    
    uint8_t ports[6];
    
} cpu_state;

typedef struct instruction instruction;

typedef void (*inst_func)(cpu_state *, instruction *);

typedef struct instruction {
    instruction_t inst_t;
    inst_func inst_func;
    reg regs[2];
    uint8_t num_bytes;
    uint8_t num_cycles;
} instruction;

/** FUNCTION DECLARATIONS **/

void set_debug_mode(uint8_t mode);
void print_debug(cpu_state *state, instruction *inst);
void print_inst(cpu_state *state, instruction *inst, FILE *fp);
void print_state(cpu_state *state, FILE *fp);

/*
 *  Loads the block of bytes of the rom into the cpu_state, given the filepath.
 *  Returns: 1 if successful load, 0 otherwise
 *  Errors: if file doesn't exist or there's a problem reading memory,
 *	the state->rom will be NULL, so check for that.
 */
uint8_t load_rom(struct cpu_state *state, const char *filepath);
cpu_state cpu_state_from_rom_file(const char* rom_path);
void run_cpu(cpu_state *state);
void gen_interrupt(cpu_state *state, uint8_t interrupt_num);

/*
 * Given the cpu_state, use pc to read an instruction from the
 * rom and update the pc.
 */
instruction fetch_decode(struct cpu_state *state);
uint16_t emulate_inst_and_get_num_cycles(cpu_state *state, FILE *fp);
void lineup_with_cpu_rate(cpu_state *state, unsigned short (*emulate_func)(cpu_state *state, FILE *fp), FILE *fp);

uint16_t data_for_instruction(cpu_state *state, instruction *inst);
uint16_t data_for_regpair(cpu_state *state, reg regpair);
uint8_t is_pc_changing_inst(instruction *inst);

uint8_t get_flagbit(cpu_state *state, uint8_t bit_shift);

void push_data(cpu_state *state, uint16_t data);
uint16_t pop_data(cpu_state *state, uint8_t num_bytes);

#endif
