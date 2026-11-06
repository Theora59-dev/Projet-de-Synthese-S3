#include "test_utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "util.h"

/* Vérifie le contenu de la liste et la cohérence des liens dans les deux sens.
 */
static void check_list_values(list_t *L, const int *expected,
                              unsigned int count) {
	TEST_ASSERT_EQ_INT(count, get_list_size(L));
	TEST_ASSERT_EQ_INT(count == 0, list_is_empty(L));

	list_node_t *node = get_list_head(L);
	list_node_t *previous = NULL;

	for (unsigned int i = 0; i < count; i++) {
		TEST_ASSERT(node != NULL);
		if (node == NULL) {
			return;
		}

		TEST_ASSERT_EQ_INT(expected[i], *(int *)get_list_node_data(node));
		TEST_ASSERT_EQ_PTR(previous, get_predecessor(node));

		previous = node;
		node = get_successor(node);
	}

	TEST_ASSERT_EQ_PTR(NULL, node);
	TEST_ASSERT_EQ_PTR(previous, get_list_tail(L));
}

/* Crée un nœud détaché (pour tester set_list_head/set_list_tail). */
static list_node_t *new_detached_node(int value) {
	list_node_t *node = calloc(1, sizeof(*node));
	if (node == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	node->data = test_new_int(value);
	return node;
}

static int freed_count = 0;

static void counting_free(void *data) {
	freed_count++;
	freeInt(data);
}

/* -------------------------------------------------------------------------- */
/* Nœuds                                                                      */
/* -------------------------------------------------------------------------- */

static void test_list_node_accessors(void) {
	list_t *L = new_list();
	int *first = test_new_int(1);
	int *second = test_new_int(2);

	list_insert_last(L, first);
	list_insert_last(L, second);

	list_node_t *head = get_list_head(L);
	list_node_t *tail = get_list_tail(L);

	TEST_ASSERT_EQ_PTR(first, get_list_node_data(head));
	TEST_ASSERT_EQ_PTR(second, get_list_node_data(tail));
	TEST_ASSERT_EQ_PTR(tail, get_successor(head));
	TEST_ASSERT_EQ_PTR(head, get_predecessor(tail));
	TEST_ASSERT_EQ_PTR(NULL, get_predecessor(head));
	TEST_ASSERT_EQ_PTR(NULL, get_successor(tail));

	int *replacement = test_new_int(3);
	set_list_node_data(head, replacement);
	TEST_ASSERT_EQ_PTR(replacement, get_list_node_data(head));
	TEST_ASSERT_EQ_INT(3, *(int *)get_list_node_data(head));

	/* `first` n'est plus référencé par la liste. */
	freeInt(first);
	delete_list(L, freeInt);
}

static void test_set_successor(void) {
	list_t *L = new_list();
	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));

	list_node_t *head = get_list_head(L);
	list_node_t *tail = get_list_tail(L);
	list_node_t *inserted = new_detached_node(3);

	set_successor(head, inserted);

	TEST_ASSERT_EQ_PTR(inserted, get_successor(head));
	TEST_ASSERT_EQ_PTR(head, get_predecessor(inserted));
	TEST_ASSERT_EQ_PTR(tail, get_successor(inserted));
	TEST_ASSERT_EQ_PTR(inserted, get_predecessor(tail));

	/* Le nœud inséré est désormais libéré par delete_list. */
	delete_list(L, freeInt);
}

static void test_set_predecessor(void) {
	list_t *L = new_list();
	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));

	list_node_t *head = get_list_head(L);
	list_node_t *tail = get_list_tail(L);
	list_node_t *inserted = new_detached_node(3);

	set_predecessor(tail, inserted);

	TEST_ASSERT_EQ_PTR(inserted, get_predecessor(tail));
	TEST_ASSERT_EQ_PTR(tail, get_successor(inserted));
	TEST_ASSERT_EQ_PTR(inserted, get_successor(head));
	TEST_ASSERT_EQ_PTR(head, get_predecessor(inserted));

	delete_list(L, freeInt);
}

/* -------------------------------------------------------------------------- */
/* Liste                                                                      */
/* -------------------------------------------------------------------------- */

static void test_new_list(void) {
	list_t *L = new_list();

	TEST_ASSERT(L != NULL);
	TEST_ASSERT_EQ_PTR(NULL, get_list_head(L));
	TEST_ASSERT_EQ_PTR(NULL, get_list_tail(L));
	TEST_ASSERT_EQ_INT(0, get_list_size(L));
	TEST_ASSERT(list_is_empty(L));

	delete_list(L, NULL);
}

static void test_list_size_primitives(void) {
	list_t *L = new_list();

	increase_list_size(L);
	TEST_ASSERT_EQ_INT(1, get_list_size(L));
	TEST_ASSERT(!list_is_empty(L));

	increase_list_size(L);
	TEST_ASSERT_EQ_INT(2, get_list_size(L));

	decrease_list_size(L);
	TEST_ASSERT_EQ_INT(1, get_list_size(L));

	decrease_list_size(L);
	TEST_ASSERT_EQ_INT(0, get_list_size(L));
	TEST_ASSERT(list_is_empty(L));

	set_list_size(L, 42);
	TEST_ASSERT_EQ_INT(42, get_list_size(L));
	TEST_ASSERT(!list_is_empty(L));

	set_list_size(L, 0);
	TEST_ASSERT(list_is_empty(L));

	delete_list(L, NULL);
}

static void test_set_list_head_and_tail(void) {
	list_t *L = new_list();
	list_node_t *a = new_detached_node(1);
	list_node_t *b = new_detached_node(2);
	list_node_t *c = new_detached_node(3);

	set_list_head(L, a);
	TEST_ASSERT_EQ_INT(1, get_list_size(L));
	TEST_ASSERT_EQ_PTR(a, get_list_head(L));
	TEST_ASSERT_EQ_PTR(a, get_list_tail(L));

	set_list_head(L, b);
	TEST_ASSERT_EQ_INT(2, get_list_size(L));
	TEST_ASSERT_EQ_PTR(b, get_list_head(L));
	TEST_ASSERT_EQ_PTR(a, get_list_tail(L));
	TEST_ASSERT_EQ_PTR(a, get_successor(b));
	TEST_ASSERT_EQ_PTR(b, get_predecessor(a));

	set_list_tail(L, c);
	TEST_ASSERT_EQ_INT(3, get_list_size(L));
	TEST_ASSERT_EQ_PTR(b, get_list_head(L));
	TEST_ASSERT_EQ_PTR(c, get_list_tail(L));
	TEST_ASSERT_EQ_PTR(a, get_successor(b));
	TEST_ASSERT_EQ_PTR(c, get_successor(a));
	TEST_ASSERT_EQ_PTR(a, get_predecessor(c));

	int expected[] = {2, 1, 3};
	check_list_values(L, expected, 3);

	delete_list(L, freeInt);
}

static void test_list_insert_first(void) {
	list_t *L = new_list();

	list_insert_first(L, test_new_int(10));
	list_insert_first(L, test_new_int(20));
	list_insert_first(L, test_new_int(30));

	int expected[] = {30, 20, 10};
	check_list_values(L, expected, 3);

	delete_list(L, freeInt);
}

static void test_list_insert_last(void) {
	list_t *L = new_list();

	list_insert_last(L, test_new_int(10));
	list_insert_last(L, test_new_int(20));
	list_insert_last(L, test_new_int(30));

	int expected[] = {10, 20, 30};
	check_list_values(L, expected, 3);

	delete_list(L, freeInt);
}

static void test_list_insert_after(void) {
	list_t *L = new_list();

	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));
	list_insert_last(L, test_new_int(3));

	list_node_t *node1 = get_list_head(L);
	list_node_t *node2 = get_successor(node1);
	list_node_t *node3 = get_successor(node2);

	/* Insertion au milieu. */
	list_insert_after(L, test_new_int(4), node2);
	int after_middle[] = {1, 2, 4, 3};
	check_list_values(L, after_middle, 4);
	TEST_ASSERT_EQ_PTR(node3, get_successor(get_successor(node2)));
	TEST_ASSERT_EQ_PTR(get_successor(node2), get_predecessor(node3));

	/* Insertion en queue. */
	list_insert_after(L, test_new_int(5), node3);
	int after_tail[] = {1, 2, 4, 3, 5};
	check_list_values(L, after_tail, 5);

	/* Insertion juste après la tête. */
	list_insert_after(L, test_new_int(0), node1);
	int after_head[] = {1, 0, 2, 4, 3, 5};
	check_list_values(L, after_head, 6);

	delete_list(L, freeInt);
}

static void test_list_remove_first(void) {
	list_t *L = new_list();

	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));
	list_insert_last(L, test_new_int(3));

	int *data = list_remove_first(L);
	TEST_ASSERT_EQ_INT(1, *data);
	freeInt(data);
	int after_first[] = {2, 3};
	check_list_values(L, after_first, 2);
	TEST_ASSERT_EQ_PTR(NULL, get_predecessor(get_list_head(L)));

	data = list_remove_first(L);
	TEST_ASSERT_EQ_INT(2, *data);
	freeInt(data);
	int after_second[] = {3};
	check_list_values(L, after_second, 1);

	data = list_remove_first(L);
	TEST_ASSERT_EQ_INT(3, *data);
	freeInt(data);
	TEST_ASSERT_EQ_INT(0, get_list_size(L));
	TEST_ASSERT_EQ_PTR(NULL, get_list_head(L));
	TEST_ASSERT_EQ_PTR(NULL, get_list_tail(L));
	TEST_ASSERT(list_is_empty(L));

	delete_list(L, freeInt);
}

static void test_list_remove_last(void) {
	list_t *L = new_list();

	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));
	list_insert_last(L, test_new_int(3));

	int *data = list_remove_last(L);
	TEST_ASSERT_EQ_INT(3, *data);
	freeInt(data);
	int after_last[] = {1, 2};
	check_list_values(L, after_last, 2);
	TEST_ASSERT_EQ_PTR(NULL, get_successor(get_list_tail(L)));

	data = list_remove_last(L);
	TEST_ASSERT_EQ_INT(2, *data);
	freeInt(data);
	int after_second[] = {1};
	check_list_values(L, after_second, 1);

	data = list_remove_last(L);
	TEST_ASSERT_EQ_INT(1, *data);
	freeInt(data);
	TEST_ASSERT_EQ_INT(0, get_list_size(L));
	TEST_ASSERT_EQ_PTR(NULL, get_list_head(L));
	TEST_ASSERT_EQ_PTR(NULL, get_list_tail(L));

	delete_list(L, freeInt);
}

static void test_list_remove_node(void) {
	list_t *L = new_list();

	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));
	list_insert_last(L, test_new_int(3));
	list_insert_last(L, test_new_int(4));

	list_node_t *node1 = get_list_head(L);
	list_node_t *node2 = get_successor(node1);
	list_node_t *node3 = get_successor(node2);
	list_node_t *node4 = get_successor(node3);

	/* Suppression d'un nœud au milieu. */
	int *data = list_remove_node(L, node2);
	TEST_ASSERT_EQ_INT(2, *data);
	freeInt(data);
	int after_middle[] = {1, 3, 4};
	check_list_values(L, after_middle, 3);
	TEST_ASSERT_EQ_PTR(node3, get_successor(node1));
	TEST_ASSERT_EQ_PTR(node1, get_predecessor(node3));

	/* Suppression de la tête. */
	data = list_remove_node(L, node1);
	TEST_ASSERT_EQ_INT(1, *data);
	freeInt(data);
	int after_head[] = {3, 4};
	check_list_values(L, after_head, 2);
	TEST_ASSERT_EQ_PTR(node3, get_list_head(L));

	/* Suppression de la queue. */
	data = list_remove_node(L, node4);
	TEST_ASSERT_EQ_INT(4, *data);
	freeInt(data);
	int after_tail[] = {3};
	check_list_values(L, after_tail, 1);
	TEST_ASSERT_EQ_PTR(node3, get_list_tail(L));

	/* Suppression du seul nœud restant. */
	data = list_remove_node(L, node3);
	TEST_ASSERT_EQ_INT(3, *data);
	freeInt(data);
	TEST_ASSERT_EQ_INT(0, get_list_size(L));
	TEST_ASSERT_EQ_PTR(NULL, get_list_head(L));
	TEST_ASSERT_EQ_PTR(NULL, get_list_tail(L));
	TEST_ASSERT(list_is_empty(L));

	delete_list(L, freeInt);
}

static void test_list_swap_nodes_data(void) {
	list_t *L = new_list();

	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));
	list_insert_last(L, test_new_int(3));

	list_node_t *head = get_list_head(L);
	list_node_t *middle = get_successor(head);
	list_node_t *tail = get_list_tail(L);

	list_swap_nodes_data(head, tail);
	int swapped[] = {3, 2, 1};
	check_list_values(L, swapped, 3);

	/* Les nœuds eux-mêmes n'ont pas bougé. */
	TEST_ASSERT_EQ_PTR(middle, get_successor(head));
	TEST_ASSERT_EQ_PTR(middle, get_predecessor(tail));

	/* Permutation d'un nœud avec lui-même : aucun changement. */
	list_swap_nodes_data(middle, middle);
	int unchanged[] = {3, 2, 1};
	check_list_values(L, unchanged, 3);

	delete_list(L, freeInt);
}

static void test_list_data_exist(void) {
	list_t *L = new_list();
	int *value10 = test_new_int(10);
	int *value20 = test_new_int(20);
	int *value30 = test_new_int(30);

	list_insert_last(L, value10);
	list_insert_last(L, value20);
	list_insert_last(L, value30);

	TEST_ASSERT(list_data_exist(L, value10));
	TEST_ASSERT(list_data_exist(L, value20));
	TEST_ASSERT(list_data_exist(L, value30));

	/* La comparaison se fait sur l'identité du pointeur, pas sur la valeur. */
	int *same_value_as_20 = test_new_int(20);
	TEST_ASSERT(!list_data_exist(L, same_value_as_20));

	list_node_t *node20 = get_successor(get_list_head(L));
	int *removed = list_remove_node(L, node20);
	TEST_ASSERT_EQ_INT(20, *removed);
	freeInt(removed);
	TEST_ASSERT(!list_data_exist(L, value20));

	freeInt(same_value_as_20);
	delete_list(L, freeInt);
}

static void test_delete_list(void) {
	list_t *L = new_list();

	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));
	list_insert_last(L, test_new_int(3));

	freed_count = 0;
	delete_list(L, counting_free);
	TEST_ASSERT_EQ_INT(3, freed_count);

	/* Avec freeData == NULL, les données ne sont pas libérées. */
	int *kept = test_new_int(7);
	L = new_list();
	list_insert_last(L, kept);

	freed_count = 0;
	delete_list(L, NULL);
	TEST_ASSERT_EQ_INT(0, freed_count);
	TEST_ASSERT_EQ_INT(7, *kept);
	freeInt(kept);

	/* Liste vide. */
	L = new_list();
	freed_count = 0;
	delete_list(L, counting_free);
	TEST_ASSERT_EQ_INT(0, freed_count);
}

static void test_view_list(void) {
	list_t *L = new_list();
	list_insert_last(L, test_new_int(1));
	list_insert_last(L, test_new_int(2));
	list_insert_last(L, test_new_int(3));

	test_capture_t capture = test_capture_begin();
	view_list(L, viewInt);
	char *output = test_capture_end(&capture);

	TEST_ASSERT_STR_EQ("----- Liste de taille 3 -----\n"
	                   "123"
	                   "----- Fin de la liste -----\n",
	                   output);
	free(output);
	delete_list(L, freeInt);

	list_t *empty = new_list();
	capture = test_capture_begin();
	view_list(empty, viewInt);
	output = test_capture_end(&capture);

	TEST_ASSERT_STR_EQ("----- Liste de taille 0 -----\n"
	                   "----- Fin de la liste -----\n",
	                   output);
	free(output);
	delete_list(empty, freeInt);
}

int main(void) {
	printf("=== Tests de list.h ===\n");

	test_list_node_accessors();
	test_set_successor();
	test_set_predecessor();

	test_new_list();
	test_list_size_primitives();
	test_set_list_head_and_tail();
	test_list_insert_first();
	test_list_insert_last();
	test_list_insert_after();
	test_list_remove_first();
	test_list_remove_last();
	test_list_remove_node();
	test_list_swap_nodes_data();
	test_list_data_exist();
	test_delete_list();
	test_view_list();

	return test_summary("list");
}
