#ifndef CSTACK_H
#define CSTACK_H

#include <complex.h>
#include <stddef.h>
#include "cvar.h"

typedef struct cstack
{
	size_t size;
	size_t capacity;
	_Dcomplex* data;
} cstack;

cstack* cstack_init(size_t size);

void cstack_free(cstack* s);

void cstack_reserve(cstack* s, size_t new_capacity);

void cstack_push(cstack* s, _Dcomplex z);

_Dcomplex cstack_pop(cstack* s);

_Dcomplex cstack_back(cstack const* s);

void cstack_print(cstack const* s);

#endif
