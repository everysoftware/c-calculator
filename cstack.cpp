#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

#include "cstack.h"
#include "cvar.h"

#define CSTACK_ZOOM_FACTOR 2

cstack* cstack_init(size_t size) {
	cstack* temp = (cstack*)calloc(1, sizeof(cstack));
	if (temp != NULL) {
		temp->data = (_Dcomplex*)calloc(size, sizeof(_Dcomplex));
		temp->capacity = size;
		temp->size = 0;
	}
	return temp;
}

void cstack_free(cstack* s) {
	free(s->data);
	free(s);
}

void cstack_reserve(cstack* s, size_t new_capacity) {
	_Dcomplex* temp = (_Dcomplex*)calloc(new_capacity, sizeof(_Dcomplex));
	if (temp != NULL) {
		for (size_t i = 0; i < s->size && i < new_capacity; ++i) {
			temp[i] = s->data[i];
		}
	}
	free(s->data);
	s->data = temp;
	s->capacity = new_capacity;
}

void cstack_push(cstack* s, _Dcomplex x) {
	if (s->size == s->capacity) {
		cstack_reserve(s, s->size * CSTACK_ZOOM_FACTOR);
	}
	s->data[s->size] = x;
	++s->size;
}

_Dcomplex cstack_pop(cstack* s) {
	--s->size;
	return s->data[s->size];
}

_Dcomplex cstack_back(cstack const* s) {
	return s->data[s->size - 1];
}

void cstack_print(cstack const* s) {
	char temp[C2S_BUFFER_SIZE] = "";
	printf("Complex stack (%u elements): ", s->size);
	for (size_t i = 0; i < s->size; ++i) {
		printf("%s ", complex2str(s->data[i], temp));
	}
	printf("\n");
}
