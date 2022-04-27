# Myonトランスパイラ

Rustに似たオリジナルの言語をPythonやCに変換するソフトウェア。

This is a software convert to C and Python what original language like Rust.

# DEMO

# Features

インタプリタ・コンパイル型など実行形式や静的・動的／型付・型なしの構造的な問題を克服した言語。

「仕事でPython使うけど文法よくわかんないな」や「C言語って難しそうだよな」なんて時に使える言語変換機。

This is a language that overcame differences of execution formats such as interpreters and compilations.

If you have to use Python or C in your work but you arent know about these languages 

Or if you Has resistance to study C,it will be very usefull.

# Laungages

* C 11
* python 3.8.2

# Grammer

Define as a function

```
fn FunctionName ( Type ArgName ) <- FunctionType { FunctionDatas }
```
:
print something number

```
put ( Number ) ;
```

Define as vartage

```
let VarName : Type <- Data ;
let VarName : Type is Data ;
```

# Useage

build

```fish
git clone https://github.com/Dangornushi/Myon/
cd Myon
make
```

Run(trancepile to C)

```fish
./myon hoge.my
```

Compile and run (C)

```fish
./myon -r hoge hoge.my
```

Trancepile to the Python and Run

```
./myon -pyrun hoge.py hoge.my
```

# Note

* エラー表示にバグがある可能性
* 機能など追加実装あり

# Author

* Made by Dangomushi
* Gmail: dangomushi150@gmail.com
* Twiiter: dangomu39300363
