#include <stdio.h>
#include <string.h>
#include <complex.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

#include "cvar.h"

bool is_digit(const char c) {
	return '0' <= c && c <= '9';
}
bool is_complex(const char* s)
{
	if (*s == '.') {
		return false;
	}
	for (size_t i = 0; i < strlen(s); ++i) {
		if (!is_digit(s[i]) && s[i] != '.' && s[i] != 'j' || i != 0 && s[i] == '-') {
			return false;
		}
	}
	return true;
}
bool is_real(const char* s)
{
	if (*s == '.') {
		return false;
	}
	for (size_t i = 0; i < strlen(s); ++i) {
		if ((!is_digit(s[i]) && s[i] != '.') || (i != 0 && s[i] == '-')) {
			return false;
		}
	}
	return true;
}

_Dcomplex str2complex(const char* s)
{
	_Dcomplex z = _DCOMPLEX_(0, 0);
	if (strcmp(s, "j") == 0) {
		z._Val[1] = 1;
	}
	else if (strchr(s, 'j') != NULL) {
		sscanf_s(s, "%lf %*[ji]", &z._Val[1]);
	}
	else {
		sscanf_s(s, "%lf", &z._Val[0]);
	}
	return z;
}
char* complex2str(_Dcomplex z, char* buffer)
{
	if (z._Val[1] == 0) {
		sprintf_s(buffer, C2S_BUFFER_SIZE, "%f", z._Val[0]);
	} 
	else if (z._Val[0] == 0) {
		sprintf_s(buffer, C2S_BUFFER_SIZE, "%fj", z._Val[1]);
	}
	else if (z._Val[1] > 0) {
		sprintf_s(buffer, C2S_BUFFER_SIZE, "%f+%fj", z._Val[0], z._Val[1]);
	}
	else {
		sprintf_s(buffer, C2S_BUFFER_SIZE, "%f-%fj", z._Val[0], fabs(z._Val[1]));
	}
	return buffer;
}
