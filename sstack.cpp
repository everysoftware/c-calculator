#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sstack.h"

#define SSTACK_ZOOM_FACTOR 2

sstack* sstack_init(size_t size, size_t string_size) {
	sstack* temp = (sstack*)calloc(1, sizeof(sstack));
	if (temp != NULL) {
		temp->data = (char**)calloc(size, sizeof(char*));
		if (temp->data != NULL) {
			for (size_t i = 0; i < size; ++i) {
				temp->data[i] = (char*)calloc(string_size, sizeof(char));
			}
		}
		temp->string_size = string_size;
		temp->capacity = size;
		temp->size = 0;
	}
	return temp;
}

void sstack_free(sstack* s) {
	for (size_t i = 0; i < s->capacity; ++i) {
		free(s->data[i]);
	}
	free(s->data);
	free(s);
}

void sstack_reserve(sstack* s, size_t new_capacity) {
	char** temp = (char**)calloc(new_capacity, sizeof(char*));
	size_t n = s->size > new_capacity ? new_capacity : s->size;
	if (temp != NULL) {
		for (size_t i = 0; i < new_capacity; ++i) {
			temp[i] = (i > n) ? s->data[i] : (char*)calloc(s->string_size, sizeof(char));
		}
	}
	for (size_t i = 0; i < s->capacity; ++i) {
		free(s->data[i]);
	}
	free(s->data);
	s->data = temp;
	s->capacity = new_capacity;
}

void sstack_push(sstack* s, const char* str) {
	if (s->size == s->capacity) {
		sstack_reserve(s, s->size * SSTACK_ZOOM_FACTOR);
	}
	strcpy_s(s->data[s->size], s->string_size, str);
	++s->size;
}

void sstack_push_char(sstack* s, const char c) {
	char temp[2] = { c, '\0' };
	sstack_push(s, temp);
}

char* sstack_pop(sstack* s) {
	--s->size;
	return s->data[s->size];
}

char* sstack_back(sstack const* s) {
	return s->data[s->size - 1];
}

void sstack_print(sstack const* s) {
	printf("String stack (%u elements): ", s->size);
	for (size_t i = 0; i < s->size; ++i) {
		printf("%s ", s->data[i]);
	}
	printf("\n");
}
