// calc.h - Header file for the calculator program

#ifndef CALC_H
#define CALC_H

#include <stddef.h>

typedef enum {
    TOK_NUMBER, TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_POW,
    TOK_LPAREN, TOK_RPAREN, TOK_EOF, TOK_ERROR
} TokenType;

typedef struct {
    TokenType type;
    double value;
    int start_pos;
} Token;

double evaluate_expression(const char* input, int* error_pos);
void process_file(const char* input_path, const char* output_dir, 
                  const char* name, const char* lastname, const char* studentid);

#endif // CALC_H