#ifndef COMPLEX_VAR_H
#define COMPLEX_VAR_H

#include <complex.h>
#include <stdbool.h>

#define C2S_BUFFER_SIZE 256
#define VAR_NAME_SIZE 32

typedef struct cvar
{
	char name[VAR_NAME_SIZE];
	_Dcomplex value;
} cvar;

bool is_digit(const char c);

bool is_complex(const char* s);

bool is_real(const char* s);

_Dcomplex str2complex(const char* s);

char* complex2str(_Dcomplex z, char* buffer);

#endif
