# JOINアルゴリズム実装課題
一次元の整数の配列をINNER JOINするアルゴリズムをC言語で実装しました。
あくまでもアルゴリズムの概要を知るためだけなので、効率的な実装ではありません。

- Nested Loop Join
- Merge Join
- Hash Join

## 入力データの生成
駆動表と内部表のデータを以下の形式で生成します。

```
N_outer
OD_1
OD_2
...
OD_n
M_inner
ID_1
ID_2
...
ID_m
```

それぞれのプログラムの中では駆動表と内部表を一次元の配列にします。

## コンパイルと実行
データを生成したファイルを引数に渡してください。

```
$ gcc gcc nested_loop_join.c -o nested_loop
$ go run generate.go -c 10 -m 30 > input99.txt
$ ./nested_loop input99.txt
```
