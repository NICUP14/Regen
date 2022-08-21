#include <stdlib.h>
#include <assert.h>

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

int _regen_stack_empty(struct regen_stack *stack)
{
    return stack->len == 0;
}

enum regen_context _regen_stack_peek(struct regen_stack *stack)
{
    //! Error handler placeholder
    assert(stack->len > 0);

    return stack->head->ctx;
}

void _regen_stack_free(struct regen_stack *stack)
{
    struct regen_stack_node *temp;
    struct regen_stack_node *node = stack->head;

    while (node != NULL)
    {
        temp = node;
        node = node->next;

        free(temp);
    }

    stack->len = 0;
    stack->head = NULL;
}

void _regen_stack_pop(struct regen_stack *stack)
{
    //! Error handler placeholder
    assert(stack->len > 0);

    stack->len--;
    stack->head = stack->head->next;
}

void _regen_stack_push(struct regen_stack *stack, enum regen_context ctx)
{
    struct regen_stack_node *node = malloc(sizeof(struct regen_stack_node));

    //! Error handler placeholder
    assert(node != NULL);

    node->ctx = ctx;
    node->next = stack->head;

    if (stack->head == NULL)
        stack->len = 1;
    else
        stack->len++;

    stack->head = node;
}