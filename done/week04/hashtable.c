#include "hashtable.h"

error_code add_Htable_value(Htable_t table, pps_key_t key, pps_value_t value){
	if(table = NULL){
		return ERR_BAD_PARAMETER;
	}
	else{
		table[key] = value;
		return ERR_NONE;
	}
}

pps_value_t get_Htable_value(Htable_t table, pps_key_t key){
	int k = key;
	return table[k];
}

size_t hash_function(pps_key_t key, size_t table_size){
	
}