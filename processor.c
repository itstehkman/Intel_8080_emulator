#include <stdio.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

void print_inst(cpu_state *state, instruction *inst);

void run_cpu(cpu_state *state) {
    while (1) {
        lineup_in_cycle(state, emulate_inst_and_get_num_cycles);
    }
}

instruction fetch_decode(cpu_state *state) {
    uint8_t byte = state->memory[state->pc];
    instruction inst = inst_lookup[byte];
    return inst;
}

// Returns the number of cycles to stall for
uint16_t emulate_inst_and_get_num_cycles(cpu_state *state) {
    
    instruction inst = fetch_decode(state);
#if DEBUG_ON
    print_inst(state, &inst);
#endif
    
    inst.inst_func(state, &inst);
    
    if (is_not_pc_changing_inst(&inst)) {
        state->pc += inst.num_bytes;
    }
    
#if DEBUG_ON
    print_state(state);
    printf("\n");
#endif
    return inst.num_cycles;
}

void lineup_in_cycle(cpu_state *state, unsigned short (*emulate_func)(cpu_state *state)) {
    clock_t start = clock();
    
    unsigned short num_cycles = emulate_func(state);  // The meat of the program
    
    clock_t elapsed_nsecs = 1E9 * ((float)(clock() - start)) / CLOCKS_PER_SEC;
    clock_t expected_inst_time = num_cycles * NSECS_PER_CYCLE;
    struct timespec stall_time = {0, expected_inst_time - (elapsed_nsecs % NSECS_PER_CYCLE)};
    
    
    /*if (elapsed_nsecs > expected_inst_time) {
        printf("%ld\n", elapsed_nsecs - expected_inst_time);
       // exit(1);
    }*/
    
    // if time.tv_nsec isn't already the right stall time, then stall
    if ((stall_time.tv_nsec % NSECS_PER_CYCLE) != 0) {
        nanosleep(&stall_time, NULL);
        printf("%lu %lu \n", stall_time.tv_nsec, elapsed_nsecs);
    }
}