#ifndef PARSER_H
#define PARSER_H

#include <complex.h>

#include "cmap.h"

#define EXPR_SIZE 1024
#define TOKEN_SIZE 256

enum parser_errors {
	PARSER_OK,
	PARSER_ERR_EMPTY_EXPRESSION,
	PARSER_ERR_INCORRECT_BRACKET_PLACEMENT,
	PARSER_ERR_UNDEFINED_NAME,
	PARSER_ERR_INCORRECT_EXPRESSION
};

int parse_expr(const char* exp, cmap* vars, cvar* destination);

#endif

