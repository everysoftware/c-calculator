#define _USE_MATH_DEFINES
#include <complex.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h> // exit

#include "operations.h"
#include "cstack.h"

_Dcomplex xabs(_Dcomplex z)
{
	return _DCOMPLEX_(cabs(z), 0);
}
_Dcomplex xreal(_Dcomplex z)
{
	return _DCOMPLEX_(creal(z), 0);
}
_Dcomplex ximag(_Dcomplex z)
{
	return _DCOMPLEX_(cimag(z), 0);
}
_Dcomplex uplus(_Dcomplex z)
{
	return z;
}
_Dcomplex uminus(_Dcomplex z)
{
	z._Val[0] = -z._Val[0];
	z._Val[1] = -z._Val[1];
	return z;
}
_Dcomplex phase(_Dcomplex z)
{
	double w = 0;
	if (creal(z) < 0 && cimag(z) >= 0)
		w = M_PI;
	else if (creal(z) < 0 && cimag(z) < 0)
		w = -M_PI;
	return _DCOMPLEX_(atan(cimag(z) / creal(z)) + w, 0);
}

_Dcomplex plus(_Dcomplex a, _Dcomplex b)
{
	double x1 = a._Val[0];
	double x2 = b._Val[0];
	double y1 = a._Val[1];
	double y2 = b._Val[1];
	return _DCOMPLEX_(x1 + x2, y1 + y2);
}
_Dcomplex multiply(_Dcomplex a, _Dcomplex b)
{
	double x1 = a._Val[0];
	double x2 = b._Val[0];
	double y1 = a._Val[1];
	double y2 = b._Val[1];
	return _DCOMPLEX_(x1 * x2 - y1 * y2, x1 * y2 + x2 * y1);
}
_Dcomplex minus(_Dcomplex a, _Dcomplex b)
{
	double x1 = a._Val[0];
	double x2 = b._Val[0];
	double y1 = a._Val[1];
	double y2 = b._Val[1];
	return _DCOMPLEX_(x1 - x2, y1 - y2);
}
_Dcomplex divide(_Dcomplex a, _Dcomplex b)
{
	double x1 = a._Val[0];
	double x2 = b._Val[0];
	double y1 = a._Val[1];
	double y2 = b._Val[1];
	double denominator = x2 * x2 + y2 * y2;
	if (denominator == 0)
	{
		printf("[Calculation Error] Divison by zero\n");
		return _DCOMPLEX_(0, 0);
	}
	return _DCOMPLEX_((x1 * x2 + y1 * y2) / denominator, (x2 * y1 - x1 * y2) / denominator);
}
_Dcomplex xlog(_Dcomplex x, _Dcomplex a)
{
	return divide(clog(x), clog(a));
}

static unary_op const unary_ops[] = {
	{{-'+', '\0'}, &uplus, UNARY_FUNC_PRIORITY, false},
	{{-'-', '\0'}, &uminus, UNARY_FUNC_PRIORITY, false},
	{"mag", &xabs, UNARY_FUNC_PRIORITY, true},
	{"sqrt", &csqrt, UNARY_FUNC_PRIORITY, true},
	{"sin", &csin, UNARY_FUNC_PRIORITY, true},
	{"cos", &ccos, UNARY_FUNC_PRIORITY, true},
	{"tg", &ctan, UNARY_FUNC_PRIORITY, true},
	{"exp", &cexp, UNARY_FUNC_PRIORITY, true},
	{"ln", &clog, UNARY_FUNC_PRIORITY, true},
	{"real", &xreal, UNARY_FUNC_PRIORITY, true},
	{"imag", &ximag, UNARY_FUNC_PRIORITY, true},
	{"abs", &xabs, UNARY_FUNC_PRIORITY, true},
	{"phase", &phase, UNARY_FUNC_PRIORITY, true},
	{"arg", &phase, UNARY_FUNC_PRIORITY, true},
	{"conj", &conj, UNARY_FUNC_PRIORITY, true}
};
static binary_op const binary_ops[] = {
	{"(", NULL, BRACKETS_PRIORITY, false},
	{")", NULL, BRACKETS_PRIORITY, false},
	{"+", &plus, PLUSMINUS_PRIORITY, false},
	{"-", &minus, PLUSMINUS_PRIORITY, false},
	{"*", &multiply, MULTIPLYDIVIDE_PRIORITY, false},
	{"/", &divide, MULTIPLYDIVIDE_PRIORITY, false},
	{"^", &cpow, BINARY_FUNC_PRIORITY, false},
	{"pow", &cpow, BINARY_FUNC_PRIORITY, true},
	{"log", &xlog, BINARY_FUNC_PRIORITY, true}
};
static constant const consts[] = {
	{"PI", {M_PI, 0}},
	{"e", {M_E, 0}}
};

#define UNARY_OP_COUNT (sizeof(unary_ops) / sizeof(unary_op const))
#define BINARY_OP_COUNT (sizeof(binary_ops) / sizeof(unary_op const))
#define CONST_COUNT (sizeof(consts) / sizeof(constant const))

bool get_const(const char* name, _Dcomplex* buffer) {
	for (size_t i = 0; i < CONST_COUNT; ++i) {
		if (strcmp(consts[i].name, name) == 0) {
			*buffer = consts[i].value;
			return true;
		}
	}
	return false;
}

int get_priority(const char* op)
{
	if (strlen(op) == 0) {
		return NOT_OP_PRIORITY;
	}
	// за этим скрывается унарный плюс/минус
	if (op[0] < 0) {
		return UNARY_FUNC_PRIORITY;
	}
	for (size_t i = 0; i < BINARY_OP_COUNT; ++i) {
		if (strcmp(op, binary_ops[i].name) == 0) {
			return binary_ops[i].priority;
		}
	}
	for (size_t i = 0; i < UNARY_OP_COUNT; ++i) {
		if (strcmp(op, unary_ops[i].name) == 0) {
			return unary_ops[i].priority;
		}
	}
	return NOT_OP_PRIORITY;
}
bool is_op(const char* op) {
	return get_priority(op) != NOT_OP_PRIORITY;
}
bool is_op_c(const char op) {
	char temp[2] = { op, '\0' };
	return is_op(temp);
}
bool is_unary(const char* op) {
	return get_priority(op) == UNARY_FUNC_PRIORITY;
}
bool is_bracket_op(const char* op) {
	for (size_t i = 0; i < UNARY_OP_COUNT; ++i) {
		if (strcmp(op, unary_ops[i].name) == 0) {
			return unary_ops[i].is_bracket;
		}
	}
	for (size_t i = 0; i < BINARY_OP_COUNT; ++i) {
		if (strcmp(op, binary_ops[i].name) == 0) {
			return binary_ops[i].is_bracket;
		}
	}
	return false;
}
bool is_bracket_op_c(const char op) {
	char temp[2] = { op, '\0' };
	return is_bracket_op(temp);
}
_Dcomplex process_unary_op(const char* op, _Dcomplex a) {
	for (size_t i = 0; i < UNARY_OP_COUNT; ++i) {
		if (strcmp(unary_ops[i].name, op) == 0) {
			return unary_ops[i].func(a);
		}
	}
	printf("[Parsing Error] Undefined unary op %s\n", op);
	return _DCOMPLEX_(0, 0);
}
_Dcomplex process_binary_op(const char* op, _Dcomplex a, _Dcomplex b) {
	for (size_t i = 0; i < BINARY_OP_COUNT; ++i) {
		if (strcmp(binary_ops[i].name, op) == 0) {
			return binary_ops[i].func(a, b);
		}
	}
	printf("[Parsing Error] Undefined binary op %s\n", op);
	return _DCOMPLEX_(0, 0);
}
void process_op(cstack* st, const char* op)
{
	_Dcomplex lhs = { 0 };
	_Dcomplex rhs = { 0 };
	if (is_unary(op)) {
		lhs = cstack_pop(st);
		cstack_push(st, process_unary_op(op, lhs));
	}
	else {
		rhs = cstack_pop(st);
		lhs = cstack_pop(st);
		cstack_push(st, process_binary_op(op, lhs, rhs));
	}
}
