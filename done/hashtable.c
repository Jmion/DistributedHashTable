#include "error.h"
#include "hashtable.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


Htable_t construct_Htable(size_t size){
	Htable_t table = calloc(1, sizeof(Htable_t));
	table->map = calloc(size, sizeof(bucket_t));
	if (table->map == NULL) {
		debug_print("%s", "Allocation error. Content for htable could not be allocated");
		return table;
	}
	table->size = size;
	return table;
}

void delete_Htable_and_content(Htable_t* table){
	 for (int i = 0; i < (*table)->size; ++i) {
	 	//first bucket is not allocated, no need to free
	 	bucket_t* bucket = &(*table)->map[i];
	 	if (bucket->pair.key != NULL) {
	 		kv_pair_free(&bucket->pair);
	 	}
	 	bucket = bucket->next;
	 	//freeing the linked list
	 	while(bucket != NULL && bucket->pair.key != NULL){
	 		bucket_t* next = bucket->next;
	 		kv_pair_free(&bucket->pair);
	 		free(bucket);
	 		bucket = bucket->next;
	 	}
	 }
	free((*table)->map);
	(*table)->map = NULL;
	*table = NULL;
};

void kv_pair_free(kv_pair_t *kv){
		free_const_ptr(kv->key);
		free_const_ptr(kv->value);
		kv->key = NULL;
		kv->value = NULL;
}


error_code add_Htable_value(Htable_t table, pps_key_t key, pps_value_t value) {
	if (table->map == NULL || value == NULL) {
		return ERR_BAD_PARAMETER;
	} else {
		size_t index = hash_function(key,table->size);

		bucket_t* first = &table->map[index];
		kv_pair_t pair;

		char* key_final = calloc(strlen(key) + 1, sizeof(char));
		char* value_final = calloc(strlen(value) +1, sizeof(char));
		strncpy(key_final, key, strlen(key) + 1);
		strncpy(value_final, value, strlen(value) +1);
		pair.key = key_final;
		pair.value = value_final;


		//checking if key already here
		while(first != NULL && first->pair.key != NULL){
			if (strcmp(first->pair.key, key) == 0) {
				debug_print("%s","VALUE MODIFIED");
				first->pair.value = pair.value;
				return ERR_NONE;
			} else {
				first = first->next;
			}
		}

		 //new key in this bucket
		first = &table->map[index];

		if (first->pair.key == NULL) {
			//first one to be inserted in the list
			first->pair = pair;
			first->next = NULL;
			debug_print("%s %s %s","FIRST KEY", pair.key, "(end of key)");
		} else {
			while(first->next != NULL) {
				first = first->next;
			}
			debug_print("%s","COLLISION");
			bucket_t* bucket = calloc(1, sizeof(bucket_t));
			if (bucket == NULL) {
				debug_print("%s", "Could not create new bucket");
				return ERR_NOMEM;
			}
			bucket->pair = pair;
			bucket->next = NULL;
			first->next = bucket;
		}
		return ERR_NONE;
	}
}

pps_value_t get_Htable_value(Htable_t table, pps_key_t key) {
	if (table->map == NULL || key == NULL) {
		return NULL;
	}
	size_t index = hash_function(key, table->size);
	bucket_t *first = &table->map[index];

	while(first != NULL && first->pair.key != NULL){
		if (strcmp(first->pair.key, key) == 0) {
			debug_print("%s","returning value");
			return first->pair.value;
		} else {
			first = first->next;
		}
	}
	return NULL;
}


/** ----------------------------------------------------------------------
 ** Hash a string for a given hashtable size.
 ** See http://en.wikipedia.org/wiki/Jenkins_hash_function
 **/
size_t hash_function(pps_key_t key, size_t size) {
    M_REQUIRE(size != 0, SIZE_MAX, "size == %d", 0);
    M_REQUIRE_NON_NULL_CUSTOM_ERR(key, SIZE_MAX);

    size_t hash = 0;
    const size_t key_len = strlen(key);
    for (size_t i = 0; i < key_len; ++i) {
        hash += (unsigned char) key[i];
        hash += (hash << 10);
        hash ^= (hash >>  6);
    }
    hash += (hash <<  3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % size;
}