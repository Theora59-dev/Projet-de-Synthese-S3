#include "test_utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "aliases.h"
#include "dyntable.h"
#include "util.h"

/*
 * Crée un tableau dynamique dont la taille vaut au moins `minimum_size`.
 *
 * `new_dyn_table` est censé fournir une taille initiale de 1. Le cas échéant,
 * on agrandit explicitement le tableau pour que les tests d'insertion et de
 * suppression ne dépendent pas de cette taille initiale.
 */
static dyn_table_t *new_dyn_table_with_size(unsigned int minimum_size) {
	dyn_table_t *table = new_dyn_table();
	if (table == NULL) {
		perror("new_dyn_table");
		exit(EXIT_FAILURE);
	}

	if (get_dyn_table_size(table) < minimum_size) {
		set_dyn_table_size(table, minimum_size);
	}

	return table;
}

/* Crée un tableau contenant `count` entiers initialisés à values[i]. */
static dyn_table_t *new_dyn_table_with_values(unsigned int size,
                                              const int *values,
                                              unsigned int count) {
	if (count > size) {
		fprintf(stderr, "new_dyn_table_with_values : count > size\n");
		exit(EXIT_FAILURE);
	}

	dyn_table_t *table = new_dyn_table_with_size(size);
	for (unsigned int i = 0; i < count; i++) {
		dyn_table_insert(table, test_new_int(values[i]));
	}

	return table;
}

static int freed_count = 0;

static void counting_free(void *data) {
	freed_count++;
	freeInt(data);
}

static void check_dyn_table_values(const dyn_table_t *table,
                                   const int *expected, unsigned int count) {
	TEST_ASSERT_EQ_INT(count, get_dyn_table_used(table));
	for (unsigned int i = 0; i < count; i++) {
		TEST_ASSERT_EQ_INT(expected[i],
		                   *(int *)get_dyn_table_data(table, i));
	}
}

/* -------------------------------------------------------------------------- */
/* Construction et consultation                                               */
/* -------------------------------------------------------------------------- */

static void test_new_dyn_table(void) {
	dyn_table_t *table = new_dyn_table();

	TEST_ASSERT(table != NULL);
	if (table == NULL) {
		return;
	}

	/* La documentation de new_dyn_table annonce une taille initiale de 1. */
	TEST_ASSERT_EQ_INT(1, get_dyn_table_size(table));
	TEST_ASSERT_EQ_INT(0, get_dyn_table_used(table));
	TEST_ASSERT(dyn_table_is_empty(table));
	TEST_ASSERT(!dyn_table_is_full(table));

	delete_dyn_table(table, NULL);
}

static void test_get_dyn_table_data(void) {
	int values[] = {10, 20, 30};
	dyn_table_t *table = new_dyn_table_with_values(4, values, 3);

	TEST_ASSERT_EQ_INT(10, *(int *)get_dyn_table_data(table, 0));
	TEST_ASSERT_EQ_INT(20, *(int *)get_dyn_table_data(table, 1));
	TEST_ASSERT_EQ_INT(30, *(int *)get_dyn_table_data(table, 2));

	int expected_values[] = {10, 20, 30};
	check_dyn_table_values(table, expected_values, 3);

	delete_dyn_table(table, freeInt);
}

/* -------------------------------------------------------------------------- */
/* Indicateurs                                                                */
/* -------------------------------------------------------------------------- */

static void test_dyn_table_flags(void) {
	dyn_table_t *table = new_dyn_table();

	TEST_ASSERT(table != NULL);
	if (table == NULL) {
		return;
	}

	/* Un tableau fraîchement créé ne contient aucun élément. */
	TEST_ASSERT(dyn_table_is_empty(table));
	TEST_ASSERT_EQ_INT(0, get_dyn_table_used(table));

	/* On se place sur une taille connue pour tester les bornes. */
	set_dyn_table_size(table, 4);

	/* Taille non nulle mais aucune case utilisée : le tableau est vide. */
	TEST_ASSERT(dyn_table_is_empty(table));
	TEST_ASSERT(dyn_table_is_quasi_empty(table));
	TEST_ASSERT(!dyn_table_is_full(table));

	increase_dyn_table_used(table);
	TEST_ASSERT(!dyn_table_is_empty(table));
	TEST_ASSERT(dyn_table_is_quasi_empty(table));
	TEST_ASSERT(!dyn_table_is_full(table));

	increase_dyn_table_used(table);
	TEST_ASSERT(!dyn_table_is_empty(table));
	TEST_ASSERT(!dyn_table_is_quasi_empty(table));
	TEST_ASSERT(!dyn_table_is_full(table));

	increase_dyn_table_used(table);
	increase_dyn_table_used(table);
	TEST_ASSERT(!dyn_table_is_empty(table));
	TEST_ASSERT(!dyn_table_is_quasi_empty(table));
	TEST_ASSERT(dyn_table_is_full(table));

	decrease_dyn_table_used(table);
	TEST_ASSERT(!dyn_table_is_full(table));
	TEST_ASSERT(!dyn_table_is_quasi_empty(table));

	delete_dyn_table(table, NULL);
}

/* -------------------------------------------------------------------------- */
/* Modification de la taille et du nombre de cases utilisées                  */
/* -------------------------------------------------------------------------- */

static void test_set_dyn_table_size(void) {
	int values[] = {1, 2};
	dyn_table_t *table = new_dyn_table_with_values(2, values, 2);

	TEST_ASSERT_EQ_INT(2, get_dyn_table_size(table));
	TEST_ASSERT_EQ_INT(2, get_dyn_table_used(table));

	/* Agrandissement : le contenu utilisé est conservé. */
	set_dyn_table_size(table, 8);
	TEST_ASSERT_EQ_INT(8, get_dyn_table_size(table));
	TEST_ASSERT_EQ_INT(2, get_dyn_table_used(table));
	int expected[] = {1, 2};
	check_dyn_table_values(table, expected, 2);

	/* Réduction tant que toutes les cases utilisées tiennent. */
	set_dyn_table_size(table, 2);
	TEST_ASSERT_EQ_INT(2, get_dyn_table_size(table));
	TEST_ASSERT_EQ_INT(2, get_dyn_table_used(table));
	check_dyn_table_values(table, expected, 2);

	/* Réduction au nombre exact de cases utilisées. */
	int *removed = dyn_table_remove(table);
	TEST_ASSERT_EQ_INT(2, *removed);
	TEST_ASSERT_EQ_INT(1, get_dyn_table_used(table));
	freeInt(removed);

	set_dyn_table_size(table, 1);
	TEST_ASSERT_EQ_INT(1, get_dyn_table_size(table));
	TEST_ASSERT_EQ_INT(1, get_dyn_table_used(table));
	TEST_ASSERT_EQ_INT(1, *(int *)get_dyn_table_data(table, 0));

	delete_dyn_table(table, freeInt);
}

static void test_used_primitives(void) {
	dyn_table_t *table = new_dyn_table_with_size(4);

	TEST_ASSERT_EQ_INT(0, get_dyn_table_used(table));

	increase_dyn_table_used(table);
	TEST_ASSERT_EQ_INT(1, get_dyn_table_used(table));

	increase_dyn_table_used(table);
	TEST_ASSERT_EQ_INT(2, get_dyn_table_used(table));

	decrease_dyn_table_used(table);
	TEST_ASSERT_EQ_INT(1, get_dyn_table_used(table));

	decrease_dyn_table_used(table);
	TEST_ASSERT_EQ_INT(0, get_dyn_table_used(table));

	delete_dyn_table(table, NULL);
}

static void test_set_dyn_table_data(void) {
	int values[] = {1, 2, 3};
	dyn_table_t *table = new_dyn_table_with_values(4, values, 3);

	int *old = get_dyn_table_data(table, 1);
	int *replacement = test_new_int(20);

	set_dyn_table_data(table, 1, replacement);
	TEST_ASSERT_EQ_PTR(replacement, get_dyn_table_data(table, 1));
	TEST_ASSERT_EQ_INT(20, *(int *)get_dyn_table_data(table, 1));
	freeInt(old);

	/* set_dyn_table_data accepte une case libre tant qu'elle est dans T. */
	int *last = test_new_int(40);
	set_dyn_table_data(table, 3, last);
	increase_dyn_table_used(table);

	int expected[] = {1, 20, 3, 40};
	check_dyn_table_values(table, expected, 4);
	TEST_ASSERT_EQ_PTR(last, get_dyn_table_data(table, 3));

	delete_dyn_table(table, freeInt);
}

/* -------------------------------------------------------------------------- */
/* Insertion et suppression                                                   */
/* -------------------------------------------------------------------------- */

static void test_dyn_table_insert(void) {
	dyn_table_t *table = new_dyn_table_with_size(2);

	int *first = test_new_int(10);
	int *second = test_new_int(20);
	int *third = test_new_int(30);

	dyn_table_insert(table, first);
	TEST_ASSERT_EQ_INT(1, get_dyn_table_used(table));
	TEST_ASSERT_EQ_INT(2, get_dyn_table_size(table));
	TEST_ASSERT_EQ_PTR(first, get_dyn_table_data(table, 0));

	dyn_table_insert(table, second);
	TEST_ASSERT_EQ_INT(2, get_dyn_table_used(table));
	TEST_ASSERT_EQ_INT(2, get_dyn_table_size(table));
	TEST_ASSERT(dyn_table_is_full(table));
	TEST_ASSERT_EQ_PTR(second, get_dyn_table_data(table, 1));

	/* Le tableau est plein : l'insertion doit d'abord doubler sa taille. */
	dyn_table_insert(table, third);
	TEST_ASSERT_EQ_INT(3, get_dyn_table_used(table));
	TEST_ASSERT_EQ_INT(4, get_dyn_table_size(table));
	TEST_ASSERT(!dyn_table_is_full(table));

	int expected[] = {10, 20, 30};
	check_dyn_table_values(table, expected, 3);

	delete_dyn_table(table, freeInt);
}

static void test_dyn_table_remove(void) {
	int values[] = {10, 20, 30};
	dyn_table_t *table = new_dyn_table_with_values(4, values, 3);

	int *removed = dyn_table_remove(table);
	TEST_ASSERT_EQ_INT(30, *removed);
	TEST_ASSERT_EQ_INT(2, get_dyn_table_used(table));
	TEST_ASSERT_EQ_INT(4, get_dyn_table_size(table));
	freeInt(removed);

	int after_first[] = {10, 20};
	check_dyn_table_values(table, after_first, 2);

	/* used = 1 <= size / 4 : la taille est divisée par 2. */
	removed = dyn_table_remove(table);
	TEST_ASSERT_EQ_INT(20, *removed);
	TEST_ASSERT_EQ_INT(1, get_dyn_table_used(table));
	TEST_ASSERT_EQ_INT(2, get_dyn_table_size(table));
	freeInt(removed);

	int after_second[] = {10};
	check_dyn_table_values(table, after_second, 1);

	/* used = 0 <= size / 4 : nouvelle division par 2. */
	removed = dyn_table_remove(table);
	TEST_ASSERT_EQ_INT(10, *removed);
	TEST_ASSERT_EQ_INT(0, get_dyn_table_used(table));
	TEST_ASSERT_EQ_INT(1, get_dyn_table_size(table));
	freeInt(removed);

	delete_dyn_table(table, NULL);
}

/* -------------------------------------------------------------------------- */
/* Permutation                                                                */
/* -------------------------------------------------------------------------- */

static void test_dyn_table_swap_nodes_data(void) {
	int values[] = {1, 2, 3};
	dyn_table_t *table = new_dyn_table_with_values(4, values, 3);

	dyn_table_swap_nodes_data(table, 0, 2);
	int swapped[] = {3, 2, 1};
	check_dyn_table_values(table, swapped, 3);

	/* Permutation d'une position avec elle-même. */
	dyn_table_swap_nodes_data(table, 1, 1);
	check_dyn_table_values(table, swapped, 3);

	dyn_table_swap_nodes_data(table, 0, 1);
	int swapped_again[] = {2, 3, 1};
	check_dyn_table_values(table, swapped_again, 3);

	delete_dyn_table(table, freeInt);
}

/* -------------------------------------------------------------------------- */
/* Suppression et affichage                                                   */
/* -------------------------------------------------------------------------- */

static void test_delete_dyn_table(void) {
	int values[] = {1, 2, 3};
	dyn_table_t *table = new_dyn_table_with_values(4, values, 3);

	freed_count = 0;
	delete_dyn_table(table, counting_free);
	TEST_ASSERT_EQ_INT(3, freed_count);

	/* Avec freeData == NULL, les données ne sont pas libérées. */
	int *kept1 = test_new_int(7);
	int *kept2 = test_new_int(8);
	table = new_dyn_table_with_size(2);
	dyn_table_insert(table, kept1);
	dyn_table_insert(table, kept2);

	freed_count = 0;
	delete_dyn_table(table, NULL);
	TEST_ASSERT_EQ_INT(0, freed_count);
	TEST_ASSERT_EQ_INT(7, *kept1);
	TEST_ASSERT_EQ_INT(8, *kept2);
	freeInt(kept1);
	freeInt(kept2);

	/* Tableau vide. */
	table = new_dyn_table();
	freed_count = 0;
	delete_dyn_table(table, counting_free);
	TEST_ASSERT_EQ_INT(0, freed_count);
}

static void test_view_dyn_table(void) {
	int values[] = {1, 2, 3};
	dyn_table_t *table = new_dyn_table_with_values(4, values, 3);

	test_capture_t capture = test_capture_begin();
	view_dyn_table(table, viewInt);
	char *output = test_capture_end(&capture);

	TEST_ASSERT_STR_EQ("---- Début DynTable ----\n"
	                   "Élément à l'indice 0:1\n"
	                   "Élément à l'indice 1:2\n"
	                   "Élément à l'indice 2:3\n"
	                   "---- Fin DynTable ----\n",
	                   output);
	free(output);
	delete_dyn_table(table, freeInt);

	dyn_table_t *empty = new_dyn_table();
	capture = test_capture_begin();
	view_dyn_table(empty, viewInt);
	output = test_capture_end(&capture);

	TEST_ASSERT_STR_EQ("---- Début DynTable ----\n"
	                   "---- Fin DynTable ----\n",
	                   output);
	free(output);
	delete_dyn_table(empty, freeInt);
}

int main(void) {
	printf("=== Tests de dyntable.h ===\n");

	test_new_dyn_table();
	test_get_dyn_table_data();
	test_dyn_table_flags();
	test_set_dyn_table_size();
	test_used_primitives();
	test_set_dyn_table_data();
	test_dyn_table_insert();
	test_dyn_table_remove();
	test_dyn_table_swap_nodes_data();
	test_delete_dyn_table();
	test_view_dyn_table();

	return test_summary("dyntable");
}
