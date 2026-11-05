#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "algo.h"

void display_help() {
	ShowMessage("", 0);
	// // À FAIRE : écrire un Makefile proposant une cible "run" qui accepte
	// les paramètres in, out, source et heap, comme indiqué ci-dessous.
	// Exemple d'appel : make run in=graphe.txt out=solution.txt source=01 heap=0
	ShowMessage("make run in=filename out=filename source=initial_node_name heap=type\n", 0);
	ShowMessage("type:", 0);
	ShowMessage("\t0 - dynamic table", 0);
	ShowMessage("\t1 - complete binary tree", 0);
	ShowMessage("\t2 - ordered list", 0);
	ShowMessage("", 1);
}

int main(int argc, char *argv[]) {
	if (argc != 5)
		display_help();

	char * input_filename = argv[1];
	char * output_filename = argv[2];
	char * source_name = argv[3];
	int heap_type = (int) strtol(argv[4], NULL, 10);
	assert(heap_type == 0 || heap_type == 1 || heap_type == 2);

	graph G = read_graph(input_filename);
	Dijkstra(G, source_name, heap_type);
	view_solution(G, source_name);
	if (output_filename != NULL)
		save_solution(output_filename, G, source_name);
	delete_graph(G);

	return EXIT_SUCCESS;
}