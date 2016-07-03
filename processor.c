#include <stdio.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

void print_inst(cpu_state *state, instruction *inst);

instruction fetch_decode(cpu_state *state) { 
	unsigned char byte = state->memory[state->pc];
	instruction inst = inst_lookup[byte];
	return inst;
}

// Returns the number of cycles to stall for
unsigned short emulate_inst_and_get_num_cycles(cpu_state *state) {
	instruction inst = fetch_decode(state);
	inst.inst_func(state, &inst);
	//print_inst(state, &inst);
	
	/* Ordered steps to a single emulation "step":
		set cpu states io pins based on what user presses
		run the instruction
		output graphics (python)
		play the sounds (python)

	*/
	state->pc += inst.num_bytes;
	return inst.num_cycles;
}

void lineup_in_cycle(cpu_state *state, unsigned short (*emulate_func)(cpu_state *state)) {
	clock_t start = clock();

	unsigned short num_cycles = emulate_func(state);  // The meat of the program

	clock_t elapsed = 1E9 * ((float)(clock() - start)) / CLOCKS_PER_SEC;
	struct timespec time = {0, num_cycles * NSECS_PER_CYCLE - 
		(elapsed % NSECS_PER_CYCLE)};

	// if time.tv_nsec isn't already the right stall time
	if ((time.tv_nsec % NSECS_PER_CYCLE)) {
		nanosleep(&time, NULL);
		printf("%lu %lu \n", time.tv_nsec, elapsed); 
	}
}
