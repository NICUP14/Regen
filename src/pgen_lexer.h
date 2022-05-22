#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pgen_token.h"

#define DEBUG_NOP

void *_pgen_lexer_new_token(char *symbol)
{
    struct pgen_token *token = malloc(sizeof(struct pgen_token));

    // ! ERROR handler placeholder
    assert(token != NULL);

    token->len = strlen(symbol);
    token->symbol = symbol;
    token->type = literal;

    return token;
}

size_t _pgen_lexer_count(const char *str, size_t str_len, char c)
{
    size_t counter = 0;
    for (const char *str_iter = str; str_iter < str + str_len; str_iter++)
        counter += *str_iter == c;

    return counter;
}

char *_pgen_lexer_extract(const char *start_iter, const char *stop_iter, const char *mask)
{
    // ! ERROR handler placeholder
    assert(start_iter != stop_iter);

    size_t mask_len = stop_iter - start_iter;
    size_t symbol_len = _pgen_lexer_count(mask, mask_len, 1);
    char *symbol = malloc(symbol_len + 1);

    // ! ERROR handler placeholder
    assert(symbol != NULL);

    size_t symbol_idx = 0;
    for (char const *str_iter = start_iter; str_iter < stop_iter; str_iter++)
        if (*(mask + (str_iter - start_iter)))
            symbol[symbol_idx++] = *str_iter;
    symbol[symbol_len] = '\0';

    return symbol;
}

void _pgen_lex_expression(const char *expression)
{
    // ! ERROR handler placeholder
    assert(expression != NULL);

    size_t expression_len = strlen(expression);

    // ! ERROR handler placeholder
    assert(expression_len != 0);

    struct pgen_token **tokens = malloc(sizeof(void *) * expression_len);

    // ! ERROR handler placeholder
    assert(tokens != NULL);

    char mask[expression_len + 1];
    mask[expression_len] = '\0';
    memset(mask, 1, expression_len);

    /// Gathers all the references of the predefined symbols into an array
    size_t tokens_len = 0;
    char const *expression_iter = expression;
    char const *prev_expression_iter = expression;
    while (expression_iter < expression + expression_len)
    {
        // TODO: Rename the token iterator to avoid confusion PGEN_TOKENS <-> tokens
        for (struct pgen_token **tokens_iter = PGEN_TOKENS; tokens_iter < PGEN_TOKENS + PGEN_TOKENS_LEN; tokens_iter++)
        {
            if (strncmp(expression_iter, (*tokens_iter)->symbol, (*tokens_iter)->len))
                continue;

            if (expression_iter != prev_expression_iter)
            {
                size_t mask_off = prev_expression_iter - expression;

                char *symbol = _pgen_lexer_extract(
                    prev_expression_iter,
                    expression_iter,
                    mask + mask_off);

                tokens[tokens_len++] = _pgen_lexer_new_token(symbol);
            }

            tokens[tokens_len++] = *tokens_iter;

            expression_iter += (*tokens_iter)->len - 1;
            prev_expression_iter += (*tokens_iter)->len;

            break;
        }

        expression_iter++;
    }

    DEBUG_NOP;

    for (struct pgen_token **tokens_iter = tokens; tokens_iter < tokens + tokens_len; tokens_iter++)
    {
        if ((*tokens_iter)->type == literal)
            free((*tokens_iter)->symbol);

        free(*tokens_iter);
    }
    free(tokens);
}