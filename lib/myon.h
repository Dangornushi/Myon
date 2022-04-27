#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#define PATH_SIZE 1024
#define MAX_INDEX 9024
#define MAX_FILE 1024

extern char *file_name;
extern int lang;

typedef enum { true=1, false=0 } bool;

typedef enum {
    TY_NUM,
    TY_IDENT,
    TY_OP,
    TY_STR,
    TY_EOF,
} Typekind;

typedef struct {
    int val;
    char *name;
    char *type;
} Mem;

typedef struct {
    int val;
    char *name;
    char *type;
} Arg;

typedef struct {
    int indent;
} Python;

typedef struct {
    // 変数
    Typekind ty[MAX_INDEX/1024];
    Arg arg[MAX_INDEX];
    Mem sub[MAX_INDEX];
    Mem mem[MAX_INDEX];

    char tok[MAX_INDEX][MAX_INDEX];
    char *write_data;
    char *var;
    char *function_name;
    char *function_s[MAX_INDEX];
    char *ret_var;
    char *tmp1;
    char *pyClsArg;
    char *end;

    int function_index[MAX_INDEX];
    int mem_size;
    int tmp_mem_size;
    int ti;
    int tyi;
    int ret_b;
    int func_name_quan;
    int tmp_ti;
    int number_tmp_val;
    int indent;
} Node;

int varToInt(char *val);
int equal(char *str);
void expect(char *str);
void post(char *str);
int digitSize(int n);
char *type_comp(char* type);

Node *composition();
Node *includeDatas();
Node *classDef();
Node *personal();
Node *publics();
Node *function();
Node *sent();
Node *phrase();
Node *funcCall();
Node *add_sub();
Node *mul_div();
Node *word();

void tokenize(char *data);
void parse(char token_s[1024][1024], Typekind ty[1024]);
