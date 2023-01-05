#ifndef CMAP_H
#define CMAP_H

#include <complex.h>
#include <stddef.h>

#include "cvar.h"

typedef struct cmap
{
	cvar* data;
	size_t size;
	size_t capacity;
} cmap;

cmap* cmap_init();

void cmap_free(cmap* map);

int cmap_find(cmap const* arr, const char* name);

void cmap_reserve(cmap* map, size_t new_cap);

bool cmap_insert(cmap* map, const char* name, _Dcomplex value);

bool cmap_get(cmap const* map, const char* name, _Dcomplex* buffer);

void cmap_print(cmap const* arr);

#endif
