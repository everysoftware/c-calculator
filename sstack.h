#ifndef SSTACK_H
#define SSTACK_H

#include <stddef.h>

typedef struct sstack
{
	char** data;
	size_t size;
	size_t capacity;
	size_t str_capacity;
} sstack;

sstack* sstack_init(size_t str_cap);

void sstack_free(sstack* s);

void sstack_reserve(sstack* s, size_t new_capacity);

void sstack_push(sstack* s, const char* str);

void sstack_push_char(sstack* s, const char c);

char* sstack_pop(sstack* s);

char* sstack_back(sstack const* s);

void sstack_print(sstack const* s);

#endif
