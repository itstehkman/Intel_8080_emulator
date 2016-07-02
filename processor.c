#include <stdio.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

void print_inst(cpu_state *state, instruction *inst);

instruction fetch_decode(cpu_state *state) { 
	unsigned char byte = state->memory[state->pc];
	instruction inst = inst_lookup[byte];
	state->pc += inst.num_bytes;
	return inst;
}

void emulate_cycle(cpu_state *state) {
	instruction inst = fetch_decode(state);
	inst.inst_func(state, &inst);
	print_inst(state, &inst);
}

void cycle(cpu_state *state, void (*emulate_func)(cpu_state *state)) {
	clock_t start = clock();

	emulate_func(state);  // The meat of the program

	clock_t elapsed = 1E9 * ((float)(clock() - start)) / CLOCKS_PER_SEC;
	time_since_sleep += elapsed;
	
	long sleep_candidate = NSECS_PER_CYCLE * inst_since_sleep - time_since_sleep;
	if (sleep_candidate > 0) {
		struct timespec time = {0, sleep_candidate};
		nanosleep(&time, NULL);
		time_since_sleep = 0;
		inst_since_sleep = 1;
		printf("sleeping for %lu ns\n", sleep_candidate);
	} 
	else {
		inst_since_sleep++;
		printf("$1: %hu 2: %lu\n", inst_since_sleep, time_since_sleep);
	}
}
