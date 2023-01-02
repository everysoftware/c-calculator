#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>
#include <conio.h>
#include <windows.h> // sleep

#include "parser.h"
#include "cmap.h"
#include "sstack.h"

#define INPUT_FILENAME "input.txt"
#define OUTPUT_FILENAME "output.txt"
#define FILE_LINES_COUNT 32

void print_menu() {
	printf("....... C Calculator .......\n");
	printf("Choose an action:\n");
	printf("1. Read from input.txt\n");
	printf("2. Read from console\n");
	printf("3. About\n");
	printf("4. Exit\n");
}

bool show_about() {
	printf("C Calculator Project\n");
	printf("Supports:\n");
	printf("* variables\n* complex numbers\n* popular functions(pow, abs, ...)\n");
	printf("by Ivan Stasevich\n");
	printf("02.01.2023\n");
	printf("Press any key to return to the menu\n");
	_getch();
	system("cls");
	return true;
}

void output_expr_value(FILE* f, cmap* vars, const char* expr) {
	cvar x = { 0 };
	int status = parse_expr(expr, vars, &x);
	switch (status) {
	case PARSER_OK:
		char temp[C2S_BUFFER_SIZE] = "";
		fprintf(f, "%s = %s\n", x.name[0] != '\0' ? x.name : expr, complex2str(x.value, temp));
		break;
	case PARSER_ERR_EMPTY_EXPRESSION:
		fprintf(f, "[Parsing Error] Empty expression is given\n");
		break;
	case PARSER_ERR_INCORRECT_BRACKET_PLACEMENT:
		fprintf(f, "[Parsing Error] Incorrect bracket placement\n");
		break;
	case PARSER_ERR_UNDEFINED_NAME:
		fprintf(f, "[Parsing Error] Undefined name in expression");
		break;
	}
}

bool console_input(cmap* vars) {
	char expr[EXPR_SIZE] = "";
	system("cls");
	printf("Type 'exit' to exit and 'return' to return to the menu\n");
	printf(">>> ");
	gets_s(expr, EXPR_SIZE);
	while (strcmp(expr, "exit") != 0 && strcmp(expr, "return") != 0)
	{
		output_expr_value(stdout, vars, expr);
		printf(">>> ");
		gets_s(expr, EXPR_SIZE);
	}
	return strcmp(expr, "return") == 0;
}

bool file_input(cmap* vars) {
	system("cls");
	FILE* f = NULL;
	FILE* fo = NULL;
	if (fopen_s(&f, INPUT_FILENAME, "r") != 0) {
		printf("[File Error] Could not open '%s'\n", INPUT_FILENAME);
		return false;
	}
	if (fopen_s(&fo, OUTPUT_FILENAME, "w") != 0) {
		printf("[File Error] Could not open '%s'\n", OUTPUT_FILENAME);
		return false;
	}
	sstack* ss = sstack_init(FILE_LINES_COUNT, EXPR_SIZE);
	// ������ ���������
	char temp[EXPR_SIZE] = "";
	while (fgets(temp, EXPR_SIZE, f) != NULL) {
		temp[strcspn(temp, "\n")] = '\0';
		sstack_push(ss, temp);
	}
	fclose(f);
	size_t expr_count = ss->size;
	// ���������
	for (size_t i = 0; i < expr_count; ++i) {
		output_expr_value(fo, vars, sstack_pop(ss));
	}
	printf("[Success] Results are at '%s'\n", OUTPUT_FILENAME);
	fclose(fo);
	sstack_free(ss);
	return true;
}

bool menu(cmap* vars) {
	print_menu();
	char ch = _getch();
	while (ch != '1' && ch != '2' && ch != '3') {
		ch = _getch();
	}
	system("cls");
	switch (ch) {
	case '1':
		return file_input(vars);
	case '2':
		return console_input(vars);
	case '3':
		return show_about();
	case '4':
		return false;
	}
	return false;
}

int main() {
	cmap* vars = cmap_init(VAR_COUNT);
	while (menu(vars)) {
		menu(vars);
	}
	cmap_free(vars);
	return 0;
}