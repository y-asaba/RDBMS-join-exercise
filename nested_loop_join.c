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

void nested_loop(int *outer, int size_of_outer, int *inner, int size_of_inner)
{
    int i, j, join_count = 0;

    for (i = 0; i < size_of_outer; i++) {
	for (j = 0; j < size_of_inner; j++) {
	    if (outer[i] == inner[j]) {
		printf("%d %d\n", outer[i], inner[j]);
		join_count++;
	    }
	}
    }
    printf("join count = %d\n", join_count);
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

    nested_loop(outer, size_outer, inner, size_inner);

    free(outer);
    free(inner);

    return 0;
}
