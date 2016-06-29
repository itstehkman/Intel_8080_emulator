#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

// Courtesy of http://fms.komkon.org/comp/CPUs/8080.txt for
// list of instruction types

void nop(cpu_state *state, instruction *inst);
void hlt(cpu_state *state, instruction *inst);

// 8 bit transfers
void mov(cpu_state *state, instruction *inst);
void mvi(cpu_state *state, instruction *inst);
void lda(cpu_state *state, instruction *inst);
void sta(cpu_state *state, instruction *inst);
void ldax(cpu_state *state, instruction *inst);
void stax(cpu_state *state, instruction *inst);

// 16 bit transfers
void lhld(cpu_state *state, instruction *inst);
void shld(cpu_state *state, instruction *inst);
void lxi(cpu_state *state, instruction *inst);
void push(cpu_state *state, instruction *inst);
void pop(cpu_state *state, instruction *inst);
void xthl(cpu_state *state, instruction *inst);
void sphl(cpu_state *state, instruction *inst);
void pchl(cpu_state *state, instruction *inst);
void xchg(cpu_state *state, instruction *inst);

// 8 bit arithmetic
void add(cpu_state *state, instruction *inst);
void sub(cpu_state *state, instruction *inst);
void inr(cpu_state *state, instruction *inst);
void dcr(cpu_state *state, instruction *inst);
void cmp(cpu_state *state, instruction *inst);
void ana(cpu_state *state, instruction *inst);
void ora(cpu_state *state, instruction *inst);
void xra(cpu_state *state, instruction *inst);

void adi(cpu_state *state, instruction *inst);
void sui(cpu_state *state, instruction *inst);
void cpi(cpu_state *state, instruction *inst);
void ani(cpu_state *state, instruction *inst);
void ori(cpu_state *state, instruction *inst);
void xri(cpu_state *state, instruction *inst);

void daa(cpu_state *state, instruction *inst);
void adc(cpu_state *state, instruction *inst);
void aci(cpu_state *state, instruction *inst);
void sbb(cpu_state *state, instruction *inst);
void sbi(cpu_state *state, instruction *inst);

// 16 bit arithmetic
void dad(cpu_state *state, instruction *inst);
void inx(cpu_state *state, instruction *inst);
void dcx(cpu_state *state, instruction *inst);

// Jumps, calls, returns
void jmp(cpu_state *state, instruction *inst);
void call(cpu_state *state, instruction *inst);
void ret(cpu_state *state, instruction *inst);

void jnz(cpu_state *state, instruction *inst);
void cnz(cpu_state *state, instruction *inst);
void rnz(cpu_state *state, instruction *inst);
void jz(cpu_state *state, instruction *inst);
void cz(cpu_state *state, instruction *inst);
void rz(cpu_state *state, instruction *inst);
void jnc(cpu_state *state, instruction *inst);
void cnc(cpu_state *state, instruction *inst);
void rnc(cpu_state *state, instruction *inst);
void jc(cpu_state *state, instruction *inst);
void cc(cpu_state *state, instruction *inst);
void rc(cpu_state *state, instruction *inst);
void jpo(cpu_state *state, instruction *inst);
void cpo(cpu_state *state, instruction *inst);
void rpo(cpu_state *state, instruction *inst);
void jpe(cpu_state *state, instruction *inst);
void cpe(cpu_state *state, instruction *inst);
void rpe(cpu_state *state, instruction *inst);
void jp(cpu_state *state, instruction *inst);
void cp(cpu_state *state, instruction *inst);
void rp(cpu_state *state, instruction *inst);
void jm(cpu_state *state, instruction *inst);
void cm(cpu_state *state, instruction *inst);
void rm(cpu_state *state, instruction *inst);

// Rotations
void ral(cpu_state *state, instruction *inst);
void rar(cpu_state *state, instruction *inst);
void rlc(cpu_state *state, instruction *inst);
void rrc(cpu_state *state, instruction *inst);

// Other
void in(cpu_state *state, instruction *inst);
void out(cpu_state *state, instruction *inst);

void cmc(cpu_state *state, instruction *inst);
void stc(cpu_state *state, instruction *inst);
void cma(cpu_state *state, instruction *inst);

void di(cpu_state *state, instruction *inst);
void ei(cpu_state *state, instruction *inst);

void rst0(cpu_state *state, instruction *inst);
void rst1(cpu_state *state, instruction *inst);
void rst2(cpu_state *state, instruction *inst);
void rst3(cpu_state *state, instruction *inst);
void rst4(cpu_state *state, instruction *inst);
void rst5(cpu_state *state, instruction *inst);
void rst6(cpu_state *state, instruction *inst);
void rst7(cpu_state *state, instruction *inst);

// Assembler directives
void org(cpu_state *state, instruction *inst);
void end(cpu_state *state, instruction *inst);
void equ(cpu_state *state, instruction *inst);

void set(cpu_state *state, instruction *inst);
void _if(cpu_state *state, instruction *inst);
void _endif(cpu_state *state, instruction *inst);
void db(cpu_state *state, instruction *inst);
void dw(cpu_state *state, instruction *inst);
void ds(cpu_state *state, instruction *inst);

#endif
