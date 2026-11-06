#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aliases.h"
#include "util.h"

#define false 0
#define true 1

/********************************************************************
 * list_node_t
 ********************************************************************/

/**
 * Construit et initialise un nouveau nœud d'une liste doublement chaînée.
 * Le paramètre \p data correspond à la donnée à affecter au nouveau nœud.
 *
 * @param[in] data
 * @return le nouveau nœud créé
 */
static struct list_node_t *new_list_node(void *data) {
	assert(data);
	list_node_t *instance = calloc(1, sizeof(list_node_t));
	assert(instance != NULL);

	instance->data = data;
	instance->predecessor = NULL;
	instance->successor = NULL;

	return instance;
}

void *get_list_node_data(const struct list_node_t *node) {
	assert(node);
	return node->data;
}

struct list_node_t *get_successor(const struct list_node_t *node) {
	assert(node);
	return node->successor;
}

struct list_node_t *get_predecessor(const struct list_node_t *node) {
	assert(node);
	return node->predecessor;
}

void set_list_node_data(struct list_node_t *node, void *newData) {
	assert(node && newData);
	node->data = newData;
}

void set_successor(struct list_node_t *node, struct list_node_t *newSuccessor) {
	assert(node && newSuccessor);

	list_node_t *oldSuccessor = node->successor;

	node->successor = newSuccessor;
	newSuccessor->predecessor = node;

	if (oldSuccessor != NULL) {
		oldSuccessor->predecessor = newSuccessor;
		newSuccessor->successor = oldSuccessor;
	}
}

void set_predecessor(struct list_node_t *node,
                     struct list_node_t *newPredecessor) {
	assert(node && newPredecessor);

	list_node_t *oldPredecessor = node->predecessor;

	node->predecessor = newPredecessor;
	newPredecessor->successor = node;

	if (oldPredecessor != NULL) {
		oldPredecessor->successor = newPredecessor;
		newPredecessor->predecessor = oldPredecessor;
	}
}

void delete_list_node_t(list_node_t **ptr) {
	assert(ptr != NULL);
	assert(*ptr != NULL);

	free(*ptr);
	*ptr = NULL;
}

/********************************************************************
 * list_t (Double-Linked List)
 ********************************************************************/

struct list_t *new_list() {
	list_t *list = calloc(1, sizeof(list_t));
	assert(list != NULL);

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return list;
}

int list_is_empty(const struct list_t *L) {
	assert(L);
	return get_list_size(L) == 0;
}

unsigned int get_list_size(const struct list_t *L) {
	assert(L);
	return L->size;
}

struct list_node_t *get_list_head(const struct list_t *L) {
	assert(L);
	return L->head;
}

struct list_node_t *get_list_tail(const struct list_t *L) {
	assert(L);
	return L->tail;
}

void increase_list_size(struct list_t *L) {
	assert(L);
	L->size++;
}

void decrease_list_size(struct list_t *L) {
	assert(L);
	assert(L->size > 0);
	L->size--;
}

void set_list_size(struct list_t *L, unsigned int newSize) {
	assert(L);
	L->size = newSize;
}

void set_list_head(struct list_t *L, struct list_node_t *newHead) {
	assert(L);
	assert(newHead != NULL);

	if (get_list_size(L) == 0) {
		L->tail = newHead;
	} else if (get_list_size(L) > 0) {
		list_node_t *premier = get_list_head(L);
		set_predecessor(premier, newHead);
	}

	L->head = newHead;
	increase_list_size(L);
}

void set_list_tail(struct list_t *L, struct list_node_t *newTail) {
	assert(L);
	assert(newTail != NULL);

	if (get_list_size(L) == 0) {
		L->head = newTail;
	} else if (get_list_size(L) > 0) {
		list_node_t *dernier = get_list_tail(L);
		set_successor(dernier, newTail);
	}

	L->tail = newTail;
	increase_list_size(L);
}

void delete_list(struct list_t *L, void (*freeData)(void *)) {
	assert(L);

	list_node_t *courant = get_list_head(L);
	list_node_t *temp;

	while (courant != NULL) {
		temp = get_successor(courant);
		if (freeData != NULL) {
			freeData(get_list_node_data(courant));
		}
		delete_list_node_t(&courant);

		courant = temp;
	}

	free(L);
}

void view_list(const struct list_t *L, void (*viewData)(const void *)) {
	assert(L);

	list_node_t *courant = get_list_head(L);

	printf("----- Liste de taille %i -----\n", L->size);
	while (courant != NULL) {
		viewData(get_list_node_data(courant));
		courant = get_successor(courant);
	}
	printf("----- Fin de la liste -----\n");
}

// Insère `data` dans la liste `L`.
//
// # ATTENTION
//
// Suppose que L est vide.
void list_insert_empty(struct list_t *L, void *data) {
	assert(L && data);
	assert(get_list_head(L) == NULL && get_list_tail(L) == NULL &&
	       list_is_empty(L));

	list_node_t *node = new_list_node(data);

	L->head = node;
	L->tail = node;

	increase_list_size(L);
}

void list_insert_first(struct list_t *L, void *data) {
	assert(L && data);

	if (list_is_empty(L)) {
		list_insert_empty(L, data);
		return;
	}

	list_node_t *newHead = new_list_node(data);
	set_list_head(L, newHead);
}

void list_insert_last(struct list_t *L, void *data) {
	assert(L && data);

	if (list_is_empty(L)) {
		list_insert_empty(L, data);
		return;
	}

	list_node_t *newTail = new_list_node(data);
	set_list_tail(L, newTail);
}

void list_insert_after(struct list_t *L, void *data, struct list_node_t *node) {
	assert(L && data && node);
	assert(!list_is_empty(L));

	uint listSize = get_list_size(L);
	list_node_t *insertedNode = new_list_node(data);

	if (get_list_tail(L) == node) {
		assert((listSize > 1) ||
		       (get_list_head(L) == node && get_list_tail(L) == node));
		set_list_tail(L, insertedNode);
		return;
	}

	set_successor(node, insertedNode);
	increase_list_size(L);
}

///////////////////////////////////

void *list_remove_first(struct list_t *L) {
	assert(get_list_size(L) != 0);

	list_node_t *ancienneTete = get_list_head(L);
	void *data = ancienneTete->data;

	if (get_list_size(L) == 1) {
		L->head = NULL;
		L->tail = NULL;
	} else {
		list_node_t *nouvelleTete = get_successor(ancienneTete);
		nouvelleTete->predecessor = NULL;
		L->head = nouvelleTete;
	}

	decrease_list_size(L);

	delete_list_node_t(&ancienneTete);

	return data;
}

void *list_remove_last(struct list_t *L) {
	assert(get_list_size(L) != 0);

	list_node_t *ancienneQueue = get_list_tail(L);
	void *data = ancienneQueue->data;

	if (get_list_size(L) == 1) {
		L->head = NULL;
		L->tail = NULL;
	} else {
		list_node_t *nouvelleQueue = get_predecessor(ancienneQueue);
		nouvelleQueue->successor = NULL;
		L->tail = nouvelleQueue;
	}

	decrease_list_size(L);

	delete_list_node_t(&ancienneQueue);

	return data;
}

// Connecte les deux noeuds entre eux.
//
// ATTENTION
//
// Ne se préoccupe pas des voisins perdus.
// Suppose que first->successor et second->predecessor soient NULL
// Cause un crash du programme si cette condition n'est pas respectée
void connect_nodes(list_node_t *first, list_node_t *second) {
	assert(get_successor(first) == NULL && get_predecessor(second) == NULL);

	first->successor = second;
	second->predecessor = first;
}

// Connecte les deux noeuds entre eux, en supposant qu'ils ne soient séparés que
// d'un noeud et qu'ils le partagent mutuellement.
//
// ATTENTION
//
// Suppose que first->successor == second->predecessor
// Cause un crash du programme si cette condition n'est pas respectée
// La mémoire n'est pas supprimée. Le voisin mutuel est retourné.
list_node_t *connect_close_nodes(list_node_t *first, list_node_t *second) {
	assert(first && second);

	list_node_t *neighbor = get_successor(first);
	assert(neighbor == get_predecessor(second));

	first->successor = second;
	second->predecessor = first;

	return neighbor;
}

void *list_remove_node(struct list_t *L, struct list_node_t *node) {
	assert(L && node && get_list_size(L) > 0);

	list_node_t *predecessor = get_predecessor(node);
	list_node_t *successor = get_successor(node);

	// C'est le seul élément de la liste ou alors sa tête
	// (list_remove_first donne le même résultat sur les deux cas)
	if (get_list_size(L) == 1 || predecessor == NULL) {
		return list_remove_first(L);
	}

	// C'est la queue de la liste
	else if (successor == NULL) {
		return list_remove_last(L);
	}

	// Il est à l'intérieur de la liste
	void *data = get_list_node_data(node);
	connect_close_nodes(predecessor, successor);
	delete_list_node_t(&node);
	decrease_list_size(L);

	return data;
}

void list_swap_nodes_data(struct list_node_t *node1,
                          struct list_node_t *node2) {
	assert(node1 && node2);
	void *tmp = get_list_node_data(node1);
	set_list_node_data(node1, get_list_node_data(node2));
	set_list_node_data(node2, tmp);
}

int list_data_exist(struct list_t *L, void *data) {
	assert(L && data);
	list_node_t *courant = get_list_head(L);
	while (courant != NULL) {
		if (data == get_list_node_data(courant)) {
			return true;
		}
		courant = get_successor(courant);
	}
	return false;
}
