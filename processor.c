#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#include "structs_and_consts.h"
#include "instruction_lookup.h"

void print_inst(cpu_state *state, instruction *inst);
clock_t time_usec(void);

static uint8_t DEBUG_MODE = 1;
void set_debug_mode(uint8_t mode) {
    DEBUG_MODE = 1;
}

void run_cpu(cpu_state *state) {
    while (1) {  // A HLT instruction will stop the program if necessary
        lineup_with_cpu_rate(state, emulate_inst_and_get_num_cycles);
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
    
    if (DEBUG_MODE) {
        print_inst(state, &inst);
    }
    
    inst.inst_func(state, &inst);
    
    if (!is_pc_changing_inst(&inst)) {
        state->pc += inst.num_bytes;
    }
    
    if (DEBUG_MODE) {
        print_state(state);
        printf("\n");
    }
    return inst.num_cycles;
}

clock_t time_usec(void) {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1E6) + (time.tv_usec);  // doing this for each instruction is quite expensive
}

static clock_t last_inst_finish_usec = 0;
static unsigned long total_inst_fast_enough = 0;
static unsigned long total_inst_not_fast_enough = 0;
static float percent_too_slow = 0;

// Emulating an instruction might be slower than expected, so we must make sure that everything is lined up
void lineup_with_cpu_rate(cpu_state *state, unsigned short (*emulate_func)(cpu_state *state)) {
    if (!last_inst_finish_usec) {
        last_inst_finish_usec = time_usec();  // So that we can populate a value for the first instruction
    }
    
    unsigned short num_cycles = emulate_func(state);  // The meat of the program
    clock_t elapsed_usec = time_usec() - last_inst_finish_usec;
    
    clock_t expected_inst_time_usec = ceil(num_cycles * USECS_PER_CYCLE);  // Could be x.5 usecs, but we are just truncating for now
    int stall_time_usec = expected_inst_time_usec - elapsed_usec;
    
    // if it isn't already the right stall-time, then stall
    if (stall_time_usec >= 0) {
        usleep(stall_time_usec);
        total_inst_fast_enough++;
    } else {
        total_inst_not_fast_enough++;
    }
    
    last_inst_finish_usec = time_usec();
    
    // percent_too_slow = 100.0 * (float)(total_inst_not_fast_enough) / (total_inst_fast_enough + total_inst_not_fast_enough);
}