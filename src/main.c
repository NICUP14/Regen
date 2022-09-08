#include <stdio.h>
#include "parser.h"

char *ctx2str[] = {
    "CHCLASS",
    "GROUP",
};

char *type2str[] = {
    "UNDEFINED",
    "LITERAL",
    "GROUP_BEGIN",
    "GROUP_END",
    "GROUP_SEP",
    "CHCLASS_WORD",
    "CHCLASS_SPACE",
    "CHCLASS_DIGIT",
    "CHCLASS_RANGE",
    "CHCLASS_BEGIN",
    "CHCLASS_END",
};

int main()
{
    char str[100];
    char *str_iter = str;
    struct regen_stack *ctx_stack = _regen_stack_init();

    scanf("%99s", str);
    while (*str_iter != '\0')
    {
        char *ctx_str = _regen_stack_empty(ctx_stack) ? "GENERAL" : ctx2str[_regen_stack_peek(ctx_stack)];

        enum regen_token_type type = _regen_scan_token(&str_iter, ctx_stack);
        char *type_str = type2str[type];

        printf("CTX: %s, TYPE: %s\n", ctx_str, type_str);
        str_iter += type == UNDEFINED;
    }

    //! Error handler placeholder
    assert(_regen_stack_empty(ctx_stack));

    _regen_stack_free(&ctx_stack);
    return 0;
}