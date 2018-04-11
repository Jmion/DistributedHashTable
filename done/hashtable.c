#include "error.h"
#include "hashtable.h"
#include <string.h>
#include <stdlib.h>


Htable_t construct_Htable(size_t size){
	Htable_t table;
	table.content = calloc(size, sizeof(bucket_t));
	if (table.content == NULL) {
		debug_print("%s", "Allocation error. Content for htable could not be allocated");
		return table;
	}
	table.size = size;
	return table;
}

void delete_Htable_and_content(Htable_t* table){
	for (int i = 0; i < table->size; ++i) {
		free(table->content[i].pair.key);
		free(table->content[i].pair.value);
		table->content[i].pair.key = NULL;
		table->content[i].pair.value = NULL;
		free(&table->content[i]);
	}
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

		bucket_t* first = &table.content[index];
		kv_pair_t pair;

		char* key_final = calloc(strlen(key) + 1, sizeof(char));
		char* value_final = calloc(strlen(value) +1, sizeof(char));
		strncpy(key_final, key, strlen(key) + 1);
		strncpy(value_final, value, strlen(value) +1);
		pair.key = key_final;
		pair.value = value_final;


		 while(first->next != NULL && first->pair.key != NULL){
			if (strcmp(first->pair.key, key) == 0) {
				first->pair.value = pair.value;
				return ERR_NONE;
			} else {
				//printf("Go to next bucket\n");
				first = first->next;
			}
		 }
		if (first->pair.key == NULL) {
			//first one to be inserted in the list
			first->pair = pair;
			first->next = NULL;
		} else {
			bucket_t* bucket = calloc(1, sizeof(bucket_t));
			if (bucket == NULL) {
				debug_print("%s", "Could not create new bucket");
				return ERR_NOMEM;
			}
			bucket->pair = pair;
			bucket->next = NULL;
			first->next = bucket;
		}

		//printf("%zu\n",index);
		//printf("%s\n",first->next->pair.value);
		//fflush(stdout);


		return ERR_NONE;
	}
}

pps_value_t get_Htable_value(Htable_t table, pps_key_t key) {
	if (table.content == NULL || key == NULL) {
		return NULL;
	}
	size_t index = hash_function(key, table.size);
	bucket_t *first = &table.content[index];
	//printf("%zu\n",index);
	//printf("%s\n",first->pair.key);


	while(first != NULL && first->pair.key != NULL){
		printf("%s\n", first->pair.key);
		//printf("%s\n", key);
		if (strncmp(first->pair.key, key, strlen(key)) == 0) {
			//printf("%s, %s\n", "Found something", first->pair.value);
			return first->pair.value;
		} else {
			//printf("%s\n", "Go to next");
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