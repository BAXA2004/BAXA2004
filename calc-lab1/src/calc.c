// Bahrom Hamzayev 241ADB017
// Compile: gcc -O2 -Wall -Wextra -std=c17 -o calc calc.c -lm

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

#ifdef _WIN32
#include <direct.h>
#endif

typedef enum {
    NUM, PLUS, MINUS, MUL, DIV, POW, L_PAR, R_PAR, END, ERR
} TokType;

typedef struct {
    TokType type;
    double val;
    int pos;
} Token;

typedef struct {
    const char* text;
    int index;
    int len;
} Scanner;

Token next_token(Scanner* s) {
    Token t;
    t.pos = s->index + 1;

    // Bo'sh joylarni o'tkazib yuborish
    while (s->index < s->len && isspace(s->text[s->index])) {
        s->index++;
        t.pos = s->index + 1;
    }

    // Fayl oxiri
    if (s->index >= s->len) {
        t.type = END;
        return t;
    }

    char c = s->text[s->index];

    // Sonlarni tekshirish
    if (isdigit(c)) {
        char* end;
        t.val = strtod(s->text + s->index, &end);
        t.type = NUM;
        s->index = end - s->text;
        return t;
    }

    // Operatorlarni tekshirish
    switch (c) {
        case '+': t.type = PLUS; break;
        case '-': t.type = MINUS; break;
        case '*': 
            if (s->index + 1 < s->len && s->text[s->index + 1] == '*') {
                t.type = POW;
                s->index += 2;
            } else {
                t.type = MUL;
                s->index++;
            }
            return t;
        case '/': t.type = DIV; break;
        case '(': t.type = L_PAR; break;
        case ')': t.type = R_PAR; break;
        default: t.type = ERR; break;
    }
    s->index++;
    return t;
}

double parse_expr(Scanner* s, int* err_pos);

double parse_primary(Scanner* s, int* err_pos) {
    Token t = next_token(s);
    
    if (t.type == NUM) {
        return t.val;
    }
    
    if (t.type == L_PAR) {
        double res = parse_expr(s, err_pos);
        if (*err_pos) return 0;
        
        t = next_token(s);
        if (t.type != R_PAR) {
            *err_pos = t.pos;
            return 0;
        }
        return res;
    }
    
    *err_pos = t.pos;
    return 0;
}

double parse_power(Scanner* s, int* err_pos) {
    double left = parse_primary(s, err_pos);
    if (*err_pos) return 0;
    
    Token t = next_token(s);
    
    if (t.type == POW) {
        double right = parse_power(s, err_pos);
        if (*err_pos) return 0;
        left = pow(left, right);
    } else {
        // Token ni qaytarish
        s->index = t.pos - 1;
    }
    
    return left;
}

double parse_term(Scanner* s, int* err_pos) {
    double left = parse_power(s, err_pos);
    if (*err_pos) return 0;
    
    Token t = next_token(s);
    
    while (t.type == MUL || t.type == DIV) {
        double right = parse_power(s, err_pos);
        if (*err_pos) return 0;
        
        if (t.type == MUL) {
            left *= right;
        } else {
            if (fabs(right) < 1e-12) {
                *err_pos = t.pos;
                return 0;
            }
            left /= right;
        }
        t = next_token(s);
    }
    
    // Token ni qaytarish
    if (t.type != END) {
        s->index = t.pos - 1;
    }
    return left;
}

double parse_expr(Scanner* s, int* err_pos) {
    double left = parse_term(s, err_pos);
    if (*err_pos) return 0;
    
    Token t = next_token(s);
    
    while (t.type == PLUS || t.type == MINUS) {
        double right = parse_term(s, err_pos);
        if (*err_pos) return 0;
        
        if (t.type == PLUS) {
            left += right;
        } else {
            left -= right;
        }
        t = next_token(s);
    }
    
    // Token ni qaytarish
    if (t.type != END) {
        s->index = t.pos - 1;
    }
    return left;
}

double calculate(const char* input, int* err_pos) {
    Scanner s;
    s.text = input;
    s.index = 0;
    s.len = strlen(input);
    
    *err_pos = 0;
    
    // Bo'sh satr tekshirish
    if (s.len == 0) {
        *err_pos = 1;
        return 0;
    }
    
    // Izoh tekshirish
    int i = 0;
    while (i < s.len && isspace(input[i])) i++;
    if (i < s.len && input[i] == '#') {
        *err_pos = 1;
        return 0;
    }
    
    double res = parse_expr(&s, err_pos);
    if (*err_pos) return 0;
    
    // Qolgan tokenlar tekshirish
    Token t = next_token(&s);
    if (t.type != END) {
        *err_pos = t.pos;
        return 0;
    }
    
    return res;
}

void handle_file(const char* in_path, const char* out_dir, 
                 const char* first_name, const char* family_name, const char* id) {
    FILE* f = fopen(in_path, "r");
    if (!f) {
        perror("File open error");
        return;
    }

    char data[10001] = {0};
    size_t bytes = 0;
    char buf[1024];
    
    while (fgets(buf, sizeof(buf), f)) {
        // Izoh qatorlarini o'tkazib yuborish
        int is_comment = 0;
        for (int i = 0; buf[i] && isspace(buf[i]); i++) {
            if (buf[i] == '#') {
                is_comment = 1;
                break;
            }
        }
        if (is_comment) continue;
        
        size_t n = strlen(buf);
        if (bytes + n < sizeof(data)) {
            strcat(data, buf);
            bytes += n;
        }
    }
    fclose(f);

    // Yangi qatorni olib tashlash
    if (bytes > 0 && data[bytes - 1] == '\n') {
        data[bytes - 1] = '\0';
    }

    int err_loc = 0;
    double res = calculate(data, &err_loc);

    const char* base = strrchr(in_path, '/');
    if (!base) base = in_path;
    else base++;
    
    char name_no_ext[256];
    strncpy(name_no_ext, base, sizeof(name_no_ext) - 1);
    name_no_ext[sizeof(name_no_ext) - 1] = '\0';
    
    char* dot = strrchr(name_no_ext, '.');
    if (dot && strcmp(dot, ".txt") == 0) *dot = '\0';
    
    char out_file[512];
    int n = snprintf(out_file, sizeof(out_file), 
                    "%s/%s_%s_%s_%s.txt", 
                    out_dir, name_no_ext, first_name, family_name, id);
    
    if (n >= (int)sizeof(out_file)) {
        fprintf(stderr, "Filename too long: %s\n", base);
        return;
    }

    FILE* out = fopen(out_file, "w");
    if (!out) {
        perror("Output file error");
        return;
    }

    if (err_loc) {
        fprintf(out, "ERROR:%d\n", err_loc);
    } else {
        if (fabs(res - round(res)) < 1e-12) {
            fprintf(out, "%.0f\n", res);
        } else {
            fprintf(out, "%.15g\n", res);
        }
    }
    fclose(out);
    
    printf("Done: %s -> %s\n", in_path, out_file);
}

int main(int argc, char* argv[]) {
    char* input_folder = NULL;
    char* output_folder = NULL;
    char* single_file = NULL;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dir") == 0) {
            if (i + 1 < argc) input_folder = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output-dir") == 0) {
            if (i + 1 < argc) output_folder = argv[++i];
        } else {
            single_file = argv[i];
        }
    }

    char* first_name = "Bahrom";
    char* family_name = "Hamzayev"; 
    char* id = "241ADB017";

    char def_out_dir[256];
    if (!output_folder) {
        snprintf(def_out_dir, sizeof(def_out_dir), "labs_%s_%s", first_name, id);
        output_folder = def_out_dir;
    }
    
#ifdef _WIN32
    _mkdir(output_folder);
#else
    mkdir(output_folder, 0775);
#endif

    if (input_folder) {
        DIR* d = opendir(input_folder);
        if (!d) {
            perror("Directory error");
            return 1;
        }
        
        struct dirent* e;
        while ((e = readdir(d))) {
            if (strstr(e->d_name, ".txt")) {
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", input_folder, e->d_name);
                handle_file(path, output_folder, first_name, family_name, id);
            }
        }
        closedir(d);
    } else if (single_file) {
        handle_file(single_file, output_folder, first_name, family_name, id);
    } else {
        fprintf(stderr, "Use: %s [-d DIR] [-o OUT_DIR] file.txt\n", argv[0]);
        return 1;
    }

    return 0;
}