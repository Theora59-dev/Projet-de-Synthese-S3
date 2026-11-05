#include "list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

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
  // TO DO
}

void *get_list_node_data(const struct list_node_t *node) {
  // TO DO
}

struct list_node_t *get_successor(const struct list_node_t *node) {
  // TO DO
}

struct list_node_t *get_predecessor(const struct list_node_t *node) {
  assert(node);
  return node->predecessor;
}

void set_list_node_data(struct list_node_t *node, void *newData) {
  // TO DO
}

void set_successor(struct list_node_t *node, struct list_node_t *newSuccessor) {
  // TO DO
}

void set_predecessor(struct list_node_t *node,
                     struct list_node_t *newPredecessor) {
  // TO DO
}

/********************************************************************
 * list_t (Double-Linked List)
 ********************************************************************/

struct list_t *new_list() {
  // TO DO
}

int list_is_empty(const struct list_t *L) {
  // TO DO
}

unsigned int get_list_size(const struct list_t *L) {
  // TO DO
}

struct list_node_t *get_list_head(const struct list_t *L) {
  // TO DO
}

struct list_node_t *get_list_tail(const struct list_t *L) {
  // TO DO
}

void increase_list_size(struct list_t *L) {
  // TO DO
}

void decrease_list_size(struct list_t *L) {
  // TO DO
}

void set_list_size(struct list_t *L, unsigned int newSize) {
  // TO DO
}

void set_list_head(struct list_t *L, struct list_node_t *newHead) {
  // TO DO
}

void set_list_tail(struct list_t *L, struct list_node_t *newTail) {
  // TO DO
}

void delete_list(struct list_t *L, void (*freeData)(void *)) {
  // TO DO
}

void view_list(const struct list_t *L, void (*viewData)(const void *)) {
  // TO DO
}

void list_insert_first(struct list_t *L, void *data) {
  // TO DO
}

void list_insert_last(struct list_t *L, void *data) {
  // TO DO
}

void list_insert_after(struct list_t *L, void *data, struct list_node_t *node) {
  // TO DO
}

void *list_remove_first(struct list_t *L) {
  // TO DO
}

void *list_remove_last(struct list_t *L) {
  // TO DO
}

void *list_remove_node(struct list_t *L, struct list_node_t *node) {
  // TO DO
}

void list_swap_nodes_data(struct list_node_t *node1,
                          struct list_node_t *node2) {
  // TO DO
}

int list_data_exist(struct list_t *L, void *data) {
  // TO DO
}
