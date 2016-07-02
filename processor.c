#include <stdio.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

void print_inst(cpu_state *state, instruction *inst);

instruction fetch_decode (cpu_state *state) { 
	instruction inst;
	unsigned char byte = state->memory[state->pc];
	inst = inst_lookup[byte];
	inst.inst_func(state, &inst);
	print_inst(state, &inst);
	state->pc += inst.num_bytes;
	return inst;
}

void emulate () {
	//TODO: listen for io (multithreaded???) 
}
