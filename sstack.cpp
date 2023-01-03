#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sstack.h"

#define SSTACK_CAPACITY 2
#define SSTACK_ZOOM_FACTOR 1.5

sstack* sstack_init(size_t str_cap) {
	sstack* temp = (sstack*)calloc(1, sizeof(sstack));
	if (temp != NULL) {
		temp->data = (char**)calloc(SSTACK_CAPACITY, sizeof(char*));
		if (temp->data != NULL) {
			for (size_t i = 0; i < SSTACK_CAPACITY; ++i) {
				temp->data[i] = (char*)calloc(str_cap, sizeof(char));
			}
		}
		temp->str_capacity = str_cap;
		temp->capacity = SSTACK_CAPACITY;
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

void sstack_reserve(sstack* s, size_t new_cap) {
	char** temp = (char**)calloc(new_cap, sizeof(char*));
	if (temp != NULL) {
		for (size_t i = 0; i < new_cap; ++i) {
			temp[i] = (i < s->size) ? s->data[i] : (char*)calloc(s->str_capacity, sizeof(char));
		}
	}
	free(s->data);
	s->data = temp;
	s->capacity = new_cap;
}

void sstack_push(sstack* s, const char* str) {
	if (s->size == s->capacity) {
		sstack_reserve(s, (size_t)(s->size * SSTACK_ZOOM_FACTOR));
	}
	strcpy_s(s->data[s->size], s->str_capacity, str);
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
