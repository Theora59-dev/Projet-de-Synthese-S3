#include "heap.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dyntable.h"
#include "list.h"
#include "tree.h"
#include "util.h"

/**********************************************************************************
 * heap_node_t
 **********************************************************************************/

/**
 * Construire et initialiser un nouveau noeud d'un tas.
 *
 * @param[in] key
 * @param[in] data
 * @return le nouveau noeud créé
 */
static struct heap_node_t *new_heap_node(unsigned long key, void *data) {
	// TO DO
}

unsigned long get_heap_node_key(const struct heap_node_t *node) {
	// TO DO
}

void *get_heap_node_data(const struct heap_node_t *node) {
	// TO DO
}

unsigned int get_heap_node_dict_position(const struct heap_node_t *node) {
	// TO DO
}

void set_heap_node_key(struct heap_node_t *node, unsigned long newKey) {
	// TO DO
}

void set_heap_node_data(struct heap_node_t *node, void *newData) {
	// TO DO
}

void set_heap_node_dict_position(struct heap_node_t *node,
                                 unsigned int newPosition) {
	// TO DO
}

// void view_heap_node(const struct heap_node_t * node, void (*viewData)(const
// void *)) {} void delete_heap_node(struct heap_node_t * node, void
// (*freeData)(void *)) {}

/**********************************************************************************
 * heap_t
 **********************************************************************************/

// type =
//    0 (Dynamic Table Heap)
//    1 (Complete Binary Tree Heap)
//    2 (Ordered List Heap)
struct heap_t *new_heap(int type) {
	struct heap_t *H = calloc(1, sizeof(struct heap_t));
	if (H == NULL) {
		ShowMessage("heap:new_heap : The memory is full", 1);
		return NULL;
	}
	H->dict = new_dyn_table();
	switch (type) {
	case 0:
		H->heap = new_dyn_table();
		H->heap_insert = dyn_table_heap_insert;
		H->heap_extract_min = dyn_table_heap_extract_min;
		H->heap_increase_priority = dyn_table_heap_increase_priority;
		H->heap_is_empty = dyn_table_heap_is_empty;
		H->view_heap = view_dyn_table_heap;
		H->delete_heap = delete_dyn_table_heap;
		break;
	case 1:
		H->heap = new_tree();
		H->heap_insert = tree_heap_insert;
		H->heap_extract_min = tree_heap_extract_min;
		H->heap_increase_priority = tree_heap_increase_priority;
		H->heap_is_empty = tree_heap_is_empty;
		H->view_heap = view_tree_heap;
		H->delete_heap = delete_tree_heap;
		break;
	case 2:
		H->heap = new_list();
		H->heap_insert = list_heap_insert;
		H->heap_extract_min = list_heap_extract_min;
		H->heap_increase_priority = list_heap_increase_priority;
		H->heap_is_empty = list_heap_is_empty;
		H->view_heap = view_list_heap;
		H->delete_heap = delete_list_heap;
		break;
	default:
		ShowMessage("heap:new_heap : Unknown heap type", 1);
		return NULL;
	}
	return H;
}

void *get_heap(const struct heap_t *H) {
	assert(H);
	return H->heap;
}

struct dyn_table_t *get_heap_dictionary(const struct heap_t *H) {
	assert(H);
	return H->dict;
}

/**********************************************************************************
 * ORDERED LIST HEAP
 **********************************************************************************/

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
unsigned int list_heap_insert(struct heap_t *H, unsigned long key, void *data) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
struct heap_node_t *list_heap_extract_min(struct heap_t *H) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
void list_heap_increase_priority(struct heap_t *H, unsigned int dict_position,
                                 unsigned long newKey) {
	// TO DO
}

int list_heap_is_empty(const void *H) {
	// TO TO
}

void view_list_heap(const struct heap_t *H,
                    void (*viewHeapNode)(const void *)) {
	// TO DO
}

void delete_list_heap(struct heap_t *H, void (*freeHeapNode)(void *)) {
	// TO DO
}

/**********************************************************************************
 * DYNAMIC TABLE HEAP
 **********************************************************************************/

/**
 * Corrige la position de l'élément à la position \p position du tas \p H
 * en le comparant avec son père ((position -1) / 2) et en l'échangeant avec lui
 * si nécessaire. Le dictionnaire est mis à jour en même temps.
 *
 * Procédure récursive.
 *
 * @param[in] H
 * @param[in] position
 */
static void dyn_table_heap_update_upwards(struct heap_t *H,
                                          unsigned int position) {
	// TO TO
}

/**
 * Corrige la position de l'élément à la position \p position en le comparant
 * avec ses fils (2*position+1, 2*position+2) et en l'échangeant avec le fils de
 * la plus grande priorité si nécessaire. Le dictionnaire est mis à jour en même
 * temps.
 *
 * Procédure récursive.
 *
 * @param[in] H
 * @param[in] position
 */
static void dyn_table_heap_update_downwards(struct heap_t *H,
                                            unsigned int position) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
unsigned int dyn_table_heap_insert(struct heap_t *H, unsigned long key,
                                   void *data) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
struct heap_node_t *dyn_table_heap_extract_min(struct heap_t *H) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
void dyn_table_heap_increase_priority(struct heap_t *H,
                                      unsigned int dict_position,
                                      unsigned long newKey) {
	// TO DO
}

int dyn_table_heap_is_empty(const void *H) {
	// TO DO
}

void view_dyn_table_heap(const struct heap_t *H,
                         void (*viewHeapNode)(const void *)) {
	// TO DO
}

void delete_dyn_table_heap(struct heap_t *H, void (*freeHeapNode)(void *)) {
	// TO DO
}

/**********************************************************************************
 * COMPLETE BINARY TREE HEAP
 **********************************************************************************/

/**
 * Corrige la position du nœud à la position \p position
 * de l'arbre raciné à \p node en le comparant avec son père
 * et en l'échangeant avec lui si nécessaire.
 * Le dictionnaire est mis à jour en même temps.
 *
 * Procédure récursive. En descendant, on cherche le premier nœud
 * à corriger qui se trouve dans la position \p position (de la même façon
 * que dans insert_into_subtree). En remontant, on corrige en échangeant
 * avec le père, si besoin.
 *
 * Procédure difficile !
 *
 * @param[in] H
 * @param[in] position
 * @param[in] node
 */
static void tree_heap_update_upwards(struct heap_t *H, unsigned int position,
                                     struct tree_node_t *node) {
	// TO DO
}

/**
 * Corrige la position du nœud \p node en le comparant avec ses fils
 * et en l'échangeant avec le fils de la plus grande priorité si nécessaire.
 * Le dictionnaire est mis à jour en même temps.
 *
 * Procédure récursive.
 *
 * NB: Le sous-arbre avec racine \p node ne peut pas être vide.
 *
 * @param[in] H
 * @param[in] node
 */
static void tree_heap_update_downwards(struct heap_t *H,
                                       struct tree_node_t *node) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
unsigned int tree_heap_insert(struct heap_t *H, unsigned long key, void *data) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
struct heap_node_t *tree_heap_extract_min(struct heap_t *H) {
	// TO DO
}

// N'oubliez pas à mettre à jour le dictionnaire, si besoin !
void tree_heap_increase_priority(struct heap_t *H, unsigned int dict_position,
                                 unsigned long newKey) {
	// TO DO
}

int tree_heap_is_empty(const void *H) {
	// TO DO
}

void view_tree_heap(const struct heap_t *H,
                    void (*viewHeapNode)(const void *)) {
	// TO DO
}

void delete_tree_heap(struct heap_t *H, void (*freeHeapNode)(void *)) {
	// TO DO
}
