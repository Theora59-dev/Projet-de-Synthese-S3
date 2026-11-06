#include "test_utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "aliases.h"
#include "tree.h"
#include "util.h"

/*
 * Vérifie que l'arbre contient exactement `count` nœuds et que la donnée
 * du nœud à la position i vaut expected[i] (parcours en largeur implicite).
 */
static void check_tree_positions(tree_t *T, const int *expected,
                                 unsigned int count) {
	TEST_ASSERT_EQ_INT(count, get_tree_size(T));
	TEST_ASSERT_EQ_INT(count == 0, tree_is_empty(T));

	for (unsigned int position = 0; position < count; position++) {
		tree_node_t *node = tree_find_node(T, position);
		TEST_ASSERT(node != NULL);
		if (node == NULL) {
			return;
		}
		TEST_ASSERT_EQ_INT(expected[position],
		                   *(int *)get_tree_node_data(node));
	}

	/* La position juste après le dernier nœud n'existe pas. */
	TEST_ASSERT_EQ_PTR(NULL, tree_find_node(T, count));
}

static int freed_count = 0;

static void counting_free(void *data) {
	freed_count++;
	freeInt(data);
}

static char *capture_view_tree(tree_t *T, int order) {
	test_capture_t capture = test_capture_begin();
	view_tree(T, viewInt, order);
	return test_capture_end(&capture);
}

/* -------------------------------------------------------------------------- */
/* Nœuds                                                                      */
/* -------------------------------------------------------------------------- */

static void test_tree_node_accessors(void) {
	tree_node_t *node1 = calloc(1, sizeof(*node1));
	tree_node_t *node2 = calloc(1, sizeof(*node2));
	tree_node_t *node3 = calloc(1, sizeof(*node3));

	if (node1 == NULL || node2 == NULL || node3 == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	int *value1 = test_new_int(1);
	int *value2 = test_new_int(2);
	int *value3 = test_new_int(3);
	int *value4 = test_new_int(4);

	node1->data = value1;
	node2->data = value2;
	node3->data = value3;

	TEST_ASSERT_EQ_PTR(value1, get_tree_node_data(node1));
	TEST_ASSERT_EQ_PTR(value2, get_tree_node_data(node2));
	TEST_ASSERT_EQ_PTR(NULL, get_left(node1));
	TEST_ASSERT_EQ_PTR(NULL, get_right(node1));

	set_left(node1, node2);
	TEST_ASSERT_EQ_PTR(node2, get_left(node1));

	set_right(node1, node3);
	TEST_ASSERT_EQ_PTR(node3, get_right(node1));

	set_tree_node_data(node1, value4);
	TEST_ASSERT_EQ_PTR(value4, get_tree_node_data(node1));
	TEST_ASSERT_EQ_INT(4, *(int *)get_tree_node_data(node1));

	freeInt(value1);
	freeInt(value2);
	freeInt(value3);
	freeInt(value4);
	free(node1);
	free(node2);
	free(node3);
}

/* -------------------------------------------------------------------------- */
/* Arbre                                                                      */
/* -------------------------------------------------------------------------- */

static void test_new_tree(void) {
	tree_t *T = new_tree();

	TEST_ASSERT(T != NULL);
	TEST_ASSERT_EQ_PTR(NULL, get_tree_root(T));
	TEST_ASSERT_EQ_INT(0, get_tree_size(T));
	TEST_ASSERT(tree_is_empty(T));

	delete_tree(T, NULL);
}

static void test_tree_size_primitives(void) {
	tree_t *T = new_tree();

	increase_tree_size(T);
	TEST_ASSERT_EQ_INT(1, get_tree_size(T));
	TEST_ASSERT(!tree_is_empty(T));

	increase_tree_size(T);
	TEST_ASSERT_EQ_INT(2, get_tree_size(T));

	decrease_tree_size(T);
	TEST_ASSERT_EQ_INT(1, get_tree_size(T));

	decrease_tree_size(T);
	TEST_ASSERT_EQ_INT(0, get_tree_size(T));
	TEST_ASSERT(tree_is_empty(T));

	delete_tree(T, NULL);
}

static void test_set_tree_root(void) {
	tree_t *T = new_tree();
	tree_node_t *root = calloc(1, sizeof(*root));
	if (root == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	int *value = test_new_int(42);
	root->data = value;

	set_tree_root(T, root);
	TEST_ASSERT_EQ_PTR(root, get_tree_root(T));

	/* La racine peut être remise à NULL (arbre vidé). */
	set_tree_root(T, NULL);
	TEST_ASSERT_EQ_PTR(NULL, get_tree_root(T));

	free(root);
	freeInt(value);
	delete_tree(T, NULL);
}

static void test_tree_insert(void) {
	tree_t *T = new_tree();

	TEST_ASSERT(tree_is_empty(T));

	for (int value = 1; value <= 7; value++) {
		tree_insert(T, test_new_int(value));
	}

	int expected[] = {1, 2, 3, 4, 5, 6, 7};
	check_tree_positions(T, expected, 7);
	TEST_ASSERT(!tree_is_empty(T));

	/* Vérification explicite de la forme de l'arbre binaire complet. */
	tree_node_t *root = get_tree_root(T);
	tree_node_t *left = get_left(root);
	tree_node_t *right = get_right(root);

	TEST_ASSERT_EQ_INT(1, *(int *)get_tree_node_data(root));
	TEST_ASSERT_EQ_INT(2, *(int *)get_tree_node_data(left));
	TEST_ASSERT_EQ_INT(3, *(int *)get_tree_node_data(right));
	TEST_ASSERT_EQ_INT(4, *(int *)get_tree_node_data(get_left(left)));
	TEST_ASSERT_EQ_INT(5, *(int *)get_tree_node_data(get_right(left)));
	TEST_ASSERT_EQ_INT(6, *(int *)get_tree_node_data(get_left(right)));
	TEST_ASSERT_EQ_INT(7, *(int *)get_tree_node_data(get_right(right)));

	delete_tree(T, freeInt);
}

static void test_tree_find_node(void) {
	tree_t *T = new_tree();

	for (int value = 1; value <= 7; value++) {
		tree_insert(T, test_new_int(value));
	}

	for (unsigned int position = 0; position < 7; position++) {
		tree_node_t *node = tree_find_node(T, position);
		TEST_ASSERT(node != NULL);
		if (node != NULL) {
			TEST_ASSERT_EQ_INT((int)position + 1,
			                   *(int *)get_tree_node_data(node));
		}
	}

	/* Position invalide (au-delà du dernier nœud). */
	TEST_ASSERT_EQ_PTR(NULL, tree_find_node(T, 7));

	delete_tree(T, freeInt);

	/* Recherche dans un arbre vide. */
	tree_t *empty = new_tree();
	TEST_ASSERT_EQ_PTR(NULL, tree_find_node(empty, 0));
	delete_tree(empty, NULL);
}

static void test_tree_remove(void) {
	tree_t *T = new_tree();

	for (int value = 1; value <= 7; value++) {
		tree_insert(T, test_new_int(value));
	}

	int *data = tree_remove(T);
	TEST_ASSERT_EQ_INT(7, *data);
	freeInt(data);
	int after7[] = {1, 2, 3, 4, 5, 6};
	check_tree_positions(T, after7, 6);
	TEST_ASSERT_EQ_PTR(NULL, get_right(get_right(get_tree_root(T))));

	data = tree_remove(T);
	TEST_ASSERT_EQ_INT(6, *data);
	freeInt(data);
	int after6[] = {1, 2, 3, 4, 5};
	check_tree_positions(T, after6, 5);
	TEST_ASSERT_EQ_PTR(NULL, get_left(get_right(get_tree_root(T))));
	TEST_ASSERT_EQ_PTR(NULL, get_right(get_right(get_tree_root(T))));

	data = tree_remove(T);
	TEST_ASSERT_EQ_INT(5, *data);
	freeInt(data);
	int after5[] = {1, 2, 3, 4};
	check_tree_positions(T, after5, 4);

	data = tree_remove(T);
	TEST_ASSERT_EQ_INT(4, *data);
	freeInt(data);
	int after4[] = {1, 2, 3};
	check_tree_positions(T, after4, 3);
	TEST_ASSERT_EQ_PTR(NULL, get_left(get_left(get_tree_root(T))));

	data = tree_remove(T);
	TEST_ASSERT_EQ_INT(3, *data);
	freeInt(data);
	int after3[] = {1, 2};
	check_tree_positions(T, after3, 2);
	TEST_ASSERT_EQ_PTR(NULL, get_right(get_tree_root(T)));

	data = tree_remove(T);
	TEST_ASSERT_EQ_INT(2, *data);
	freeInt(data);
	int after2[] = {1};
	check_tree_positions(T, after2, 1);
	TEST_ASSERT_EQ_PTR(NULL, get_left(get_tree_root(T)));

	data = tree_remove(T);
	TEST_ASSERT_EQ_INT(1, *data);
	freeInt(data);
	TEST_ASSERT_EQ_INT(0, get_tree_size(T));
	TEST_ASSERT_EQ_PTR(NULL, get_tree_root(T));
	TEST_ASSERT(tree_is_empty(T));

	delete_tree(T, freeInt);
}

static void test_tree_swap_nodes_data(void) {
	tree_t *T = new_tree();

	tree_insert(T, test_new_int(1));
	tree_insert(T, test_new_int(2));
	tree_insert(T, test_new_int(3));

	tree_node_t *root = get_tree_root(T);
	tree_node_t *left = get_left(root);
	tree_node_t *right = get_right(root);

	tree_swap_nodes_data(root, left);
	TEST_ASSERT_EQ_INT(2, *(int *)get_tree_node_data(root));
	TEST_ASSERT_EQ_INT(1, *(int *)get_tree_node_data(left));
	TEST_ASSERT_EQ_INT(3, *(int *)get_tree_node_data(right));
	TEST_ASSERT_EQ_INT(3, get_tree_size(T));

	/* La structure de l'arbre n'est pas modifiée. */
	TEST_ASSERT_EQ_PTR(left, get_left(root));
	TEST_ASSERT_EQ_PTR(right, get_right(root));

	tree_swap_nodes_data(left, right);
	TEST_ASSERT_EQ_INT(3, *(int *)get_tree_node_data(left));
	TEST_ASSERT_EQ_INT(1, *(int *)get_tree_node_data(right));

	/* Permutation d'un nœud avec lui-même : aucun changement. */
	tree_swap_nodes_data(root, root);
	TEST_ASSERT_EQ_INT(2, *(int *)get_tree_node_data(root));

	int expected[] = {2, 3, 1};
	check_tree_positions(T, expected, 3);

	delete_tree(T, freeInt);
}

static void test_delete_tree(void) {
	tree_t *T = new_tree();

	tree_insert(T, test_new_int(1));
	tree_insert(T, test_new_int(2));
	tree_insert(T, test_new_int(3));

	freed_count = 0;
	delete_tree(T, counting_free);
	TEST_ASSERT_EQ_INT(3, freed_count);

	/* Avec freeData == NULL, les données ne sont pas libérées. */
	int *kept = test_new_int(7);
	T = new_tree();
	tree_insert(T, kept);

	freed_count = 0;
	delete_tree(T, NULL);
	TEST_ASSERT_EQ_INT(0, freed_count);
	TEST_ASSERT_EQ_INT(7, *kept);
	freeInt(kept);

	/* Arbre vide. */
	T = new_tree();
	freed_count = 0;
	delete_tree(T, counting_free);
	TEST_ASSERT_EQ_INT(0, freed_count);

	/* delete_tree accepte NULL. */
	delete_tree(NULL, freeInt);
}

static void test_view_tree(void) {
	tree_t *T = new_tree();

	tree_insert(T, test_new_int(1));
	tree_insert(T, test_new_int(2));
	tree_insert(T, test_new_int(3));

	char *output = capture_view_tree(T, 0);
	TEST_ASSERT_STR_EQ("NODE[\n1]\nNODE[\n2]\nNODE[\n3]\n", output);
	free(output);

	output = capture_view_tree(T, 1);
	TEST_ASSERT_STR_EQ("NODE[\n2]\nNODE[\n3]\nNODE[\n1]\n", output);
	free(output);

	output = capture_view_tree(T, 2);
	TEST_ASSERT_STR_EQ("NODE[\n2]\nNODE[\n1]\nNODE[\n3]\n", output);
	free(output);

	delete_tree(T, freeInt);

	tree_t *empty = new_tree();
	output = capture_view_tree(empty, 0);
	TEST_ASSERT_STR_EQ("", output);
	free(output);
	delete_tree(empty, freeInt);
}

int main(void) {
	printf("=== Tests de tree.h ===\n");

	test_tree_node_accessors();

	test_new_tree();
	test_tree_size_primitives();
	test_set_tree_root();
	test_tree_insert();
	test_tree_find_node();
	test_tree_remove();
	test_tree_swap_nodes_data();
	test_delete_tree();
	test_view_tree();

	return test_summary("tree");
}
