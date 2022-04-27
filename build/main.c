#include "myon.h"

char *file_name;
int lang;

int main(int argc, char **argsv ) {
    char input[1000000], tmp;
    char *now_dir = NULL;
    char *dir_and_chache = NULL;
    int i = 0;
    FILE *op, *head_fp, *sl_fp;

    file_name = argsv[argc-1];

    if (file_name[0] == '-') {
        if (!strcmp("-start", file_name)) {
            // 現在のフォルダを参照する
            now_dir = getcwd(NULL, 0);
            if (now_dir == NULL) {
                printf("[ERROR] pathname length exceeds the buffer size\n");
                exit(EXIT_FAILURE);
            }

            printf("Now : %s\n", now_dir);

            const char *cache = "cache";
            struct stat stat_buf;

            // cacheファイルが存在する
            if (stat(cache, &stat_buf) == 0) printf("The folder already exists : %s\n", cache);
            // cacheファイルが存在しない
            else {
                if (mkdir(cache, 0777) == 0) {
                    printf("Created a cache directory\n");
                    head_fp = fopen("cache/common.h", "w");
                    fprintf(head_fp,
                            "#include <stdio.h>\n"
                            "#include <string.h>\n"
                            "void put(int num);\n"
                            );
                    fclose(head_fp);
                    sl_fp = fopen("cache/systemLibrary.c", "w");
                    fprintf(sl_fp,
                            "#include \"common.h\"\n"
                            "void put(int num) {\n"
                            "   printf(\"%%d\\n\", num);\n"
                            "}\n"
                            );
                    fclose(sl_fp);
                }
                else {
                    printf("Can't created : %s\n", cache);
                }
            }
        }

        if (!strcmp("-version", file_name)) {
            printf( "This version is %s.\n"
                    "Made by dangomushi.\n", "proto-0");
        }
        if (!strcmp("-log", file_name)) {
            printf(
                "2022/4/3 - Myon kernel is completed [ver.proto-0]\n"
            );
        }
        return 1;
    }

    op = fopen(file_name, "r");

    i=0;
    while ((tmp = fgetc(op)) != EOF) {
        input[i++] = tmp;
    }
    // 初期値を確認する

    if (!strcmp(argsv[1], "-pyrun")) {
        lang = 1;
        tokenize(input);

        fclose(op);

        strcat(file_name, ".py");

        char *sys_run_com_run = NULL;
        sys_run_com_run = malloc(strlen(file_name)+8);

        sprintf(sys_run_com_run, "python3 %s", file_name);

        if (system(sys_run_com_run) == -1) {
            printf("コマンドが実行できませんでした\n");
        }
        free(sys_run_com_run);
    }

    else {
        lang = 0;
        tokenize(input);

        fclose(op);

        if (!strcmp(argsv[1], "-r")) {
            strcat(file_name, ".c");
            char *sys_run_com = malloc(strlen(file_name)+6);
            char *sys_run_com_sl = malloc(strlen("cache/systemLibrary.c")+6);
            char *sys_run_com_compile = malloc(strlen("cache/systemLibrary.c")+strlen(file_name)+strlen(argsv[2])+6);
            char *sys_run_com_run = malloc(strlen(argsv[2])+2);
            sprintf(sys_run_com, "CC -c %s", file_name);
            sprintf(sys_run_com_sl, "CC -c cache/systemLibrary.c");
            sprintf(sys_run_com_compile, "CC -o %s %s %s", argsv[2], file_name, "systemLibrary.o");
            sprintf(sys_run_com_run, "./%s", argsv[2]);
            if (system(sys_run_com) == -1 || system(sys_run_com_sl) == -1 || system(sys_run_com_compile) == -1 || system(sys_run_com_run) == -1) {
                printf("コマンドが実行できませんでした\n");
            }
            free(sys_run_com);
            free(sys_run_com_sl);
            free(sys_run_com_compile);
            free(sys_run_com_run);
        }
    }

    free(now_dir);
    //free(dir_and_chache);

    return 0;
}

