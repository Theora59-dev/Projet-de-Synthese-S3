#include "algo.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aliases.h"

#include "dyntable.h"
#include "graph.h"
#include "heap.h"
#include "list.h"
#include "util.h"

#define INFINITY ULONG_MAX

static void view_dijkstra_heap_node(const void *node) {
	assert(node);
	struct heap_node_t *heap_node = (struct heap_node_t *)node;
	printf("(%lu, %s)", get_heap_node_key(heap_node),
	       get_vertex_id(get_heap_node_data(heap_node)));
}

/*
 * 1: // Initialisation
 * 2: pour chaque sommet v ∈ V , v̸ = s faire
 * 3:       dist(v) ← ∞
 *          pred(v) ← ⊘
 * 4: dist(s) ← 0
 *    pred(s) ← ⊘
 * 5: A ← V // ensemble de sommets non trait´es
 * 6:
 * 7: tant que A̸ = ∅ faire
 * 8:   Soit v ∈ A le sommet avec dist(v) = min{dist(u)|u ∈ A}
 * 9:   A ← A \ {v}
 * 10:  pour tout voisin u de v faire
 * 11:      si dist(u) > dist(v) + w(u, v) alors
 * 12:          dist(u) ← dist(v) + w(u, v)
 * 13:          pred(u) ← v
 */
void Dijkstra(graph G, const char *source_name, int heap_type) {
	vertex_t *source_node = NULL;

	for (uint i = 0; i < get_dyn_table_used(G); i++) {
		vertex_t *v = get_dyn_table_data(G, i);
		if (strcmp(get_vertex_id(v), source_name) == 0) {
			source_node = v;
			break;
		}
	}
	assert(source_node);

	heap_t *H = new_heap(heap_type);

	for (unsigned int i = 0; i < get_dyn_table_used(G); i++) {
		vertex_t *v = get_dyn_table_data(G, i);
		unsigned long d = INFINITY;

		if (v == source_node) {
			d = 0;
		}

		set_vertex_total_distance(v, d);
		set_vertex_predecessor(v, NULL);
		uint pos = list_heap_insert(H, d, v);
		set_vertex_dict_position(v, pos);
	}

	while (!H->heap_is_empty(H)) {
		heap_node_t *m = H->heap_extract_min(H);
		vertex_t *v = get_heap_node_data(m);
		unsigned long dv = get_heap_node_key(m);

		free(m);

		set_vertex_dict_position(v, UINT_MAX);

		list_t *inc = get_vertex_incidence_list(v);
		list_node_t *n = get_list_head(inc);
		while (n != NULL) {
			edge_t *e = get_list_node_data(n);

			// Le graphe est non orienté : l'arête figure dans les deux
			// listes d'incidence. On récupère donc l'autre extrémité.
			vertex_t *u = get_edge_endpoint_U(e);
			if (u == v) {
				u = get_edge_endpoint_V(e);
			}

			if (get_vertex_dict_position(u) != UINT_MAX) {
				unsigned int w = get_edge_distance(e);
				if (dv <= INFINITY - w) {
					unsigned long candidate = dv + w;
					if (get_vertex_total_distance(u) > candidate) {
						set_vertex_total_distance(u, candidate);
						set_vertex_predecessor(u, v);

						H->heap_increase_priority(
						    H, get_vertex_dict_position(u), candidate);
					}
				}
			}
			n = get_successor(n);
		}
	}

	// Supprimer le tas (devenu inutile)
	H->delete_heap(H, free);
	free(H);
}

void view_solution(graph G, const char *source_name) {
	// TO DO
}

void save_solution(const char *out_filename, graph G, const char *source_name) {
	// TO DO
}
