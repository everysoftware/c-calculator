#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <complex.h>
#include <stdbool.h>

#include "sstack.h"
#include "cstack.h"

#define OP_COUNT 32
#define OP_NAME_SIZE 10 
#define CONSTANT_NAME_SIZE 10 

enum op_types {
	UNARY_OP_TYPE,
	BINARY_OP_TYPE
};
enum op_priorities
{
	NOT_OP_PRIORITY = -1,
	BRACKETS_PRIORITY,
	PLUSMINUS_PRIORITY,
	MULTIPLYDIVIDE_PRIORITY,
	BINARY_FUNC_PRIORITY,
	UNARY_FUNC_PRIORITY
};

typedef struct binary_op {
	char name[OP_NAME_SIZE];
	_Dcomplex(*func)(_Dcomplex, _Dcomplex);
	int priority;
	bool is_bracket;
} binary_op;

typedef struct unary_op {
	char name[OP_NAME_SIZE];
	_Dcomplex(*func)(_Dcomplex);
	int priority;
	bool is_bracket;
} unary_op;

typedef struct constant {
	char name[CONSTANT_NAME_SIZE];
	_Dcomplex value;
} constant;


bool get_const(const char* name, _Dcomplex* buffer);

int get_priority(const char* op);
bool is_bracket_op(const char* op);
bool is_bracket_op_c(const char op);
bool is_op(const char* op);
bool is_op_c(const char op);
bool is_unary(const char* op);
bool process_op(cstack* st, const char* op);

#endif
