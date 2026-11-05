#include "algo.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "util.h"
#include "dyntable.h"
#include "list.h"
#include "heap.h"
#include "graph.h"

#define INFINITY ULONG_MAX

static void view_dijkstra_heap_node(const void * node) {
	assert(node);
	struct heap_node_t * heap_node = (struct heap_node_t *) node;
	printf("(%lu, %s)", get_heap_node_key(heap_node), get_vertex_id(get_heap_node_data(heap_node)));
}

void Dijkstra(graph G, const char * source_name, int heap_type) {
	// TO DO
}

void view_solution(graph G, const char * source_name) {
	// TO DO
}

void save_solution(const char * out_filename, graph G, const char * source_name) {
	// TO DO
}