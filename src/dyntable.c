#include "dyntable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

/********************************************************************
 * dyn_table_t (Dynamic Table)
 ********************************************************************/
struct dyn_table_t *new_dyn_table() {
  // TO DO
}

void *get_dyn_table_data(const struct dyn_table_t *table,
                         unsigned int position) {
  // TO DO
}

unsigned int get_dyn_table_size(const struct dyn_table_t *table) {
  // TO DO
}

unsigned int get_dyn_table_used(const struct dyn_table_t *table) {
  // TO DO
}

int dyn_table_is_empty(const struct dyn_table_t *table) {
  // TO DO
}

int dyn_table_is_full(const struct dyn_table_t *table) {
  // TO DO
}

int dyn_table_is_quasi_empty(const struct dyn_table_t *table) {
  // TO DO
}

void set_dyn_table_data(struct dyn_table_t *table, unsigned int position,
                        void *newData) {
  // TO DO
}

void set_dyn_table_size(struct dyn_table_t *table, unsigned int newSize) {
  // TO DO
}

void increase_dyn_table_used(struct dyn_table_t *table) {
  // TO DO
}

void decrease_dyn_table_used(struct dyn_table_t *table) {
  // TO DO
}

void view_dyn_table(const struct dyn_table_t *table,
                    void (*viewData)(const void *)) {
  // TO DO
}

void delete_dyn_table(struct dyn_table_t *table, void (*freeData)(void *)) {
  // TO DO
}

/**
 * Dédoubler la taille du tableau dynamique \p table.
 *
 * @param table
 */
static void scale_up(struct dyn_table_t *table) {
  // TO DO
}

/**
 * Diviser par 2 la taille du tableau dynamique \p table.
 *
 * @param table
 */
static void scale_down(struct dyn_table_t *table) {
  // TO DO
}

void dyn_table_insert(struct dyn_table_t *table, void *data) {
  // TO DO
}

void *dyn_table_remove(struct dyn_table_t *table) {
  // TO DO
}

void dyn_table_swap_nodes_data(struct dyn_table_t *table, unsigned int pos1,
                               unsigned int pos2) {
  // TO DO
}
