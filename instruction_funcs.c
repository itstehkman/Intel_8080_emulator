#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

// Credits to http://fms.komkon.org/comp/CPUs/8080.txt for
// list of instruction types

// Credits to http://altairclone.com/downloads/manuals/8080%20Programmers%20Manual.pdf
// for how to implement the instructions

// INSTRUCTIONS NECESSARY FOR SPACE INVADERS:
// NOP, MOV, LXI, DCR, MVI, DAD, DCR, RRC, INX, LDAX, STA, LDA, ANA, XRA, POP, JNZ,
// JMP, PUSH, ADI, RET, CALL, OUT, ANI, XCHG, EI, CPI

// INSTRUCTIONS LEFT (13)
// DAD, RRC, INX, ANA, XRA, JNZ,
// ADI, RET, OUT, ANI, EI, CPI

// Stack operations:
// Push: msb stored at one byte less than sp.
//       lsb stored at 2 bytes less than sp
//       sp -= 2
// Pop: 2nd reg of pair (or lsb of pc) loaded from sp.
//      1st reg of pair (or msb of pc) loaded from sp+1
//      sp += 2

#pragma mark - helper function decls

static int is_allowed_to_write_here(cpu_state *state, uint16_t offset);
static void write_to_mem(cpu_state *state, uint16_t offset, uint16_t data);
//static void write_to_HL(cpu_state *state, uint16_t data);
static uint16_t read_from_HL(cpu_state *state);

static void push_data(cpu_state *state, uint16_t data);
static uint16_t pop_data(cpu_state *state, uint8_t num_bytes);
static uint16_t reverse2bytes(uint16_t data);

static uint8_t calc_parity(uint16_t data);

static void set_flagbits_szapc(cpu_state *state, uint8_t s, uint8_t z, uint8_t a, uint8_t p, uint8_t c);
static void set_flagbit(cpu_state *state, uint8_t bit, uint8_t bit_shift);
uint8_t get_flagbit(cpu_state *state, uint8_t bit_shift);

#pragma mark - common instructions

void nop(cpu_state *state, instruction *inst) {
    // do nothing...
}

void hlt(cpu_state *state, instruction *inst) {
    exit(1);
}

#pragma mark - 8 bit transfer instructions

void mov(cpu_state *state, instruction *inst) {
    reg reg0 = inst->regs[0];
    reg reg1 = inst->regs[1];
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        write_to_mem(state, address, state->regs[reg1]);
    }
    else if (reg1 == M) {
        uint16_t address = read_from_HL(state);
        state->regs[reg0] = state->memory[address];
    }
    else {
        state->regs[reg0] = state->regs[1];
    }
}

void mvi(cpu_state *state, instruction *inst) {
    reg reg0 = inst->regs[0];
    
    uint8_t data = data_for_instruction(state, inst);
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        write_to_mem(state, address, data);
    }
    else {
        state->regs[reg0] = data;
    }
}

void lda(cpu_state *state, instruction *inst) {
    uint16_t offset = data_for_instruction(state, inst);
    state->regs[A] = state->memory[offset];
}

void sta(cpu_state *state, instruction *inst) {
    uint16_t offset = data_for_instruction(state, inst);
    write_to_mem(state, offset, state->regs[A]);
}

void ldax(cpu_state *state, instruction *inst) {
    reg reg0 = inst->regs[0];
    uint16_t address = data_for_regpair(state, reg0);
    state->regs[A] = state->memory[address];
}

void stax(cpu_state *state, instruction *inst){}

#pragma mark - 16 bit transfers

void lhld(cpu_state *state, instruction *inst){}
void shld(cpu_state *state, instruction *inst){}

void lxi(cpu_state *state, instruction *inst) {
    reg reg0 = inst->regs[0];
    uint16_t data = data_for_instruction(state, inst);
    
    // NOTE: lsb is the leftmost and msb is the rightmost
    
    uint8_t msb = data;
    uint8_t lsb = (data >> 8);
    
    if (reg0 == B) { // BC register pair
        state->regs[B] = msb;
        state->regs[C] = lsb;
    }
    else if (reg0 == D) { // DE reg pair
        state->regs[D] = msb;
        state->regs[E] = lsb;
    }
    else if (reg0 == H) { // HL reg pair
        state->regs[H] = msb;
        state->regs[L] = lsb;
    }
    else if (reg0 == SP) {
        state->sp = (lsb << 8) | msb;
    }
    
}

void push(cpu_state *state, instruction *inst) {
// Push: 1st reg stored at sp - 1.
//       2nd reg stored at sp - 2
//       sp -= 2
    uint16_t data = data_for_regpair(state, inst->regs[0]);
    push_data(state, data);
}

void pop(cpu_state *state, instruction *inst) {
    // Pop: 2nd reg of pair (or lsb of pc) loaded from sp
    //      1st reg of pair (or msb of pc) loaded from sp+1
    //      sp += 2
    uint8_t second = state->memory[state->sp];
    uint8_t first = state->memory[state->sp + 1];
    reg reg0 = inst->regs[0];
    
    if (reg0 == PSW) {
        state->regs[A] = first;
        state->regs[F] = second;
    }
    else if (reg0 == B) {
        state->regs[B] = first;
        state->regs[C] = second;
    }
    else if (reg0 == D) {
        state->regs[D] = first;
        state->regs[E] = second;
    }
    else if (reg0 == H) {
        state->regs[H] = first;
        state->regs[L] = second;
    }

    state->sp -= 2;
}

void xthl(cpu_state *state, instruction *inst){}
void sphl(cpu_state *state, instruction *inst){}
void pchl(cpu_state *state, instruction *inst){}

void xchg(cpu_state *state, instruction *inst){
    // Exchange HL and DE
    
    uint8_t d_old = state->regs[D];
    uint8_t e_old = state->regs[E];
    state->regs[D] = state->regs[H];
    state->regs[E] = state->regs[L];
    state->regs[H] = d_old;
    state->regs[L] = e_old;
}

#pragma mark - 8 bit arithmetic

void add(cpu_state *state, instruction *inst){}
void sub(cpu_state *state, instruction *inst){}
void inr(cpu_state *state, instruction *inst){}

void dcr(cpu_state *state, instruction *inst){
    uint8_t current_value, next_value;
    reg reg0 = inst->regs[0];
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        current_value = state->memory[address];
        next_value = current_value - 1;
        write_to_mem(state, address, next_value);
    }
    else {
        current_value = state->regs[reg0];
        next_value = current_value - 1;
        state->regs[reg0] = next_value;
    }
    
    set_flagbits_szapc(state,
                 (next_value >> 7) && 1,
                 next_value == 0,
                 0,
                 calc_parity(next_value),
                 next_value > current_value);
}

void cmp(cpu_state *state, instruction *inst){}
void ana(cpu_state *state, instruction *inst){
    reg reg0 = inst->regs[0];
    uint8_t data;
    uint8_t next_a;
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        data = state->memory[address];
        next_a = state->regs[A] & data;
        write_to_mem(state, address, next_a);
    }
    else {
        data = state->regs[reg0];
        next_a = state->regs[A] & data;
        state->regs[reg0] = next_a;
    }
      
    set_flagbits_szapc(state,
                       (next_a >> 7) & 1,
                       next_a == 0,
                       0,
                       calc_parity(next_a),
                       0);
}
void ora(cpu_state *state, instruction *inst){}
void xra(cpu_state *state, instruction *inst){
    reg reg0 = inst->regs[0];
    uint8_t data;
    uint8_t next_a;
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        data = state->memory[address];
        next_a = state->regs[A] ^ data;
        write_to_mem(state, address, next_a);
    }
    else {
        data = state->regs[reg0];
        next_a = state->regs[A] ^ data;
        state->regs[reg0] = next_a;
    }
    
    set_flagbits_szapc(state,
                       (next_a >> 7) & 1,
                       next_a == 0,
                       0,
                       calc_parity(next_a),
                       0);
}

void adi(cpu_state *state, instruction *inst){
    uint8_t data = data_for_instruction(state, inst);
    uint16_t next_a = state->regs[A] + data;
    state->regs[A] = next_a;
    
    set_flagbits_szapc(state,
                       (next_a >> 7) & 1,
                       (next_a & 0xff) == 0,
                       0,
                       calc_parity(next_a),
                       next_a > 0xff);
}
void sui(cpu_state *state, instruction *inst){}
void cpi(cpu_state *state, instruction *inst){
    uint8_t data = data_for_instruction(state, inst);
    
    uint16_t result = state->regs[A] - data;
    set_flagbits_szapc(state,
                       (result >> 7) & 1,
                       (result & 0xff) == 0,
                       0,
                       calc_parity(result),
                       (result >> 8) & 1);
}
void ani(cpu_state *state, instruction *inst){
    uint8_t data = data_for_instruction(state, inst);
    uint16_t next_a = state->regs[A] & data;
    state->regs[A] = next_a;
    
    set_flagbits_szapc(state,
                       (next_a >> 7) & 1,
                       (next_a & 0xff) == 0,
                       0,
                       calc_parity(next_a),
                       0);
}
void ori(cpu_state *state, instruction *inst){}
void xri(cpu_state *state, instruction *inst){}

void daa(cpu_state *state, instruction *inst){}
void adc(cpu_state *state, instruction *inst){}
void aci(cpu_state *state, instruction *inst){}
void sbb(cpu_state *state, instruction *inst){}
void sbi(cpu_state *state, instruction *inst){}

#pragma mark - 16 bit arithmetic

void dad(cpu_state *state, instruction *inst){
    reg reg0 = state->regs[0];
    uint16_t data1 = data_for_regpair(state, reg0);
    uint16_t data2 = data_for_regpair(state, H);  //contents of HL register pair
    uint32_t result = data1 + data2;
    
    state->regs[H] = result >> 8;
    state->regs[L] = result;
    
    set_flagbit(state, (result >> 8) & 1, C_shift);
}
void inx(cpu_state *state, instruction *inst){
    reg reg0 = state->regs[0];
    uint16_t data = data_for_regpair(state, reg0);
    data++;
    uint8_t left = data >> 8;
    uint8_t right = data;
    
    if (reg0 == H) {
        state->regs[H] = left;
        state->regs[L] = right;
    }
    else if (reg0 == B) {
        state->regs[B] = left;
        state->regs[C] = right;
    }
    else if (reg0 == D) {
        state->regs[D] = left;
        state->regs[E] = right;
    }
    else if (reg0 == SP) {
        state->sp = data;
    }
}
void dcx(cpu_state *state, instruction *inst){}

#pragma mark - Jumps, Calls, Returns

void jmp(cpu_state *state, instruction *inst){
    state->pc = data_for_instruction(state, inst);
}

void call(cpu_state *state, instruction *inst){
#warning double check this logic
    uint16_t return_pc = state->pc + inst->num_bytes;  // - 1;
    push_data(state, return_pc);
    state->pc = data_for_instruction(state, inst);
}

void ret(cpu_state *state, instruction *inst){
    state->pc = pop_data(state, 2);
}

void jnz(cpu_state *state, instruction *inst){
    if (!(state->regs[F] & Z_mask)) {
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}

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

#pragma mark - Rotations
void ral(cpu_state *state, instruction *inst){}
void rar(cpu_state *state, instruction *inst){}
void rlc(cpu_state *state, instruction *inst){}

void rrc(cpu_state *state, instruction *inst){
    uint8_t a = state->regs[A];
    uint8_t bit0 = (a << 7) >> 7;
    uint8_t a_next = (a >> 1) | (bit0 << 7);
    state->regs[A] = a_next;
    set_flagbit(state, bit0, C_shift);
}

#pragma mark - Other

void in(cpu_state *state, instruction *inst){}
void out(cpu_state *state, instruction *inst){
#warning do this
}

void cmc(cpu_state *state, instruction *inst){}
void stc(cpu_state *state, instruction *inst){}
void cma(cpu_state *state, instruction *inst){}

void di(cpu_state *state, instruction *inst) {}
void ei(cpu_state *state, instruction *inst) {
#warning do this
}

void rst0(cpu_state *state, instruction *inst){}
void rst1(cpu_state *state, instruction *inst){}
void rst2(cpu_state *state, instruction *inst){}
void rst3(cpu_state *state, instruction *inst){}
void rst4(cpu_state *state, instruction *inst){}
void rst5(cpu_state *state, instruction *inst){}
void rst6(cpu_state *state, instruction *inst){}
void rst7(cpu_state *state, instruction *inst){}

#pragma mark - Assembler directives

void org(cpu_state *state, instruction *inst){}
void end(cpu_state *state, instruction *inst){}
void equ(cpu_state *state, instruction *inst){}

void set(cpu_state *state, instruction *inst){}
void _if(cpu_state *state, instruction *inst){}
void _endif(cpu_state *state, instruction *inst){}
void db(cpu_state *state, instruction *inst){}
void dw(cpu_state *state, instruction *inst){}
void ds(cpu_state *state, instruction *inst){}

#pragma mark - Utility functions

uint16_t data_for_regpair(cpu_state *state, reg regpair) {
    uint16_t data = 0;
    
    if (regpair == B) {  // BC
        data = (state->regs[B] << 8) | state->regs[C];
    }
    else if (regpair == D) {  // DE
        data = (state->regs[D] << 8) | state->regs[E];
    }
    else if (regpair == H) {  // HL
        data = read_from_HL(state);
    }
    else if (regpair == PSW) { // PSW
        data = (state->regs[A] << 8) | state->regs[F];
    }
    return data;
}

uint16_t data_for_instruction(cpu_state *state, instruction *inst) {
    
    uint16_t data = 0;
    
    for (int i = 1; i < inst->num_bytes; i++) {
        data |= ((unsigned char)state->memory[state->pc+i]) << ((i-1) * 8);
    }
    
    return data;
}

static uint16_t reverse2bytes(uint16_t data) {
    return data | ((data >> 8) & 0xff);
}

static int is_allowed_to_write_here(cpu_state *state, uint16_t offset) {
    return offset > 0x2000 && offset < 0x4000;
}

static void write_to_mem(cpu_state *state, uint16_t offset, uint16_t data) {
    if (is_allowed_to_write_here(state, offset)) {
        state->memory[offset] = data;
    }
}

static uint16_t read_from_HL(cpu_state *state) {
    return ((state->regs[H] << 8) | state->regs[L]);
}

static uint16_t pop_data(cpu_state *state, uint8_t num_bytes) {
    uint16_t data = 0;
    
    for (uint8_t i = 0; i < num_bytes; i++) {
        data |= state->memory[state->sp + i] << (i*8);
        state->pc++;
    }
    
    return data;
}

static void push_data(cpu_state *state, uint16_t data) {
    uint8_t msb = (uint8_t)data;  // if data is from reg_pair, this is second reg
    uint8_t lsb = (uint8_t)(data >> 8);  // first reg
    
    state->memory[state->sp - 1] = lsb;
    state->memory[state->sp - 2] = msb;
    state->sp -= 2;
}

uint8_t is_not_pc_changing_inst(instruction *inst) {
    instruction_t t = inst->inst_t;
    
    return  t == JMP    || t == CALL    || t == RET ||
            t == JNZ    || t == CNZ     || t == RNZ ||
            t == JZ     || t == CZ      || t == RZ  ||
            t == JNC    || t == CNC     || t == RNC ||
            t == JC     || t == CC      || t == RC  ||
            t == JPO    || t == CPO     || t == RPO ||
            t == JPE    || t == CPE     || t == RPE ||
            t == JP     || t == CP      || t == RP  ||
            t == JM     || t == CM      || t == RM;
}



static uint8_t calc_parity(uint16_t data) {
    uint8_t num_ones = 0;
    
    for (uint8_t i = 0; i < 16; i++) {
        num_ones += data & 1;
        data >>= 1;
    }
    return num_ones % 2 ? 0 : 1;  // 0 for odd parity, 1 for even parity 
}

static void set_flagbits_szapc(cpu_state *state, uint8_t s, uint8_t z, uint8_t a, uint8_t p, uint8_t c) {
    set_flagbit(state, s, S_shift);
    set_flagbit(state, z, Z_shift);
    set_flagbit(state, a, A_shift);
    set_flagbit(state, p, P_shift);
    set_flagbit(state, c, C_shift);
}

static void set_flagbit(cpu_state *state, uint8_t bit, uint8_t bit_shift) {
    state->regs[F] &= ~(1 << bit_shift);  // clear the bit
    state->regs[F] |= bit << bit_shift;  // set it to "bit"
}

uint8_t get_flagbit(cpu_state *state, uint8_t bit_shift) {
    return (state->regs[F] & (1 << bit_shift)) >> bit_shift;
}