#include <stdio.h>
#include "stack.h"

enum regen_token_type
{
	UNDEFINED,
	LITERAL,
	GROUP_BEGIN,
	GROUP_END,
	GROUP_SEP,
	CHCLASS_WORD,
	CHCLASS_SPACE,
	CHCLASS_DIGIT,
	CHCLASS_RANGE,
	CHCLASS_BEGIN,
	CHCLASS_END
};

#define CURR(i) (*i)
#define NEXT(i) (*(i + 1))
#define RET_TYPE(t) \
	type = t;       \
	goto RET

/// Detects escape sequences of the regen language
//? The method below acts as an extension of the "_regen_scan_token" method
char _regen_scan_escape(char **iter)
{
	if (CURR(*iter) != '\\')
		return -1;

	switch (NEXT(*iter))
	{
	case '0':
		return '\0';
	case 't':
		return '\t';
	case 'r':
		return '\r';
	case 'n':
		return '\n';
	default:
		return NEXT(*iter);
	}
}

/// Detects token constructs of the regen language
//? The method below acts as the lexer of the regen language
enum regen_token_type _regen_scan_token(char **iter, struct regen_stack *ctx_stack)
{
	size_t iter_off;
	enum regen_token_type type;

	/// Detects default character classes
	if (CURR(*iter) == '\\')
	{
		iter_off = 2;
		switch (NEXT(*iter))
		{
		case 'd':
			RET_TYPE(CHCLASS_DIGIT);
		case 'w':
			RET_TYPE(CHCLASS_WORD);
		case 's':
			RET_TYPE(CHCLASS_SPACE);
		default:
			iter_off = 0;
			RET_TYPE(UNDEFINED);
		}
	}

	/// Detects custom character classes
	if (CURR(*iter) == '[')
	{
		_regen_stack_push(ctx_stack, CHCLASS);

		iter_off = 1;
		RET_TYPE(CHCLASS_BEGIN);
	}
	if (CURR(*iter) == ']')
	{
		//! Error handler placeholder
		assert(_regen_stack_cmp(ctx_stack, CHCLASS));

		_regen_stack_pop(ctx_stack);

		iter_off = 1;
		RET_TYPE(CHCLASS_END);
	}

	/// Detects groups
	if ((CURR(*iter) == '(' || CURR(*iter) == ')') &&
		_regen_stack_cmp(ctx_stack, CHCLASS))
	{
		iter_off = 0;
		RET_TYPE(UNDEFINED);
	}
	if (CURR(*iter) == '(')
	{
		_regen_stack_push(ctx_stack, GROUP);

		iter_off = 1;
		RET_TYPE(GROUP_BEGIN);
	}
	if (CURR(*iter) == ')')
	{
		//! Error handler placeholder
		assert(_regen_stack_cmp(ctx_stack, GROUP));

		_regen_stack_pop(ctx_stack);

		iter_off = 1;
		RET_TYPE(GROUP_END);
	}

	/// Detects character class-specific tokens
	if (CURR(*iter) == '-' &&
		_regen_stack_cmp(ctx_stack, CHCLASS))
	{
		iter_off = 1;
		RET_TYPE(CHCLASS_RANGE);
	}

	/// Detects group-specific tokens
	if (CURR(*iter) == '|' &&
		_regen_stack_cmp(ctx_stack, GROUP))
	{
		iter_off = 1;
		RET_TYPE(GROUP_SEP);
	}

	iter_off = 0;
	RET_TYPE(UNDEFINED);

RET:
	*iter += iter_off;
	return type;
}

void regen_scan_str(const char *str)
{
	// TODO: Implement AST, AST_node constructs
	// TODO: Implement driver code for parser
}