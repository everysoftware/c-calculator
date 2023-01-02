#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cmap.h"
#include "cvar.h"

#define CMAP_CAPACITY 1
#define CMAP_ZOOM_FACTOR 2

cmap* cmap_init() {
	cmap* temp = (cmap*)calloc(1, sizeof(cmap));
	if (temp != NULL) {
		temp->data = (cvar*)calloc(CMAP_CAPACITY, sizeof(cvar));
		temp->capacity = CMAP_CAPACITY;
		temp->size = 0;
	}
	return temp;
}

void cmap_free(cmap* map) {
	free(map->data);
	free(map);
}

int cmap_find(cmap const* map, const char* name) {
	for (size_t i = 0; i < map->size; ++i) {
		if (strcmp(map->data[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

void cmap_reserve(cmap* map, size_t new_capacity) {
	cvar* temp = (cvar*)calloc(new_capacity, sizeof(cvar));
	if (temp != NULL) {
		for (size_t i = 0; i < map->size && i < new_capacity; ++i) {
			temp[i] = map->data[i];
		}
	}
	free(map->data);
	map->data = temp;
	map->capacity = new_capacity;
}

bool cmap_insert(cmap* map, const char* name, _Dcomplex value) {
	if (cmap_find(map, name) != -1) {
		return false;
	}
	if (map->size == map->capacity) {
		cmap_reserve(map, map->size * CMAP_ZOOM_FACTOR);
	}
	strcpy_s(map->data[map->size].name, VAR_NAME_SIZE, name);
	map->data[map->size].value = value;
	++map->size;
	return true;
}

cvar cmap_back(cmap* map) {
	return map->data[map->size - 1];
}

bool cmap_get(cmap const* map, const char* name, _Dcomplex* buffer) {
	int i = cmap_find(map, name);
	if (i == -1) {
		return false;
	}
	*buffer = map->data[i].value;
	return true;
}

void cmap_print(cmap const* map) {
	char temp[C2S_BUFFER_SIZE] = "";
	printf("Complex map (%u elements):\n", map->size);
	for (size_t i = 0; i < map->size; ++i) {
		if (map->data[i].name[0] != '\0') {
			printf("\t%s = %s\n", map->data[i].name, complex2str(map->data[i].value, temp));
		}
	}
}




