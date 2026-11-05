#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"
#include "graph.h"
#include "algo.h"

int main(int argc, char *argv[]) {
	if (argc != 3)
		ShowMessage("expe:main: invalid number of arguments", 1);

	char * in_filename = argv[1];
	char * source_name = argv[2];

	clock_t start, end;

	graph G = read_graph(in_filename);

	start = clock();
	Dijkstra(G, source_name, 0);
	end = clock();
	double cpu_time_used0 = ((double) (end - start)) / CLOCKS_PER_SEC;

	reset_graph(G);

	start = clock();
	Dijkstra(G, source_name, 1);
	end = clock();
	double cpu_time_used1 = ((double) (end - start)) / CLOCKS_PER_SEC;

	reset_graph(G);

	start = clock();
	Dijkstra(G, source_name, 2);
	end = clock();
	double cpu_time_used2 = ((double) (end - start)) / CLOCKS_PER_SEC;

	delete_graph(G);

	printf("%lf %lf %lf", cpu_time_used0, cpu_time_used1, cpu_time_used2);

	return EXIT_SUCCESS;
}