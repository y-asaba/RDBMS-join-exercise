#include <stdio.h>
#include <stdlib.h>

int *scan_data(FILE *fp, int *size)
{
    char buf[64];
    int i;
    int *table;

    /*
     * 最初のNを読み込む
     */
    fgets(buf, sizeof(buf), fp);
    sscanf(buf, "%d", size);

    if (*size <= 0)
	return NULL;

    /*
     * 必要な配列を確保
     */
    table = (int *) malloc(sizeof(int) * *size);
    if (table == NULL)
	return NULL;

    /*
     * データ読み込み
     */
    for (i = 0; i < *size; i++) {
	fgets(buf, sizeof(buf), fp);
	sscanf(buf, "%d", &table[i]);
    }

    return table;
}

int compare_int(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

void merge_join(int *outer, const int size_of_outer,
		int *inner, const int size_of_inner)
{
    int outer_pos = 0, inner_pos = 0;
    int origin_inner_pos;
    int join_count = 0;

    /*
     * マージソートの事前準備として駆動表と内部表をソートする
     */
    qsort(outer, size_of_outer, sizeof(int), compare_int);
    qsort(inner, size_of_inner, sizeof(int), compare_int);

    for (;;) {
	/*
	 * 一致するまで駆動表と内部表のポジションを進める
	 * 一致せずにどちらかの配列が終了した場合はjoinを停止する
	 */
	while (outer[outer_pos] != inner[inner_pos]) {
	    if (outer[outer_pos] < inner[inner_pos])
		outer_pos++;
	    else
		inner_pos++;

	    if (outer_pos >= size_of_outer || inner_pos >= size_of_inner) {
		printf("join count = %d\n", join_count);
		return;
	    }
	}

	/*
	 * 内部表のポジションをマークする
	 */
	origin_inner_pos = inner_pos;

	/*
	 * 一致している間は内部表のポジションを進めてjoinを続ける
	 */
	while (outer[outer_pos] == inner[inner_pos] && inner_pos < size_of_inner) {
	    printf("%d %d\n", outer[outer_pos], inner[inner_pos]);
	    join_count++;
	    inner_pos++;
	}

	/*
	 * 駆動表のポジションを一つ進めて、内部表のポジションをマークした場所まで戻す
	 */
	outer_pos++;
	inner_pos = origin_inner_pos;

	/*
	 * どちらかが終了まで進んでいればjoinを停止する
	 */
	if (outer_pos >= size_of_outer || inner_pos >= size_of_inner) {
	    printf("join count = %d\n", join_count);
	    return;
	}
    }
}

int main(int argc, char *argv[])
{
    int size_outer, size_inner;
    int *outer, *inner;
    FILE *fp;

    if (argc == 2)
	fp = fopen(argv[1], "r");
    else
	fp = stdin;

    outer = scan_data(fp, &size_outer);
    inner = scan_data(fp, &size_inner);

    merge_join(outer, size_outer, inner, size_inner);

    free(outer);
    free(inner);

    return 0;
}
