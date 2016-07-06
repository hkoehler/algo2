#include <stdio.h>
#include <stdlib.h>

#include <map>

struct edge
{
	int node1, node2;
	int cost;
}edges[20000];


int main(int argc, char *argv[])
{
	FILE *file;
	int n, num_nodes;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "couldn't open file\n");
		return 1;
	}

	fscanf(file, "%d %d", &num_nodes, &n);
	printf("%d total number of edges \n", n);

	for (int i = 0; i < n; i++) {
		struct edge *e = &edges[i];
		fscanf(file, "%d %d %d", &e->node1, &e->node2, &e->cost);	
	}

	int total_cost = 0;
	int nodes[10000];

	memset(nodes, 0, sizeof(nodes));
	nodes[1] = 1;

	edge *e = NULL;
	do {
		int cost = 0x7fffffff;
		e = NULL;
		for(int i = 0; i < n; i++) {
			int n1 = edges[i].node1;
			int n2 = edges[i].node2;
			if (nodes[n1] == nodes[n2])
				continue;
			if (edges[i].cost < cost) {
				e = &edges[i];
				cost = e->cost;
			}
		}
		if (e) {
			//printf("traverse edge %d %d with cost %d\n", e->node1, e->node2, e->cost);
			total_cost += e->cost;
			nodes[e->node1] = nodes[e->node2] = 1;	 
		}
	} while (e);

	printf("cost of minimum spanning tree = %d\n", total_cost);
	fclose(file);

	return 0;
}
