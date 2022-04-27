/*
 * 関数の戻り根について:
 * 0 == False
 * 1 == True
 *
 * langについてэ
 * 0 == C
 * 1 == Python
 */

/*
 * TODO:
 * classのC言語対応
 *
 * ```
 * // メンバ関数群
 * ~~~
 * ~~~
 * // ---
 *
 * struct CLASSNAME {
 *     //関数をなんとかしてテーブルか何かにして記憶
 * }
 * ```
 *
 * Instance : 
 * 構造体のインスタンス
 *
 * Function call :
 * CLASSNAME.FUNCNAMES[FUNCNAMESEARCH(FUNCNAME)];
 *
 * 宣伝と広報用のHP作ってみる
 * Ubuntu Server立ててみる
 * Serverの立て方調べてみる
 * JavascriptとCSSとHTML
 */

#include "myon.h"

int b1 = 0;
int wl         = 0;
int arg_size   = 0;
int calc_index = 0;
char *calc_ary[MAX_INDEX];

Node *node;

int array_index(char *str, char *array[MAX_INDEX], int size) {
    for (int s=0;s < size;s++) {
        if (!strcmp(array[s],str)) {
            return s;
        }
    }
    printf("Can't find '%s'\n", str);
    return -1;
}

int mem_index(char *str) {
    for (int s=0;s < node->mem_size;s++) {
        if (!strcmp(node->mem[s].name,str)) {
            return s;
        }
    }
    return -1;
}

int equal(char *str) {
    if (!strcmp(node->tok[node->ti], str)) {
        return 0;
    }
    return 1;
}

int next_equal(char *str) {
    if (!strcmp(node->tok[node->ti+1], str)) {
        return 0;
    }
    return 1;
}

void expect(char *str) {
    if (equal(str)) {
        printf("Expected : '%s' but '%s'\n", str, node->tok[node->ti]);
        exit(1);
    }
    node->ti++;
}

void func_arg_call() {
    node->arg[arg_size].type = type_comp(node->tok[node->ti++]);
    node = add_sub();
    node->arg[arg_size++].name = node->var;
    node->ti++;
    if (!equal(",")) {
        node->ti++;
        func_arg_call();
    }
}

void arg_call() {
    node = add_sub();
    node->arg[arg_size++].name = node->var;
    node->ti++;
    if (!equal(",")) {
        node->ti++;
        arg_call();
    }
}

void mov(char *name, int val) {
    node->mem[node->mem_size].name = name;
    node->mem[node->mem_size++].val = val;
}

int isDigit(char *str) {
    for (int i=0;str[i]!='\0';i++) {
        if (!isdigit(str[i])) return 1;
    }
    return 0;
}

int varToInt(char *val) {
    // 数字
    if (!isDigit(val)) {
        return atoi(val);
    }
    // 文字列定数
    if (mem_index(val) == -1) {
        char tmp[MAX_INDEX];
        sprintf(tmp, "%d", node->var[0]);
        return atoi(tmp);
    }
    // 変数
    else {
        return node->mem[mem_index(val)].val;
    }
}

char *varOrconst(char *val) {
    char *tmp, *var_name, *define_sent, *ret_var_name;
    var_name = NULL;
    // 数字
    if (!isDigit(val)) {
        return val;
    }
    // 変数
    else {
        return val;
    }

}

int comparison() {
    char *l, *r, *op;

    node = add_sub();
    post(node->var);
    op = node->tok[++node->ti];
    post(op);
    node->ti++;
    node = add_sub();node->ti++;
    post(node->var);
    return 0;
}

void post(char *str) {
    char *tmp_ptr = NULL;
    tmp_ptr = (char*)realloc(node->write_data, 1024 );//strlen(node->write_data)+strlen(str)+1);
    if (tmp_ptr == NULL) { /* 失敗時 */
        printf("メモリーの確保に失敗しました\n");
        free(node->write_data); // 必要に応じて元オブジェクトを解放
        return;
    }
    if (node->write_data != tmp_ptr){
        node->write_data = tmp_ptr;
    }
    sprintf(node->write_data, "%s%s", node->write_data, str);
}

int digitSize(int n) {
    int i;
    if (n == 0) {
        return 1;
    }
    while (n!=0) {
        n /= 10;
        ++i;
    }
    return i;
}

void writeToFile() {
    FILE *wfp;
    char *c_file_name = malloc(strlen(file_name)+2);

    if (lang == 0)
        sprintf(c_file_name, "%s.c", file_name);
    if (lang == 1)
        sprintf(c_file_name, "%s.py", file_name);
    wfp = fopen(c_file_name, "w");
    fprintf(wfp, "%s", node->write_data);
    fclose(wfp);

    free(c_file_name);
}

char *type_comp(char* type) {
    if (!strcmp(type, "int")) {
        if (lang == 0) return "long long";
        if (lang == 1) return "int";
    }
    if (!strcmp(type, "str")) {
        return "char*";
    }
    return "void";
}

int main_b = 1;

Node *composition() {
    while (main_b) {
        if (!equal("include")) {
            node->ti++;
            node = includeDatas();
        }
        if (!equal("fn")) {
            node->ti++;
            node = function();
        }
        if (!equal("class")) {
            node->ti++;
            node = classDef();
        }
        else {
            node->ti++;
            node = composition();
        }
    }
    return node;
}

Node *includeDatas() {
    char *openFileName, openFileData[MAX_FILE];
    FILE *ofnf = NULL;

    openFileName = node->tok[node->ti++];
    if ( NULL == (ofnf = fopen(openFileName, "r"))) {
        printf("%s : ファイル読み込み不可\n", openFileName);
        exit(1);
    }
    while (fgets(openFileData, MAX_FILE, ofnf) != NULL) {
        post(openFileData);
    }
    return node;
}

/*
* class Main {
*     @personal {
*         hoge
*     }
*     @public {
*         hoge
*     }
*
*     fn inMain(Main, ...) <- int {
*         //
*         return 0;
*     }
* }
*/

char *class_name;

Node *classDef() {
    char *class_arg;

    class_name = malloc(strlen(node->tok[node->ti])+2);
    class_name[0] = 'O';
    class_name[1] = 'b';
    for (int i=0;i<strlen(node->tok[node->ti]);i++) {
        class_name[i+2] = node->tok[node->ti][i];
    }
    node->ti++;
    expect("{");
    if (!equal("@") && !strcmp(node->tok[++node->ti], "import")) {
        char *name, *c_name, *type, *var, *tmp;

        node->ti++;
        name = node->tok[node->ti++];
        expect(":");
        type = node->tok[node->ti++];
        expect(";");

        if (lang==0) {
            // C
        }
        if (lang==1) {
            // Python
            class_arg = name;
        }
    }
    if (lang==0) {
        post("struct ");
        post(class_name);
        post("{\n");
    }
    if (lang==1) {
        post("class ");
        post(class_name+2);
        post(":\n\tdef __init__(");
        post(class_name);
        post(", ");
        post(class_arg);
        post("):\n");
        node->indent+=2;
    }
    if (!equal("@")) {
        node->ti+=2;
        expect("{");
        node = personal();
        expect("}");
    }
    node = publics();
        node->indent--;
    if (lang==0) {
        //C
    }
    if (lang==1) {
        post("\tdef myon_put(__word):\n\t\tprint(__word)\n");
        // Python
        node->pyClsArg = class_name;
        if (!equal("fn")) {
            node->ti++;
            node = function();
        }
    }
    expect("}");
    if (lang==0) post("};\n");
    if (lang==1) node->indent--;
    return node;
}

Node *personal() {
    char *name, *c_name, *type, *var, *tmp;

        c_name = class_name;//node->tok[];
        node->ti++;
        expect(".");
        name = node->tok[node->ti++];
        expect(":");
        type = node->tok[node->ti++];
        expect("<-");
        node = word();node->ti++;
        var = node->var;
        expect(";");


        // トランスパイルされたコード
        if (lang==0) {
            // C
            tmp = malloc(strlen(type)+strlen(name)+1);
            sprintf(tmp, "%s %s", type, name);
            post(tmp);
            free(tmp);
            post(";\n");
        }
        if (lang==1) {
            // Python
            for (int i=0;i<node->indent;i++) post("\t");
            if (!strcmp(type,"int")) {
                tmp = malloc(strlen(c_name)+strlen(name)+strlen(var)+2);
                sprintf(tmp, "%s.%s=%s", c_name,  name, var);
            }
            if (!strcmp(type,"str")) {
                tmp = malloc(strlen(c_name)+strlen(name)+strlen(var)+(sizeof(char*)*2)+1);
                sprintf(tmp, "%s.%s=\"%s\"", c_name,  name, var);
            }
            post(tmp);
            free(tmp);
            post("\n");
        }
        if (node->tok[node->ti][0] != '}') node = personal();
    return node;
}

Node *publics() {
    return node;
}

Node *function() {
    char *function_args;
    char *type;
    char tmp[MAX_INDEX];
    int arg_val[MAX_INDEX], i;

    node->function_name = node->tok[node->ti];
    node->ti++;
    expect("(");
    function_args = malloc(sizeof(char*));
    strcpy(function_args, "");

    // void 以外の型明記
    if (strcmp(node->tok[node->ti], "void")) {
        for (i=0;i<arg_size;i++) {
            arg_val[i] = node->arg[i].val;
        }
        arg_size = 0;
        if (lang == 0) {
            func_arg_call();
            for (i=0;i<arg_size;i++) {
                function_args = realloc(function_args, strlen(function_args)+strlen(node->arg[i].type)+strlen(node->arg[i].name)+1);
                if (function_args == NULL) {
                    printf("メモリエラー\n");
                    exit(1);
                }
                sprintf(function_args, "%s%s %s", function_args, node->arg[i].type, node->arg[i].name);
            }
        }
        if (lang == 1) {
            func_arg_call();
            for (i=0;i<arg_size;i++) {
                function_args = realloc(function_args, strlen(function_args)+strlen(node->arg[i].name)+1);
                if (function_args == NULL) {
                    printf("メモリエラー\n");
                    exit(1);
                }
                sprintf(function_args, "%s%s", function_args, node->arg[i].name);
            }

        }
    }
    // void表記である
    if (!strcmp(node->tok[node->ti],"void")) {
        node->ti++;
        if (lang == 0) sprintf(function_args, "void");
    }
    // 表記なし
    else ;

    expect(")");
    expect("<-");
    node = word();node->ti++;
    type = type_comp(node->var);
    expect("{");
    if (!strcmp(node->function_name, "main")) main_b = 0;

    // トランスパイルされたコードを生成するフェーズ
    // tmp : 生成された関数定義を格納するデータ
    if (lang == 0) {
        // C --------
        sprintf(tmp, "%s myon_%s(", type, node->function_name);
        sprintf(tmp, "%s%s) {\n", tmp, function_args);
        post(tmp);
    }
    if (lang == 1) {
        // python --------
        for (int i=0;i<node->indent;i++) post("\t");
        sprintf(tmp, "def myon_%s(", node->function_name);
        sprintf(tmp, "%s%s) :\n", tmp, function_args);
        node->indent++;
        post(tmp);
    }

    node = sent();
    if (lang == 0) post("}\n");
    free(function_args);
    node->indent--;

    return node;
}

Node *sent() {
    char tmp[MAX_INDEX], *var_name;
    node = funcCall();

    if (!equal("let")) {
        /*
         * 変数定義
         * let var_name : type <- var ;
         */
        char *type, *def;
        int var, usedMalloc, len;
        char *var_data;

        node->ti++;
        node = word();node->ti++;
        var_name = node->var;

        expect(":");

        type = node->tok[node->ti++];


        if (equal("<-") && equal("is")) {
            printf("SynErr : %s\n", node->tok[node->ti++]);
            exit(1);
        }
        else node->ti++;

        // 変数の方が文字列
        if (!strcmp(type, "str")) {
            // C
            if (!equal("[")) {
                node->ti++;    // [
                node = add_sub();node->ti++;
                char *index = node->var;
                node->ti++;    // ]

                // コード生成フェーズ
                if (lang == 0) {
                    // C
                    post("u_char *");
                    post(var_name);
                    post("[");
                    post(index);
                    post("];\n");
                }
                if (lang == 1) {
                    // Python
                    for (int i=0;i<node->indent;i++) post("\t");
                    post(var_name);
                    post("= [0 for i in range(");
                    post(index);
                    post(")]\n");
                }
                //----------
                node = sent();
                free(def);
                return node;
            }
            else {
                len = strlen(node->tok[node->ti]);
                node = add_sub();node->ti++;
                var_data = node->var; //varOrconst(node->var);
                def = malloc(2+len);
                if (lang == 0) {
                    // C
                    if (len > 1) {
                        post("u_char*");
                        post(" ");
                        post(var_name);
                        post("=");
                        sprintf(def, "\"%s\"", var_data);
                    }
                    else {
                        post("char");
                        post(" ");
                        post(var_name);
                        post("=");
                        sprintf(def, "'%s'", var_data);
                    }
                }
                if (lang == 1) {
                    // Python
                    for (int i=0;i<node->indent;i++) post("\t");
                    post(var_name);
                    post("=");
                    sprintf(def, "\"%s\"", var_data);
                }
            }
        }
        if (!strcmp(type, "int")) {
            // Python
            node = add_sub();node->ti++;
            //ここまで動作確認-----------------------------
            var_data = node->var; //varOrconst(node->var);
            def = malloc(digitSize(atoi(var_data)));
            if (def == NULL) {
                printf("メモリエラー\n");
                free(def);
                exit(1);
            }
            else {
                if (lang == 0) {
                    // C
                    post("unsigned long");
                    post(" ");
                }
                if (lang == 1) {
                    // Python
                    for (int i=0;i<node->indent;i++) post("\t");
                }
                post(var_name);
                post("=");
                sprintf(def, "%s", var_data);
            }
        }
        if (!strcmp(type, "class")) {
            char *cls_name;

            node = add_sub();node->ti++;
            var_data = node->var; //varOrconst(node->var);
            cls_name = var_data+5;
            def = malloc(strlen(var_name)+strlen(cls_name)+9);
            sprintf(def, "myon_%s = %s", var_name,  cls_name);
        }
        var_data = NULL;
         free(var_data);

        expect(";");

        /*
         * コード生成フェーズ
        */
        post(def);
        if (lang == 0) post(";");

        post("\n");
        free(def);
        node = sent();
    }
    else if (!next_equal("=") || !next_equal("is") || !equal("=") || !equal("is")) {
        node->ti--;
        var_name = node->var;
        post(var_name);
        node->ti++;
        if (!next_equal("=") || !next_equal("is")) node->ti++;
        if (equal("=") && equal("is")) {
            printf("SynErr : %s\n", node->tok[node->ti++]);
            exit(1);
        }
        else node->ti++;
        post("=");
        node = add_sub();node->ti++;
        post(node->var);
        expect(";");
        if (lang == 0) post(";\n");
        node = sent();
    }
    else if (!equal("do")){
        node->ti++;
        node = funcCall();node->ti++;
        expect(";");
        //node = sent();
    }
    else if (!equal("put")) {
        char *sentens;
        node->ti++;
        node = add_sub();node->ti++;
        sentens = malloc(7+strlen(node->var));
        if (lang == 0) {
            sprintf(sentens, "put(%s);\n", node->var);
            post(sentens);
            expect(";");
        }
        if (lang == 1) {
            for (int i=0;i<node->indent;i++) post("\t");
            post("print(");
            post(node->var);
            post(")\n");
            expect(";");
        }
        free(sentens);
        node = sent();
    }
    else if (!equal("strput")) {
        node->ti++;
        node = add_sub();node->ti++;
        if (mem_index(node->var) == -1) {
            printf("%c", atoi(node->var));
        }
        else {
            int base_var_data = node->mem[mem_index(node->var)].val;
            printf("%c", base_var_data);
        }
        expect(";");
        //node = sent();
    }
    else if (!equal("strputs")) {
        node->ti++;
        node = add_sub();node->ti++;
        if (mem_index(node->var) == -1) {
            printf("%s", node->var);
        }
        else {
            int i;
            for (i=mem_index(node->var)+1;i<node->mem[mem_index(node->var)].val+1;i++) {
                printf("%c", node->mem[i].val);
            }
        }
        expect(";");
        //node = sent();
    }
    else if (!equal("return")) {
        node->ti++;
        node = add_sub();node->ti++;
        for (int i=0;i<node->indent;i++) post("\t");
        post("return ");
        post(node->var);
        node->ret_var = node->var;
        //node->ret_b = 0;
        expect(";");
        if (lang == 0) post(";");
        post("\n");
    }
    else if (!equal("input")) {
        int input_data;
        scanf("%d", &input_data);
        node->ti++;
        node = add_sub();node->ti++;
        expect(";");
        //mov(node->var, input_data);
        node->mem[mem_index(node->var)].val = input_data;
    }
    else if (!equal("loop")) {
        int loop_bool, c;
        c = 0;
        node->ti++;
        if (lang == 0) {
            post("while (");
            comparison();
            post(") {");
        }
        if (lang == 1) {
            for (int i=0;i<node->indent;i++) post("\t");
            post("while (1):\n");
            post("if ");
            comparison();
            post(":\n\tbreak");
            node->indent++;
        }
        expect("{");
        while (1) {
            if (!equal("if") || !equal("loop"))
                c++;
            if (!equal("}")) {
                if (c==0)
                    break;
                c--;
            }
            node = sent();
        }
        expect("}");
        if (lang == 0) post("}");
        if (lang == 1) {
            node->indent--;
        }
        node = sent();
    }
    else if (!equal("if")) {
        int if_bool, c;
        c = 0;
        node->ti++;
        if (lang == 0) {
            post("if (");
            comparison();
            post(") {");
        }
        if (lang == 1) {
            for (int i=0;i<node->indent;i++) post("\t");
            post("if ");
            comparison();
            post(":\n");
            node->indent++;
        }
        expect("{");
        while (1) {
            if (!equal("if") || !equal("loop"))
                c++;
            if (!equal("}")) {
                if (c==0)
                    break;
                c--;
            }
            node = sent();
        }
        expect("}");
        node->indent--;
        if (lang == 0) post("}");
        node = sent();
    }

    else if (!equal(";") || !next_equal(";")) {
        if (lang == 1) post("\n");
        if (node->var[0] != ';') {
            for (int i=0;i<node->indent;i++) post("\t");
            post(node->var);
        }

        if (lang == 0) post(";");
        post("\n");
        node->ti++;
        node = sent();
    }
    return node;
}

Node *add_sub() {
    char tmp[MAX_INDEX], *op, *l, *r;
    node = mul_div();
    if (!next_equal("+") || !next_equal("-")) {
        l = node->var;
        node->ti+=2;
        op = node->tok[node->ti-1];
        node = add_sub();//node->ti++;
        r = node->var;
        if (op[0] == '+')
            sprintf(tmp, "%s + %s", l, r);
        else
            sprintf(tmp, "%s - %s", l, r);
        node->var = tmp;
    }
    else
        return node;
    calc_index = 0;
    return node;
}

Node *mul_div() {
    char tmp[MAX_INDEX], *op;
    int l, r;
    node = funcCall();
    if (!next_equal("*") || !next_equal("/")) {
        l = varToInt(node->var);
        node->ti+=2;
        op = node->tok[node->ti-1];
        node = funcCall();
        r = varToInt(node->var);
        if (op[0] == '*')
            sprintf(tmp, "%d", l * r);
        else
            sprintf(tmp, "%d", l / r);
        node->var = tmp;
    }
    else
        return node;
    calc_index = 0;
    return node;
}

Node *funcCall() {
    char *tmp_func, *tmp_var, *tmp_write_data, *func_call_sent;
    int goto_index, tmp_ti, tmp_mem_size;
    node = word();
    // 呼び出される関数名
    char *call_func_name = node->var;
    if (!next_equal("(")) {
        // 関数呼び出しの引数のインデックス、リセットして内容をカバーする
        arg_size = 0;
        // next_equalの分
        node->ti++;
        expect("(");
        func_call_sent = NULL;
        func_call_sent = malloc(strlen(call_func_name)+7);
        if (func_call_sent == NULL) exit(1);
        if (strcmp(call_func_name, "put")) {
            if (lang == 1) for (int i=0;i<node->indent;i++) post("\t");
            sprintf(func_call_sent, "myon_%s", call_func_name); // printでなければの処理
        }
        else {
            if (lang == 1) {
                for (int i=0;i<node->indent;i++) post("\t");
                sprintf(func_call_sent, "print");
            }
            if (lang == 0) {
                sprintf(func_call_sent, "put");
            }
        }
        //strcat(func_call_sent, call_func_name);
        strcat(func_call_sent, "(");
        // 引数を審査して構造体(arg)に格納
        if (node->ty[node->ti] != TY_IDENT) {
            arg_call();
            for (int i=0;i<arg_size;i++) {
                char *tmp_ptr = NULL;
                if ((tmp_ptr = realloc(func_call_sent, strlen(func_call_sent) + strlen(node->arg[i].name)+1)) == NULL) {
                    exit(1);
                }
                func_call_sent = tmp_ptr;
                sprintf(func_call_sent, "%s%s", func_call_sent, node->arg[i].name);
            }
            char *tmp_ptr = realloc(func_call_sent, strlen(func_call_sent) + 1);
            if (tmp_ptr == NULL) {
                exit(1);
            }
            func_call_sent = tmp_ptr;
        }
        else {
            node->ti++;
        }

        strcat(func_call_sent, ")");
        expect(")");
        node->ti--;
        node->var =  func_call_sent;
        free(func_call_sent);
    }
    return node;
}

/*
 * wordまでは完璧
 * そこから先にBugあり
 * */
char *tmp[1024];
char *c_data = "";
int c=0;
char *tmp_var;

Node *word() {
    if (!next_equal(".")) {
        char *name = NULL;
        char *p, *t2;

        name = node->tok[node->ti++];
        node->ti++;
        tmp[c++] = name;
        b1 = 1;
        node->var = name;
        node = funcCall();

        return node;
    }
    else if (b1!=0) {
        char *word = NULL;
        char *p = NULL;
        char *p2 = NULL;
        char *now_sent = NULL;

        if (node->tok[node->ti+1][0] == '(') {
            now_sent = malloc(1+strlen(node->tok[node->ti])+sizeof(char*)*5);
            sprintf(now_sent, "myon_%s", node->tok[node->ti]);
        }
        else {
            now_sent = malloc(strlen(node->tok[node->ti])+1);
            strcpy(now_sent, node->tok[node->ti]);
        }

        word = (char*)malloc(strlen("a"));

        for (int i=0;i<c;i++) {
            p = (char*)realloc(word, strlen(word)+strlen(tmp[i])+sizeof(char*));

            if (p == NULL) {
                printf("Memory Err.\n");
                exit(1);
            }
            if (p != word) {
                word = p;
            }
            sprintf(word, "%s%s.", word, tmp[i]);
        }
        p2 = realloc(word, strlen(word)+strlen(now_sent)+1);
        if (p2 == NULL) {
            printf("Memory Err\n");
            exit(1);
        }
        if (p2 != word) {
            word = p2;
        }
        sprintf(word, "%s%s", word, now_sent);//+5);
        strcpy(node->var, word);
        free(word);
        free(now_sent);

        b1 = 0;
        c= 0;
        return node;
    }
    if (!next_equal("[")) {
        char *name = node->tok[node->ti++];
        char *tmp_ptr = malloc(strlen(name)+strlen(node->tok[++node->ti])+2);
        sprintf(tmp_ptr, "%s[%s]", name, node->tok[node->ti++]);
        node->var = tmp_ptr;
        free(tmp_ptr);//node->ti++;
        return node;
    }
    if (!next_equal("++")) {
        char *tmp_ptr;
        node->var = node->tok[node->ti++];
        tmp_ptr   = malloc(strlen(node->var)+3);
        sprintf(tmp_ptr, "%s++", node->var);
        node->var = tmp_ptr;
        free(tmp_ptr);//node->ti++;
        return node;
    }
    node->var = node->tok[node->ti];
    return node;
}

void parse(char token_s[1024][1024], Typekind ty[1024]) {
    node = calloc(1, sizeof(Node));
    node->write_data = NULL;
    int i;
    for (i=0;token_s[i][0]!='\0';i++) {
        for (int j=0;token_s[i][j]!='\0';j++)
            node->tok[i][j] = token_s[i][j];
        node->ty[i] = ty[i];
    }
    node->ti             = 0;                     // 今のトークン配列のインデックス
    node->tyi            = 0;                     // 今のタイプ格納配列のインデックス
    node->ret_b          = 1;                     // returnされたか否かの判定
    node->mem_size       = 0;                     // 記関数定義などスコープ変更の際に書き換えられる憶領域のインデックス
    node->func_name_quan = 0;                     // 関数定義の通算
    node->number_tmp_val = 0;                     // 一時的変数の名称インデックス
    node->write_data     = malloc(sizeof(char*)*2); // トランスパイルされたC言語ファイル
    // Pythonのインデックス
    node->indent = 0;

    node = composition();

    if (lang == 0) {

        char *tmp_ptr      = NULL;
        char *include_sent = "#include \"cache/common.h\"\n";

        tmp_ptr = malloc(1+strlen(node->write_data)+strlen(include_sent));
        if (tmp_ptr == NULL) { /* 失敗時 */
            printf("メモリーの確保に失敗しました\n");
            free(node->write_data); // 必要に応じて元オブジェクトを解放
            return;
        } else {
        }
        sprintf(tmp_ptr, "%s\n%s", include_sent, node->write_data);
        node->write_data = malloc(strlen(tmp_ptr));

        strcpy(node->write_data, tmp_ptr);
        post("\nint main() { return myon_main(); }\n");
    free(tmp_ptr);
    }
    if (lang == 1) {
        post("\nmyon_main()\n");
    }

    writeToFile();

    free(node->write_data);
    printf("\x1b[34m");
    printf("Successful transpile.\n");
    printf("\x1b[39m");
    printf("\n");
        free(tmp_var);
        free(class_name);
}

/*
 *
 * Myon/
 * ┃
 * ┠ Documents/
 * ┃  ┗ README.md
 * ┃
 * ┠ bin/
 * ┃  ┗ myon
 * ┃
 * ┠ build/
 * ┃  ┣ main.c
 * ┃  ┣ parse.c
 * ┃  ┣ token.c
 * ┃  ┗ myon.h
 * ┃
 * ┠ lib/
 * ┃  ┗ common.h
 *
*/
