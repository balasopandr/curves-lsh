#ifndef __HASH_TABLE__
#define __HASH_TABLE__
#include "list.h"

typedef struct hst *HashTable;

HashTable HashTable_init(unsigned int bucket_count);
void HashTable_insert(HashTable hash, void *entry, void *parameters, unsigned int (*hash_func)(void *, void *));
void HashTable_print(HashTable hash, FILE *stream, void (* print_func)(void *, FILE *));
void HashTable_destroy(HashTable hash, void (* delete_func)(void *));
unsigned int HashTable_get_bucket_count(HashTable hash);
List * HashTable_get_buckets(HashTable hash);

#endif
