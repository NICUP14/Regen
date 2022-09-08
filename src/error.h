#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/// Program's name
#define PROG "Regen"

/// ANSI color sequences
#define ANSI_RED "\\e[0;31m"
#define ANSI_GREEN "\\e[0;32m"
#define ANSI_PURPLE "\\e[0;35m"
#define ANSI_RESET "\\e[0m"

#define REGEN_MEM_ERR "Insufficient memory"

/// Error logging macros
#define printf_err(str) fprintf(stderr, "%s: %s%s%s", PROG, ANSI_RED, str, ANSI_RESET)
#define printf_warn(str) fprintf(stderr, "%s: %s%s%s", PROG, ANSI_PURPLE, str, ANSI_RESET)
#define printf_succ(str) fprintf(stdout, "%s: %s%s%s", PROG, ANSI_GREEN, str, ANSI_RESET)