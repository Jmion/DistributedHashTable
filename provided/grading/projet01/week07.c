/**
 * @file test-hashtable.c
 * @brief test code for hashtables
 *
 * @author Valérian Rousset & Jean-Cédric Chappelier
 * @date 02 Oct 2017
 */



#include "hashtable.h"



int main(void){

	Htable_t table = construct_Htable(1);

	const pps_key_t key = "abc";
	const pps_value_t value = "xy";
	add_Htable_value(table, key, value);
	const pps_key_t key2 = "def";
	add_Htable_value(table, key2, value);

	pps_value_t read = get_Htable_value(table, key2);

	printf("%c\n",read[1]);

}