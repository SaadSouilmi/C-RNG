#ifndef STATE_H
#define STATE_H

#include <stddef.h>
#include "ran.h"  

int get_state_file_path(char *buffer, size_t size);
int load_rng_state(Ran *rng, const char *filename);
void save_rng_state(const Ran *rng, const char *filename);

#endif // STATE_H

