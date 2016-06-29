#include <stdio.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

instruction fetch_decode (struct cpu_state *state) { 
	instruction inst;
	unsigned char byte = state->memory[state->pc++];
	inst = inst_lookup[byte];
	return inst;
}

void emulate () {
	//TODO: listen for io (multithreaded???) 
}
