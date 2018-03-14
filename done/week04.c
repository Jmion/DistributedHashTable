/**
 * @file test-hashtable.c
 * @brief test code for hashtables
 *
 * @author Valérian Rousset & Jean-Cédric Chappelier
 * @date 02 Oct 2017
 */


#include <check.h>

#include "tests.h"
#include "hashtable.h"

START_TEST(add_value_does_retrieve_same_value)
{

	Htable_t table;

	const pps_key_t key = 'c';
	const pps_value_t value = 42;
	add_Htable_value(table, key, value);

	const pps_value_t read = get_Htable_value(table, key);
	ck_assert_int_eq(value, read);
}
END_TEST;

START_TEST(null_table)
{

	const pps_key_t key = 'c';
	const pps_value_t value = 42;
	int error = add_Htable_value(NULL, key, value);

	ck_assert_bad_param(error);
}
END_TEST;

Suite *hashtable_suite()
{

	Suite *s = suite_create("hashtable.h");

	TCase *tc_ht = tcase_create("hashtable");
	suite_add_tcase(s, tc_ht);

	tcase_add_test(tc_ht, add_value_does_retrieve_same_value);
	tcase_add_test(tc_ht, null_table);


	return s;
}

TEST_SUITE(hashtable_suite)
