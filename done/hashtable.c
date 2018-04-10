#include "error.h"
#include "hashtable.h"
#include <string.h>
#include <stdlib.h>


Htable_t construct_Htable(size_t size){
	Htable_t table;
	table.content = calloc(size, sizeof(bucket_t));
	if (table.content == NULL) {
		debug_print("%s", "Allocation error. Content for htable could not be allocated");
		//error, content will be NULL
	}
	table.size = size;
	return table;
}

void delete_Htable_and_content(Htable_t* table){
	free(table->content);
	table->content = NULL;
	free(table);
	table = NULL;
};


error_code add_Htable_value(Htable_t table, pps_key_t key, pps_value_t value) {
	if (table.content == NULL || value == NULL) {
		return ERR_BAD_PARAMETER;
	} else {
		size_t index = hash_function(key,table.size);
		bucket_t* list = &table.content[index];
		kv_pair_t pair;
		pair.key = key;
		pair.value = value;

		for (int i = 0; i < list->size; ++i) {
			if (strcmp(list->pairs[i].key, key) == 0) {
				strcpy(list->pairs[i].value, value);
				return ERR_NONE;
			}
		}

		list->size += 1;
		list->pairs = calloc(list->size, sizeof(kv_pair_t));
		if (list->pairs == NULL) {
			debug_print("%s", "Allocation problem, could not create list in bucket");
			return ERR_NOMEM;
		}
		list->pairs[list->size] = pair;

		return ERR_NONE;
	}
}

pps_value_t get_Htable_value(Htable_t table, pps_key_t key) {
	if (table.content == NULL || key == NULL) {
		return NULL;
	}
	size_t index = hash_function(key, table.size);
	bucket_t list = table.content[index];
	for (int i = 0; i < list.size; ++i) {
		if (strcmp(list.pairs[i].key, key) == 0) {
			return list.pairs[i].value;
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