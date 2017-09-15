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

#pragma mark - helper function declarationss

static int is_allowed_to_write_here(cpu_state *state, uint16_t offset);
static void write_to_mem(cpu_state *state, uint16_t offset, uint8_t data);
//static void write_to_HL(cpu_state *state, uint16_t data);
static uint16_t read_from_HL(cpu_state *state);

//void push_data(cpu_state *state, uint16_t data);
//uint16_t pop_data(cpu_state *state, uint8_t num_bytes);
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
        state->regs[reg0] = state->regs[reg1];
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

void stax(cpu_state *state, instruction *inst){
    //printf("not implemented1");
    reg reg0 = inst->regs[0];
    uint16_t offset = data_for_regpair(state, reg0);
    write_to_mem(state, offset, state->regs[A]);
}

#pragma mark - 16 bit transfers

#warning
void lhld(cpu_state *state, instruction *inst){
    //printf("not implemented2");
    uint16_t offset = data_for_instruction(state, inst);
    state->regs[L] = state->memory[offset];
    state->regs[H] = state->memory[offset+1];
}

void shld(cpu_state *state, instruction *inst){
    //printf("shld\n");
    uint16_t address = data_for_instruction(state, inst);
    write_to_mem(state, address, state->regs[L]);
    write_to_mem(state, address+1, state->regs[H]);
}

void lxi(cpu_state *state, instruction *inst) {
    reg reg0 = inst->regs[0];
    uint16_t data = data_for_instruction(state, inst);
    
    // NOTE: lsb is the leftmost and msb is the rightmost
    
    uint8_t lo = data & 0xff; // lo
    uint8_t hi = (data >> 8); // hi. [lsb msb]
    
    if (reg0 == B) { // BC register pair
        state->regs[B] = hi;
        state->regs[C] = lo;
    }
    else if (reg0 == D) { // DE reg pair
        state->regs[D] = hi;
        state->regs[E] = lo;
    }
    else if (reg0 == H) { // HL reg pair
        state->regs[H] = hi;
        state->regs[L] = lo;
    }
    else if (reg0 == SP) {
        state->sp = (hi << 8) | lo;
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

    state->sp += 2;
}

#warning
void xthl(cpu_state *state, instruction *inst){
    //printf("not implemented3");
    uint8_t h = state->regs[H];
    uint8_t l = state->regs[L];
    state->regs[L] = state->memory[state->sp];
    state->regs[H] = state->memory[state->sp+1];
    write_to_mem(state, state->sp, l);
    write_to_mem(state, state->sp+1, h);
}
void sphl(cpu_state *state, instruction *inst){printf("not implemented4");}
#warning
void pchl(cpu_state *state, instruction *inst){
    //printf("not implemented5");
    state->pc = read_from_HL(state);
}

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

void add(cpu_state *state, instruction *inst){
    reg reg0 = inst->regs[0];
    uint8_t result = state->regs[A] + state->regs[reg0];
    state->regs[A] = result;
    set_flagbits_szapc(state,
                       (result >> 7) & 1,
                       result == 0,
                       0,
                       calc_parity(result),
                       result < state->regs[A]);
    state->regs[A] = result;
    //printf("not implemented6");
}

void sub(cpu_state *state, instruction *inst){printf("not implemented7");}
#warning
void inr(cpu_state *state, instruction *inst){
    //printf("not implemented8");
    uint8_t current_value, next_value;
    reg reg0 = inst->regs[0];
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        current_value = state->memory[address];
        next_value = current_value + 1;
        write_to_mem(state, address, next_value);
    }
    else {
        current_value = state->regs[reg0];
        next_value = current_value + 1;
        state->regs[reg0] = next_value;
    }
    
    set_flagbits_szapc(state,
                       (next_value >> 7) && 1,
                       next_value == 0,
                       0,
                       calc_parity(next_value),
                       next_value > current_value);
}

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

void cmp(cpu_state *state, instruction *inst){
    reg cmp_reg = inst->regs[0];
    uint8_t cmp_data = 0;
    /*switch (cmp_reg) {
        case H:
            cmp_data = data_for_regpair(state, H);
            break;
        default:
            cmp_data = state->regs[cmp_reg];
            break;
    }*/
    cmp_data = state->regs[cmp_reg];
    
    uint16_t result = state->regs[A] - cmp_data;
    set_flagbits_szapc(state,
                       (result >> 7) & 1,
                       (result & 0xff) == 0,
                       0,
                       calc_parity(result),
                       (result >> 8) & 1);
    
}
void ana(cpu_state *state, instruction *inst){
    reg reg0 = inst->regs[0];
    uint8_t data;
    uint8_t next_a;
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        data = state->memory[address];
        next_a = state->regs[A] & data;
        //write_to_mem(state, address, next_a);
        state->regs[A] = next_a;
    }
    else {
        data = state->regs[reg0];
        next_a = state->regs[A] & data;
        state->regs[A] = next_a;
    }
      
    set_flagbits_szapc(state,
                       (next_a >> 7) & 1,
                       next_a == 0,
                       0,
                       calc_parity(next_a),
                       0);
}
#warning
void ora(cpu_state *state, instruction *inst){
    //printf("not implemented9");
    reg reg0 = inst->regs[0];
    uint8_t data;
    uint8_t next_a;
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        data = state->memory[address];
        next_a = state->regs[A] | data;
        state->regs[A] = next_a;
    }
    else {
        data = state->regs[reg0];
        next_a = state->regs[A] | data;
        state->regs[A] = next_a;
    }
    
    set_flagbits_szapc(state,
                       (next_a >> 7) & 1,
                       next_a == 0,
                       0,
                       calc_parity(next_a),
                       0);
}

void xra(cpu_state *state, instruction *inst){
    reg reg0 = inst->regs[0];
    uint8_t data;
    uint8_t next_a;
    
    if (reg0 == M) {
        uint16_t address = read_from_HL(state);
        data = state->memory[address];
        next_a = state->regs[A] ^ data;
        //write_to_mem(state, address, next_a);
        state->regs[A] = next_a;
    }
    else {
        data = state->regs[reg0];
        next_a = state->regs[A] ^ data;
        state->regs[A] = next_a;
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
void sui(cpu_state *state, instruction *inst){printf("not implemented0");}
void cpi(cpu_state *state, instruction *inst){
    uint8_t data = data_for_instruction(state, inst);
    
    uint16_t result = state->regs[A] - data;
    // Carry bit set if a < data
    set_flagbits_szapc(state,
                       (result >> 7) & 1,
                       (result & 0xff) == 0,
                       0,
                       calc_parity(result),
                       state->regs[A] < data);
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
#warning
void ori(cpu_state *state, instruction *inst){
    //printf("not implementeda");
    uint8_t data = data_for_instruction(state, inst);
    uint8_t x = state->regs[A] | data;
    state->regs[A] = x;
    set_flagbits_szapc(state,
                       (x >> 7) & 1,
                       (x & 0xff) == 0,
                       0,
                       calc_parity(x),
                       0);
}
void xri(cpu_state *state, instruction *inst){printf("not implementedb");}

void daa(cpu_state *state, instruction *inst){
    if ((state->regs[A] & 0xf) > 9) {
        state->regs[A] += 6;
    }
    if ((state->regs[A] & 0xf0) > 0x90)
    {
        uint16_t res = (uint16_t) state->regs[A] + 0x60;
        set_flagbits_szapc(state,
                           0x80 == (res & 0x80),
                           (res&0xff) == 0,
                           0,
                           calc_parity(res&0xff),
                           res > 0xff);
        state->regs[A] = res&0xff;
    }
    //printf("not implementedc");
}

void adc(cpu_state *state, instruction *inst){
    //printf("not implementedd");
    reg reg0 = inst->regs[0];
    uint16_t c = get_flagbit(state, C_shift);
    uint16_t other = 0;
    
    if (reg0 == M) {
        other = read_from_HL(state);
    } else {
        other = state->regs[reg0];
    }
    
    uint16_t res = state->regs[A] + other + c;
    state->regs[A]= (res & 0xff);
    
    set_flagbits_szapc(state,
                       0x80 == (res & 0x80),
                       (res&0xff) == 0,
                       0,
                       calc_parity(res&0xff),
                       res > 0xff);
}

void aci(cpu_state *state, instruction *inst){printf("not implementede");}
void sbb(cpu_state *state, instruction *inst){printf("not implementedf");}
void sbi(cpu_state *state, instruction *inst){
    //printf("not implementedg");
    uint8_t data = data_for_instruction(state, inst);
    uint8_t c = get_flagbit(state, C_shift);
    uint16_t res = state->regs[A] - data - c;
    state->regs[A] = res & 0xff;
    set_flagbits_szapc(state,
                       0x80 == (res & 0x80),
                       (res&0xff) == 0,
                       0,
                       calc_parity(res&0xff),
                       res > 0xff);
}

#pragma mark - 16 bit arithmetic

void dad(cpu_state *state, instruction *inst){
    reg reg0 = inst->regs[0];
    uint16_t data1 = data_for_regpair(state, reg0);
    uint16_t data2 = data_for_regpair(state, H);  //contents of HL register pair
    uint32_t result = data1 + data2;
    
    state->regs[H] = (result & 0xff00) >> 8;
    state->regs[L] = result & 0xff;
    
    set_flagbit(state, (result & 0xffff0000) != 0, C_shift);
}
void inx(cpu_state *state, instruction *inst){
    reg reg0 = inst->regs[0];
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
#warning
void dcx(cpu_state *state, instruction *inst){
    //printf("not implementedh");
    reg reg0 = inst->regs[0];
    uint16_t data = data_for_regpair(state, reg0) - 1;
    
    if (reg0 == B) {
        state->regs[B] = (data & 0xff00) >> 8;
        state->regs[C] = data & 0xff;
    } else if (reg0 == H) {
        state->regs[H] = (data & 0xff00) >> 8;
        state->regs[L] = data & 0xff;
    } else if (reg0 == D) {
        state->regs[D] = (data & 0xff00) >> 8;
        state->regs[E] = data & 0xff;
    } else if (reg0 == SP) {
        state->sp -= 1;
    }
}

#pragma mark - Jumps, Calls, Returns

void jmp(cpu_state *state, instruction *inst){
    state->pc = data_for_instruction(state, inst);
}

void call(cpu_state *state, instruction *inst){
#warning double check this logic
    uint16_t return_pc = state->pc + inst->num_bytes; // - 1;
    push_data(state, return_pc);
    state->pc = data_for_instruction(state, inst);
}

void ret(cpu_state *state, instruction *inst){
    state->pc = pop_data(state, 2);
}

void jnz(cpu_state *state, instruction *inst){
    if (!get_flagbit(state, Z_shift)) {
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}

#warning
void cnz(cpu_state *state, instruction *inst){
    //printf("not implementedi");
    if (!get_flagbit(state, Z_shift)) {
        uint16_t return_pc = state->pc + inst->num_bytes; // - 1;
        push_data(state, return_pc);
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void rnz(cpu_state *state, instruction *inst){
    if (!get_flagbit(state, Z_shift)) {
        state->pc = pop_data(state, 2);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void jz(cpu_state *state, instruction *inst){
    if (get_flagbit(state, Z_shift)) {
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void cz(cpu_state *state, instruction *inst){
    //printf("not implementedj");
    if (get_flagbit(state, Z_shift)) {
        uint16_t return_pc = state->pc + inst->num_bytes; // - 1;
        push_data(state, return_pc);
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void rz(cpu_state *state, instruction *inst){
    if (get_flagbit(state, Z_shift)) {
        state->pc = pop_data(state, 2);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void jnc(cpu_state *state, instruction *inst){
    if (!get_flagbit(state, C_shift)) {
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void cnc(cpu_state *state, instruction *inst){
    if (!get_flagbit(state, C_shift)) {
        uint16_t return_pc = state->pc + inst->num_bytes;
        push_data(state, return_pc);
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
#warning DOIT
void rnc(cpu_state *state, instruction *inst){
    //printf("not implementedk");
    if (!get_flagbit(state, C_shift)) {
        state->pc = pop_data(state, 2);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void jc(cpu_state *state, instruction *inst){
    if (get_flagbit(state, C_shift)) {
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void cc(cpu_state *state, instruction *inst){printf("not implementedl");}
void rc(cpu_state *state, instruction *inst){
    if (get_flagbit(state, C_shift)) {
        state->pc = pop_data(state, 2);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
#warning
void jpo(cpu_state *state, instruction *inst){
    //printf("not implementedm");
    if (!get_flagbit(state, P_shift)) {   // parity of 0 is odd
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void cpo(cpu_state *state, instruction *inst){printf("not implementedn");}
#warning
void rpo(cpu_state *state, instruction *inst){
    //printf("not implementedo");
    if (!get_flagbit(state, P_shift)) {   // parity of 0 is odd
        state->pc = pop_data(state, 2);
    }
    else {
        state->pc += inst->num_bytes;
    }
}

void jpe(cpu_state *state, instruction *inst){printf("not implementedp");}
void cpe(cpu_state *state, instruction *inst){
    if (get_flagbit(state, P_shift)) {   // parity of 1 is even
        uint16_t return_pc = state->pc + inst->num_bytes;
        push_data(state, return_pc);
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void rpe(cpu_state *state, instruction *inst){printf("not implementedq");}
void jp(cpu_state *state, instruction *inst){printf("not implementedr");}
void cp(cpu_state *state, instruction *inst){
    if (!get_flagbit(state, S_shift)) {   // sign off is plus
        uint16_t return_pc = state->pc + inst->num_bytes;
        push_data(state, return_pc);
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void rp(cpu_state *state, instruction *inst){
    if (!get_flagbit(state, S_shift)) {  // If sign bit is on, it is negative
        state->pc = pop_data(state, 2);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
#warning
void jm(cpu_state *state, instruction *inst){
    //printf("not implementeds");
    if (get_flagbit(state, S_shift)) {  // sign on is minus
        state->pc = data_for_instruction(state, inst);
    } else {
        state->pc += inst->num_bytes;
    }
}
void cm(cpu_state *state, instruction *inst){
    if (get_flagbit(state, S_shift)) {   // sign on is minus
        uint16_t return_pc = state->pc + inst->num_bytes;
        push_data(state, return_pc);
        state->pc = data_for_instruction(state, inst);
    }
    else {
        state->pc += inst->num_bytes;
    }
}
void rm(cpu_state *state, instruction *inst){
    //printf("not implementedt");
    if (get_flagbit(state, S_shift)) {  // If sign bit is on, it is negative
        state->pc = pop_data(state, 2);
    }
    else {
        state->pc += inst->num_bytes;
    }
}

#pragma mark - Rotations
#warning
void ral(cpu_state *state, instruction *inst){
    //printf("not implementedu");
    uint8_t x = state->regs[A];
    state->regs[A] = get_flagbit(state, C_shift)  | (x << 1);
    set_flagbit(state, 0x80 == (x & 0x80), C_shift);
}

#warning
void rar(cpu_state *state, instruction *inst){
    //printf("not implementedv");
    uint8_t x = state->regs[A];
    uint8_t c = get_flagbit(state, C_shift);
    state->regs[A] = (c << 7) | (x >> 1);
    set_flagbit(state, 1 == (x&1), C_shift);
}

#warning
void rlc(cpu_state *state, instruction *inst){
    //printf("not implementedw");
    uint8_t x = state->regs[A];
    state->regs[A] = ((x & 0x80) >> 7) | (x << 1);
    set_flagbit(state, 0x80 == (x & 0x80), C_shift);
}

void rrc(cpu_state *state, instruction *inst){
    uint8_t a = state->regs[A];
    uint8_t bit0 = (a << 7) >> 7;
    uint8_t a_next = (a >> 1) | (bit0 << 7);
    state->regs[A] = a_next;
    set_flagbit(state, bit0, C_shift);
}

#pragma mark - Other

#warning The in and out functions are hardcoded strategies for Space Invaders. It should instead be generalized for any program.
// Read from http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html for more information on the shift hardware
void in_inst(cpu_state *state, instruction *inst){
    uint8_t port = data_for_instruction(state, inst);
    uint16_t a = 0;
    
    switch (port) {
        case 0:
            a = 1;
            break;
        case 1:
            a = state->ports[1];
            break;
        case 3:
            a = (state->shift_left_byte << 8) | state->shift_right_byte;
            a >>= (8 - state->shift_offset) & 0xff;
            break;
        default:
            return;  // don't set regs[A] when it shouldn't be changed
    }
    
    state->regs[A] = a;
}
void out_inst(cpu_state *state, instruction *inst){
    uint8_t data = state->regs[A];
    uint8_t port = data_for_instruction(state, inst);
    //printf("out\n");
    switch (port) {
        case 2:  // Port 2 sets the shift amount
            state->shift_offset = data & 0x7;
            break;
        case 4:  // Port 4 writes a byte to the 16 bit shift register
            //printf("4\n");
            state->shift_right_byte = state->shift_left_byte;
            state->shift_left_byte = data;// & 0x7;
            break;
    }
}

void cmc(cpu_state *state, instruction *inst){printf("not implementedx");}
#warning
void stc(cpu_state *state, instruction *inst){
    //printf("not implementedy");
    set_flagbit(state, 1, C_shift);
}

void cma(cpu_state *state, instruction *inst){
    //printf("not implementedz");
    state->regs[A] = ~state->regs[A];
}

void di(cpu_state *state, instruction *inst) {
    state->interrupt_enable = 0;
}
void ei(cpu_state *state, instruction *inst) {
    state->interrupt_enable = 1;
}

void rst(cpu_state *state, instruction *inst, uint8_t interrupt_num){
    //printf("rst%d\n", interrupt_num);
    push_data(state, state->pc);
    state->pc = 8 * interrupt_num;
}

void rst0(cpu_state *state, instruction *inst){ rst(state, inst, 0);}
void rst1(cpu_state *state, instruction *inst){ rst(state, inst, 1);}
void rst2(cpu_state *state, instruction *inst){ rst(state, inst, 2);}
void rst3(cpu_state *state, instruction *inst){ rst(state, inst, 3);}
void rst4(cpu_state *state, instruction *inst){ rst(state, inst, 4);}
void rst5(cpu_state *state, instruction *inst){ rst(state, inst, 5);}
void rst6(cpu_state *state, instruction *inst){ rst(state, inst, 6);}
void rst7(cpu_state *state, instruction *inst){ rst(state, inst, 7);}

#pragma mark - Assembler directives

void org(cpu_state *state, instruction *inst){printf("not implementedA");}
void end(cpu_state *state, instruction *inst){printf("not implementedB");}
void equ(cpu_state *state, instruction *inst){printf("not implementedC");}

void set(cpu_state *state, instruction *inst){printf("not implementedD");}
void _if(cpu_state *state, instruction *inst){printf("not implementedE");}
void _endif(cpu_state *state, instruction *inst){printf("not implementedF");}
void db(cpu_state *state, instruction *inst){printf("not implementedG");}
void dw(cpu_state *state, instruction *inst){printf("not implementedH");}
void ds(cpu_state *state, instruction *inst){printf("not implementedI");}

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

/* Returns the 1 or 2 byte value for an instruction. If the value is 2 bytes, then interpret the 
 2 byte value as [hi, lo]. This function will take care of the little endiannes of the instruction*/
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
    if (offset < 0x2000) {
        printf("there\n");
        return 0;
    }
    if (offset >= 0x4000) {
        printf("here\n");
        return 0;
    }
    return 1;
    //return offset >= 0x2000 && offset < 0x4000;
}

/* Writes 1 byte to memory */
static void write_to_mem(cpu_state *state, uint16_t offset, uint8_t data) {
    if (is_allowed_to_write_here(state, offset)) {
        state->memory[offset] = data;
    }
}

static uint16_t read_from_HL(cpu_state *state) {
    return ((state->regs[H] << 8) | state->regs[L]);
}

uint16_t pop_data(cpu_state *state, uint8_t num_bytes) {
    uint16_t data = 0;
    
    for (uint8_t i = 0; i < num_bytes; i++) {
        data |= state->memory[state->sp + i] << (i * 8);
        //state->pc++;
    }
    
    state->sp += num_bytes;
    return data;
}

/* Pushes 2 bytes of data onto the stack in little endian order */
void push_data(cpu_state *state, uint16_t data) {
    uint8_t msb = (uint8_t)data;  // if data is from reg_pair, this is second reg
    uint8_t lsb = (uint8_t)(data >> 8);  // first reg
    
    write_to_mem(state, state->sp - 1, lsb);
    write_to_mem(state, state->sp - 2, msb);
    state->sp -= 2;
}

uint8_t is_pc_changing_inst(instruction *inst) {
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
