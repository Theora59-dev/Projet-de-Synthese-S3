#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aliases.h"
#include "dyntable.h"
#include "util.h"

/********************************************************************
 * dyn_table_t (Dynamic Table)
 ********************************************************************/
struct dyn_table_t *new_dyn_table() {
	dyn_table_t *dyn_table = calloc(1, sizeof(dyn_table_t));
	assert(dyn_table);
	dyn_table->T = calloc(1, sizeof(void *));
	assert(dyn_table->T);
	dyn_table->size = 0;
	dyn_table->used = 0;
	return dyn_table;
}

void *get_dyn_table_data(const struct dyn_table_t *table,
                         unsigned int position) {
	assert(table);
	assert(position < table->used);
	return table->T[position];
}

unsigned int get_dyn_table_size(const struct dyn_table_t *table) {
	assert(table);
	return table->size;
}

unsigned int get_dyn_table_used(const struct dyn_table_t *table) {
	assert(table);
	return table->used;
}

int dyn_table_is_empty(const struct dyn_table_t *table) {
	assert(table);
	return table->size == 0;
}

int dyn_table_is_full(const struct dyn_table_t *table) {
	assert(table);
	return table->used == table->size;
}

int dyn_table_is_quasi_empty(const struct dyn_table_t *table) {
	assert(table);
	return table->used <= table->size / 4;
}

void set_dyn_table_data(struct dyn_table_t *table, unsigned int position,
                        void *newData) {
	assert(table);
	assert(newData);
	assert(position < table->size);
	table->T[position] = newData;
}

void set_dyn_table_size(struct dyn_table_t *table, unsigned int newSize) {
	assert(table);
	assert(newSize > 0);
	assert(table->used <= newSize);

	void **tab = calloc(newSize, sizeof(void *));
	assert(tab);

	memcpy(tab, table->T, table->used * sizeof(void *));

	free(table->T);
	table->T = tab;
	table->size = newSize;
}

void increase_dyn_table_used(struct dyn_table_t *table) {
	assert(table);
	assert(!dyn_table_is_full(table));
	table->used++;
}

void decrease_dyn_table_used(struct dyn_table_t *table) {
	assert(table);
	assert(!dyn_table_is_empty(table));
	table->used--;
}

void view_dyn_table(const struct dyn_table_t *table,
                    void (*viewData)(const void *)) {
	assert(viewData);
	assert(table);
	printf("---- Début DynTable ----\n");
	for (uint i = 0; i < table->used; i++) {
		printf("Élément à l'indice %i:", i);
		viewData(table->T[i]);
		printf("\n");
	}
	printf("---- Fin DynTable ----\n");
}

void delete_dyn_table(struct dyn_table_t *table, void (*freeData)(void *)) {
	assert(table);
	assert(table->T);
	if (freeData != NULL) {
		for (uint i = 0; i < table->used; i++) {
			freeData(table->T[i]);
			table->T[i] = NULL;
		}
	}
	free(table->T);
	table->T = NULL;
	free(table);
	table = NULL;
}

/**
 * Dédoubler la taille du tableau dynamique \p table.
 *
 * @param table
 */
static void scale_up(struct dyn_table_t *table) {
	set_dyn_table_size(table, table->size * 2);
}

/**
 * Diviser par 2 la taille du tableau dynamique \p table.
 *
 * @param table
 */
static void scale_down(struct dyn_table_t *table) {
	if (dyn_table_is_quasi_empty(table)) {
		set_dyn_table_size(table, table->size / 2);
	}
}

void dyn_table_insert(struct dyn_table_t *table, void *data) {
	assert(table);
	assert(data);

	if (dyn_table_is_full(table)) {
		scale_up(table);
	}

	table->T[table->used] = data;
	table->used++;
}

void *dyn_table_remove(struct dyn_table_t *table) {
	assert(table);
	assert(!dyn_table_is_empty(table));

	void *data = table->T[table->used - 1];
	table->T[table->used - 1] = NULL;
	table->used--;

	if (dyn_table_is_quasi_empty(table)) {
		scale_down(table);
	}

	return data;
}

void dyn_table_swap_nodes_data(struct dyn_table_t *table, unsigned int pos1,
                               unsigned int pos2) {
	assert(table);
	void *tmp = get_dyn_table_data(table, pos1);
	set_dyn_table_data(table, pos1, get_dyn_table_data(table, pos2));
	set_dyn_table_data(table, pos2, tmp);
}
