#include "algo.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void Dijkstra(graph G, const char *source_name, int heap_type) {
	// TO DO
}

void view_solution(graph G, const char *source_name) {
	// TO DO
}

void save_solution(const char *out_filename, graph G, const char *source_name) {
	// TO DO
}
