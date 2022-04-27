#include "myon.h"


typedef struct {
    char tokens[1024][1024];
    Typekind nd_s[1024][1024];
} Token;

void tokenize(char *data) {
    Typekind ty;
    char *kw[] = {
        "let",
        "str",
        "int",
        "void",
        "fn",
        "is",
        "<-",
        "+=",
        "-=",
        "++",
        "--",
        ">>",
        "do",
        "==",
        "pass",
        "put",
        "strput",
        "strputs",
        "input",
        "if",
        "loop",
        "class",
        "include",
        "@personal",
        "@import",
        "'s",
        "this",
    };
    char base[1024];
    int i = 0;
    char token_s[1024][1024];
    Typekind nd_s[1024];
    int tc = 0;
    int bc = 0;
    int flag;
    int sk_op = 0;
    int str_skip = 1;

    while (data[i]!='\0') {
        switch (data[i]) {
            case '\n':
                break;
            case ';': case '=':
            case '+': case '-':
            case ':': case '*':
            case '/': case '(':
            case ')': case '[':
            case ']': case '{':
            case '}': case ' ':
            case '<': case '>':
            case ',': case '.':
            case '?': case '!':
            case '@': case '_':
            case '\'': case '|':
                {
                sk_op = 0;
                if (data[i] == ' ' && data[i+1] == ' ') {
                    i++;
                    break;
                }
                if (/*isspace(data[i]) && */str_skip == 0) {
                    ty = TY_STR;
                    base[bc++] = data[i];
                    break;
                }
                if (flag) {
                    base[bc] = '\0';
                    flag = 1;
                }
                else {base[0] = data[i];}

                // 予約語か？
                for (int j=0;j<sizeof(kw)/sizeof(kw[0]);j++) {
                    if (!strcmp(kw[j], base)) {
                        for (int k=0;k<sizeof(base);k++)
                            token_s[tc][k] = base[k];
                        nd_s[tc] = TY_IDENT;
                        tc++;
                        flag = 0;
                        break;
                    }
                }

                if (*(data+i) == '<' && *(data+i+1) == '-') {
                    sk_op = 1;
                }

                if (data[i] == '+' && data[i+1] == '+') {
                    sk_op = 1;
                }
                if (data[i] == '=' && data[i+1] == '=') {
                    sk_op = 1;
                }

                // 予約後ではなかった
                if (flag==1) {
                    for (int j=0;j<sizeof(base);j++)
                        token_s[tc][j] = base[j];
                    nd_s[tc] = ty;
                    tc++;
                }

                // base clear
                for (int j=0;j<1024;j++) base[j] = 0;
                bc = 0;
                ty = 0;

                // 記号
                if (data[i] != ' ' && sk_op == 0) {
                    char tmp[10];
                    sprintf(tmp,"%s%c","", data[i]);
                    for (int j=0;j<sizeof(base);j++)
                        token_s[tc][j] = tmp[j];
                    nd_s[tc] = TY_OP;
                    tc++;
                }
                if (sk_op == 1) {
                    token_s[tc][0] = data[i];
                    i++;
                    token_s[tc][1] = data[i];
                    nd_s[tc] = TY_IDENT;
                    tc++;
                }
                flag = 0;
                break;
            }
            default: {
                flag = 1;
                if (data[i] == '"') {
                    if (str_skip == 1) {
                        str_skip = 0;
                    }
                    else if (str_skip == 0) {
                        str_skip = 1;
                    }
                }
                if (isdigit(data[i])) {
                    if (ty==TY_STR) ty = TY_STR;
                    else ty = TY_NUM;
                    base[bc++] = data[i];
                    break;
                }
                if ('a' <= data[i] && data[i] <= 'z' || 'A' <= data[i] && data[i] <= 'Z') {
                    ty = TY_STR;
                    base[bc++] = data[i];
                    break;
                }
                break;
            }
        }
        i++;
    }
    token_s[tc][0] = '\0';

    parse(token_s,nd_s);

}

