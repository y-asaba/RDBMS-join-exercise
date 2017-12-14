#include <stdio.h>
#include <stdlib.h>

#define NUM_BUCKETS 10000
#define MIN_BUCKET_SIZE 64

typedef struct Bucket {
    int array_max_size; /* チェインの配列サイズ */
    int size;           /* 実際にチェインに入っている値の数 */
    int *array;         /* チェイン(連結リストではなく配列で実装) */
} Bucket;

static int *scan_data(FILE *fp, int *size)
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

static int  calculate_hash(int n)
{
    /*
     * 手を抜いて mod だけでキーを求める
     */
    return n % NUM_BUCKETS;
}

static Bucket **create_hash_table(const int *table, int table_size)
{
    /*
     * チェイン法でハッシュテーブルを作るが、何でも作っても問題なし
     */
    int max = 0, i;
    Bucket **hash_table = (Bucket **) calloc(NUM_BUCKETS, sizeof(Bucket *));

    for (i = 0; i < table_size; i++) {
	int hash = calculate_hash(table[i]);

	if (hash_table[hash] == NULL) {
	    hash_table[hash] = (Bucket *) malloc(sizeof(Bucket));

	    hash_table[hash]->array_max_size = MIN_BUCKET_SIZE;
	    hash_table[hash]->size	     = 0;
	    hash_table[hash]->array	     = (int *) malloc(MIN_BUCKET_SIZE * sizeof(int));
	}

	if (hash_table[hash]->size == hash_table[hash]->array_max_size) {
	    /*
	     * チェインに値が入り切らない場合はサイズを2倍に拡張する
	     */
	    hash_table[hash]->array_max_size *= 2;
	    hash_table[hash]->array = (int *) realloc(hash_table[hash]->array,
						      hash_table[hash]->array_max_size * sizeof(int));
	}

	hash_table[hash]->array[hash_table[hash]->size] = table[i];
	hash_table[hash]->size++;
    }

    return hash_table;
}

static void free_hash_table(Bucket **hash_table)
{
    int i = 0;

    for (i = 0; i < NUM_BUCKETS; i++) {
	if (hash_table[i] != NULL) {
	    free(hash_table[i]->array);
	    free(hash_table[i]);
	}
    }
    free(hash_table);
}

static void hash_join(const int *outer, const int size_of_outer,
		      const int *inner, const int size_of_inner)
{
    Bucket **hash_table;
    int i, j, join_count = 0;

    /*
     * inner tableからhash tableを生成
     */
    hash_table = create_hash_table(inner, size_of_inner);

    for (i = 0; i < size_of_outer; i++) {
    	int hash = calculate_hash(outer[i]);
    	if (hash_table[hash] != NULL) {
	    /*
	     * キーに対応するbucketは存在するので
	     * チェインを辿って実際にデータを比較する
	     */
    	    for (j = 0; j < hash_table[hash]->size; j++) {
    		if (outer[i] == hash_table[hash]->array[j]) {
    		    join_count++;
    		    printf("%d %d\n", outer[i], hash_table[hash]->array[j]);
    		}
    	    }
    	}
    }
    printf("join count = %d\n", join_count);

    free_hash_table(hash_table);
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

    hash_join(outer, size_outer, inner, size_inner);

    free(outer);
    free(inner);

    return 0;
}
