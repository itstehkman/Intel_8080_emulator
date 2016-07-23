#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs_and_consts.h"

cpu_state cpu_state_from_rom_file(const char* rom_path) {
    
    cpu_state state;
    
    if (!load_rom(&state, rom_path)) {
        fprintf(stderr, "Error reading ROM - either file doesn't exist"
                " or error reading memory\n");
        exit(1);
    }
    
    state.pc = ROM_START;
    state.sp = 0;
    
    return state;
}

char load_rom(struct cpu_state *state, const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) {
        return 0;
    }
    
    fseek(f, 0, SEEK_END);
    unsigned int size = (unsigned int)ftell(f);
    fseek(f, 0, SEEK_SET);
    
    fread(&state->memory[ROM_START], 1, size, f);
    state->rom_size = size;
    
    fclose(f);
    return 1;
}
