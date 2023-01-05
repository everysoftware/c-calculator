#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "parser.h"
#include "cstack.h"
#include "cmap.h"
#include "sstack.h"
#include "operations.h"
#include "cvar.h"

// Удаляет пробелы и переходы на новую строку
void remove_spaces(char* source) {
	char* i = source;
	char* j = source;
	while (*j) {
		*i = *j++;
		if (*i != ' ' && *i != '\n' && *i != '\r') {
			i++;
		}
	}
	*i = 0;
}

bool check_brackets(const char* s)
{
	sstack* stack = sstack_init(2);
	bool flag = true;
	while (*s) {
		if (*s == '(') {
			sstack_push(stack, "(");
		}
		else if (*s == ')') {
			if (stack->size > 0) {
				sstack_pop(stack);
			}
			else {
				flag = false;
				break;
			}
		}
		++s;
	}
	flag = flag && (stack->size == 0);
	sstack_free(stack);
	return flag;
}

sstack* split_into_tokens(const char* expr) {
	sstack* tokens = sstack_init(TOKEN_SIZE);
	size_t len = strlen(expr);
	char x[TOKEN_SIZE] = "";
	size_t x_len = 0;
	// имя оператора, переменной или константы
	char name[TOKEN_SIZE] = "";
	size_t name_len = 0;
	for (size_t i = 0; i < len; ++i) {
		// определяем, перед нами часть числа или оператор
		if (is_digit(expr[i]) && name_len == 0 || // цифра, не являющаяся частью какого-то имени
			expr[i] == '.' || // точка (может встречаться только в составе числа)
			// мнимая единица
			// стоит в одиночку или после R-числа
			expr[i] == 'j' && (x_len == 0 || is_real(x)) &&
			// стоит в конце выражения или след. символ нескобочный оператор (+, -, ...)
			(i == len - 1 || !is_bracket_op_c(expr[i + 1]))) {
			x[x_len++] = expr[i];
			x[x_len] = '\0';
		}
		// нескобочный оператор или имя
		else {
			// обрабатываем накопившееся число
			if (x_len > 0) {
				sstack_push(tokens, x);
				x[0] = '\0';
				x_len = 0;
			}
			if (is_op_c(expr[i]) && !is_bracket_op_c(expr[i]) || expr[i] == ',') {
				// обрабатываем накопившееся имя
				if (name_len > 0) {
					sstack_push(tokens, name);
					name[0] = '\0';
					name_len = 0;
				}
				// обработка **
				if (expr[i] == '*' && i != 0 && expr[i - 1] == '*') {
					sstack_pop(tokens);
					sstack_push(tokens, "**");
				}
				else {
					sstack_push_char(tokens, expr[i]);
				}
			}
			// "накапливаем" имя
			else {
				name[name_len++] = expr[i];
				name[name_len] = '\0';
			}
		}
	}
	if (name_len > 0) {
		sstack_push(tokens, name);
	}
	if (x_len > 0) {
		sstack_push(tokens, x);
	}
	return tokens;
}

bool resolve_name(const char* name, cmap const* vars, _Dcomplex* buffer) {
	// константа? переменная?
	return get_const(name, buffer) || cmap_get(vars, name, buffer);
}

int solve_with_RPN(const char* expr, cmap const* vars, _Dcomplex* buffer) {
	cstack* st = cstack_init();
	sstack* op = sstack_init(OP_NAME_SIZE);
	bool may_unary = true; // для различения унарных плюса и минуса
	sstack* tokens = split_into_tokens(expr);
	int status = PARSER_OK;
	if (tokens->size == 0) {
		status = PARSER_ERR_EMPTY_EXPRESSION;
	}
	for (size_t i = 0; i < tokens->size; ++i) {
		const char* t = tokens->data[i];
		if (strcmp(t, "(") == 0) {
			sstack_push(op, t);
			may_unary = true;
		}
		else if (strcmp(t, ")") == 0) {
			if (st->size < 1) {
				status = PARSER_ERR_INCORRECT_EXPRESSION;
			}
			while (strcmp(sstack_back(op), "(") != 0) {
				if (!process_op(st, sstack_pop(op))) {
					status = PARSER_ERR_INCORRECT_EXPRESSION;
					break;
				}
			}
			if (status != PARSER_OK) {
				break;
			}
			sstack_pop(op);
			may_unary = false;
		}
		else if (is_op(t)) {
			char curop[TOKEN_SIZE] = "";
			strcpy_s(curop, TOKEN_SIZE, t);
			if (may_unary && (strcmp(curop, "+") == 0 || strcmp(curop, "-") == 0)) {
				curop[0] = -curop[0];
			}
			while (op->size > 0 && (
				!is_unary(curop) && get_priority(sstack_back(op)) >= get_priority(curop) ||
				is_unary(curop) < 0 && get_priority(sstack_back(op)) > get_priority(curop))) {
				if (!process_op(st, sstack_pop(op))) {
					status = PARSER_ERR_INCORRECT_EXPRESSION;
					break;
				}
			}
			if (status != PARSER_OK) {
				break;
			}
			sstack_push(op, curop);
			may_unary = true;
		}
		else if (strcmp(t, ",") == 0) {
			if (op->size < 1) {
				status = PARSER_ERR_INCORRECT_EXPRESSION;
				break;
			}
			while (strcmp(sstack_back(op), "(") != 0) {
				if (!process_op(st, sstack_pop(op))) {
					status = PARSER_ERR_INCORRECT_EXPRESSION;
					break;
				}
			}
			if (status != PARSER_OK) {
				break;
			}
			may_unary = true;
		}
		else {
			_Dcomplex x = { 0 };
			if (is_complex(t)) {
				x = str2complex(t);
			}
			else {
				if (!resolve_name(t, vars, &x)) {
					status = PARSER_ERR_UNDEFINED_NAME;
					break;
				}
			}
			cstack_push(st, x);
			may_unary = false;
		}
	}
	if (status == PARSER_OK) {
		while (op->size > 0) {
			if (!process_op(st, sstack_pop(op))) {
				status = PARSER_ERR_INCORRECT_EXPRESSION;
				break;
			}
		}
		if (st->size != 1) {
			status = PARSER_ERR_INCORRECT_EXPRESSION;
		}
		if (status == PARSER_OK) {
			*buffer = cstack_pop(st);
		}
	}
	sstack_free(tokens);
	cstack_free(st);
	sstack_free(op);
	return status;
}

int parse_expr(const char* expr_, cmap* vars, cvar* destination) {
	char expr[EXPR_SIZE] = "";
	strcpy_s(expr, EXPR_SIZE, expr_);
	remove_spaces(expr);
	if (strlen(expr) == 0) {
		return PARSER_ERR_EMPTY_EXPRESSION;
	}
	if (!check_brackets(expr)) {
		return PARSER_ERR_INCORRECT_BRACKET_PLACEMENT;
	}
	char* name_index = strchr(expr, '=');
	cvar var = { 0 };
	int var_index = -1;
	// если в выражении есть присваивание
	if (name_index != NULL) {
		// определяем имя переменной
		char var_name[VAR_NAME_SIZE] = "";
		for (size_t i = 0; i < (size_t)(name_index - expr); ++i) {
			var_name[i] = expr[i];
		}
		var_index = cmap_find(vars, var_name);
		// если переменная уже существует, будем изменять её значение
		if (var_index != -1) {
			var = vars->data[var_index];
		}
		else {
			strcpy_s(var.name, VAR_NAME_SIZE, var_name);
		}
	}
	// вычисляем выражение через ОПЗ
	int status = solve_with_RPN(name_index == NULL ? expr : (name_index + 1), vars, &var.value);
	if (status != PARSER_OK) {
		return status;
	}
	// заносим результаты
	*destination = var;
	if (var_index != -1) {
		vars->data[var_index] = var;
	}
	else if (name_index != NULL) {
		cmap_insert(vars, var.name, var.value);
	}
	return PARSER_OK;
}
