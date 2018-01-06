#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

struct hst
{
	List *buckets;
	unsigned int bucket_count;
};

HashTable HashTable_init(unsigned int bucket_count)
{
	int i;
	HashTable hash = malloc(sizeof(struct hst));
	hash->bucket_count = bucket_count;

	hash->buckets = malloc(sizeof(List) * bucket_count);
	for(i=0; i<hash->bucket_count; i++)
	{
		hash->buckets[i] = List_init();
	}
	return hash;
}

void HashTable_insert(HashTable hash, void *entry, void *hash_params, unsigned int (*hash_func)(void *, void *parameters))
{
	unsigned int bucket_location;
	bucket_location = hash_func(entry, hash_params)%hash->bucket_count;
	List_insert(hash->buckets[bucket_location], entry);
}

void HashTable_print(HashTable hash, FILE *stream, void (* print_func)(void *, FILE *))
{
	int i;
	for(i=0; i<hash->bucket_count; i++)
	{
		List_print(hash->buckets[i], stream, print_func);
	}
}

void HashTable_destroy(HashTable hash, void (* delete_func)(void *))
{
	int i;
	for(i=0; i<hash->bucket_count; i++)
	{
		List_destroy(hash->buckets[i], delete_func);
	}
	free(hash->buckets);
	hash->buckets = NULL;
	free(hash);
	hash = NULL;
}

unsigned int HashTable_get_bucket_count(HashTable hash)
{
	return hash->bucket_count;
}

List * HashTable_get_buckets(HashTable hash)
{
	return hash->buckets;
}
