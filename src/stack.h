#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "error.h"

enum regen_context
{
    CHCLASS,
    GROUP
};

struct regen_stack_node
{
    enum regen_context ctx;
    struct regen_stack_node *next;
};

struct regen_stack
{
    size_t len;
    struct regen_stack_node *head;
};

void *_regen_stack_init()
{
    struct regen_stack *stack = malloc(sizeof(struct regen_stack));

    if (stack != NULL)
    {
        stack->len = 0;
        stack->head = NULL;
    }

    return stack;
}

int _regen_stack_empty(const struct regen_stack *stack)
{
    return stack->len == 0;
}

enum regen_context _regen_stack_peek(const struct regen_stack *stack)
{
    //! Error handler placeholder
    assert(stack->len > 0);

    return stack->head->ctx;
}

int _regen_stack_cmp(const struct regen_stack *stack, enum regen_context ctx)
{
    return !_regen_stack_empty(stack) && _regen_stack_peek(stack) == ctx;
}

void _regen_stack_free(struct regen_stack **stack)
{
    struct regen_stack_node *temp;
    struct regen_stack_node *node = (*stack)->head;

    while (node != NULL)
    {
        temp = node;
        node = node->next;

        free(temp);
    }

    free(*stack);
    *stack = NULL;
}

int _regen_stack_pop(struct regen_stack *stack)
{
    if (stack->len == 0)
        return 0;

    stack->len--;
    stack->head = stack->head->next;

    return 1;
}

int _regen_stack_push(struct regen_stack *stack, enum regen_context ctx)
{
    struct regen_stack_node *node = malloc(sizeof(struct regen_stack_node));

    if (node == NULL)
        return 0;

    node->ctx = ctx;
    node->next = stack->head;

    if (stack->head == NULL)
        stack->len = 1;
    else
        stack->len++;
    stack->head = node;

    return 1;
}