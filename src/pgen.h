#include "pgen_lexer.h"

typedef struct patter_generator pgen_t;

int pgen_hash(pgen_t *pattern);
void pgen_free(pgen_t *pattern);
void pgen_alloc(char *expression);
void generate(int (*validate)(char *output), void (*on_success)(char *output));
void generate_item(size_t idx, int (*validate)(char *output), void (*on_success)(char *output));
void generate_range(size_t start_idx, size_t stop_idx, int (*validate)(char *output), void (*on_success)(char *output));
