#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "ran.h"

int get_state_file_subpath(char *buffer, size_t size, const char *filename)
{
    const char *home = getenv("HOME");
    const char *base = NULL;

    if (home && strlen(home) > 0)
    {
        base = home;
    } else
    {
        return 0;
    }

    char dir_path[512];
    if (snprintf(dir_path, sizeof(dir_path), "%s/.ran_c", base) >= sizeof(dir_path)) {
        return 0;
    }

    if (mkdir(dir_path, 0700) == -1 && errno != EEXIST){
        return 0;
    }

    if (snprintf(buffer, size, "%s/%s", dir_path, filename) >= size){
        return 0;
    }

    return 1;
}


int load_array_double(const char *filename, double *array, size_t size)
{
    FILE *f = fopen(filename, "rb");
    if (!f){ return 0; }
    size_t read = fread(array, sizeof(double), size, f);
    fclose(f);
    return read == size;
}

int save_array_double(const char *filename, double *array, size_t size)
{
    FILE *f = fopen(filename, "wb");
    if (!f){ return 0; }
    size_t written = fwrite(array, sizeof(double), size, f);
    fclose(f);
    return written == size;
}



int load_rng_state(Ran *rng, const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        return 0;
    }
    size_t read = fread(rng, sizeof(Ran), 1, f);
    fclose(f);
    return read == 1;
}

void save_rng_state(const Ran *rng, const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        perror("Failed to open rng state file.");
        return;
    }
    fwrite(rng, sizeof(Ran), 1, f);
    fclose(f);
}


