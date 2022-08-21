#include <stdio.h>
#include <string.h>
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

#define ESCAPE_IDS "0trn"
#define CHCLASS_IDS "dws"

/// Detects escape sequences of the regen language
//? The block below acts as an extension of the "_regen_scan_token" method
char _regen_scan_escape(char **iter)
{
	if (CURR(*iter) != '\\')
		return -1;

	char const *escape_ids_iter = strchr(ESCAPE_IDS, NEXT(*iter));

	switch (escape_ids_iter - ESCAPE_IDS)
	{
	case 0:
		return '\0';
	case 1:
		return '\t';
	case 2:
		return '\r';
	case 3:
		return '\n';
	default:
		return NEXT(*iter);
	}
}

/// Detects token constructs of the regen language
enum regen_token_type _regen_scan_token(char **iter, struct regen_stack *ctx_stack)
{
	size_t iter_off;
	enum regen_token_type type;

	/// Detects default character classes
	if (CURR(*iter) == '\\')
	{
		char const *chclass_ids_iter = strchr(CHCLASS_IDS, NEXT(*iter));

		if (chclass_ids_iter == NULL)
		{
			iter_off = 0;
			RET_TYPE(UNDEFINED);
		}

		iter_off = 2;
		switch (chclass_ids_iter - CHCLASS_IDS)
		{
		case 0:
			RET_TYPE(CHCLASS_DIGIT);
		case 1:
			RET_TYPE(CHCLASS_WORD);
		case 2:
			RET_TYPE(CHCLASS_SPACE);
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
		assert(_regen_stack_empty(ctx_stack) || _regen_stack_peek(ctx_stack) == CHCLASS);

		_regen_stack_pop(ctx_stack);

		iter_off = 1;
		RET_TYPE(CHCLASS_END);
	}

	/// Detects groups
	if ((CURR(*iter) == '(' || CURR(*iter) == ')') &&
		!_regen_stack_empty(ctx_stack) && _regen_stack_peek(ctx_stack) == CHCLASS)
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
		assert(_regen_stack_empty(ctx_stack) || _regen_stack_peek(ctx_stack) == GROUP);

		_regen_stack_pop(ctx_stack);

		iter_off = 1;
		RET_TYPE(GROUP_END);
	}

	/// Detects character class-specific tokens
	if (CURR(*iter) == '-' &&
		!_regen_stack_empty(ctx_stack) && _regen_stack_peek(ctx_stack) == CHCLASS)
	{
		iter_off = 1;
		RET_TYPE(CHCLASS_RANGE);
	}

	/// Detects group-specific tokens
	if (CURR(*iter) == '|' &&
		!_regen_stack_empty(ctx_stack) && _regen_stack_peek(ctx_stack) == GROUP)
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