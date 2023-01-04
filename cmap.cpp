#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cmap.h"
#include "cvar.h"

#define CMAP_CAPACITY 2
#define CMAP_ZOOM_FACTOR 1.5

cmap* cmap_init() {
	cmap* temp = (cmap*)calloc(1, sizeof(cmap));
	if (temp != NULL) {
		temp->data = (cvar*)calloc(CMAP_CAPACITY, sizeof(cvar));
		temp->capacity = CMAP_CAPACITY;
		temp->size = 0;
	}
	else {
		perror("calloc");
		exit(EXIT_FAILURE);
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

void cmap_reserve(cmap* map, size_t new_cap) {
	cvar* temp = (cvar*)calloc(new_cap, sizeof(cvar));
	if (temp != NULL) {
		for (size_t i = 0; i < map->size && i < new_cap; ++i) {
			temp[i] = map->data[i];
		}
	}
	else {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	free(map->data);
	map->data = temp;
	map->capacity = new_cap;
}

bool cmap_insert(cmap* map, const char* name, _Dcomplex value) {
	if (cmap_find(map, name) != -1) {
		return false;
	}
	if (map->size == map->capacity) {
		cmap_reserve(map, (size_t)(map->size * CMAP_ZOOM_FACTOR));
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




