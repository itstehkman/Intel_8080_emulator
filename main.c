#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs_and_consts.h" 

int main (int argc, const char *argv[]) {
  if (argc != NUM_ARGS){
    fprintf(stderr, "usage: %s <rom-file>\n", argv[0]);
    exit(1);
  }
	
  cpu_state state;

	if (!load_rom(&state, argv[1])) {  
		fprintf(stderr, "Error reading ROM - either file doesn't exist \
		or error reading memory");
		exit(1);
	}

	state.pc = ROM_START;
	state.sp = 0;

	for (int i = 0; i < state.rom_size; i++) {
		fetch_decode(&state);
	}

  return 0;

}

char load_rom (struct cpu_state *state, const char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (!f) {
    return 0;
	}

  fseek(f, 0, SEEK_END);
  unsigned int size = ftell(f);
  fseek(f, 0, SEEK_SET);
  
  //char *rom = (char *)calloc(size, 1);
	
	fread(&state->memory[ROM_START], 1, size, f);
	state->rom_size = size;

  fclose(f);
	return 1;
}
