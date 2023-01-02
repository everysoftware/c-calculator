#ifndef CMAP_H
#define CMAP_H

#include <complex.h>
#include <stddef.h>

#include "cvar.h"

#define VAR_COUNT 1024

typedef struct cmap
{
	cvar* data;
	size_t size;
	size_t capacity;
} cmap;

cmap* cmap_init();

void cmap_free(cmap* map);

int cmap_find(cmap const* arr, const char* name);

bool cmap_insert(cmap* map, const char* name, _Dcomplex value);

bool cmap_get(cmap const* map, const char* name, _Dcomplex* buffer);

void cmap_print(cmap const* arr);

#endif
