#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <vector>
#include <limits.h>

struct edge
{
	int node1, node2;
	int cost;
};

long long state[1000][1000];

int main(int argc, char *argv[])
{
	FILE *file;
	int num_edges, num_nodes;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "couldn't open file\n");
		return 1;
	}

	fscanf(file, "%d %d", &num_nodes, &num_edges);
	printf("%d total number of edges, %d total number of nodes\n", num_edges, num_nodes);

	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j <  num_nodes; j++)
			state[i][j] = LLONG_MAX;

	for (int i = 0; i < num_edges; i++) {
		struct edge e;
		fscanf(file, "%d %d %d", &e.node1, &e.node2, &e.cost);	
		e.node1--;
		e.node2--;
		state[e.node1][e.node2] = e.cost;
		state[e.node1][e.node1] = state[e.node2][e.node2] = 0;
	}
	fclose(file); 

	for (int k = 0; k < num_nodes; k++)
		for (int i = 0; i < num_nodes; i++)
			for (int j = 0; j < num_nodes; j++) {
				if ( state[i][k] == LLONG_MAX || state[k][j] == LLONG_MAX )
					continue;
				long long c = state[i][k] + state[k][j];
				if ( c < state[i][j] )
					state[i][j] = c;
			}

	// check for negative cost cycles
	for (int i = 0; i < num_nodes; i++)
		if (state[i][i] < 0) 
			printf("node %d has negative cost cycle\n", i);

	// find the shortest shortest path
	long long p = LLONG_MAX;
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			if (state[i][j] < p)
				p = state[i][j];

	printf("shortest shortest path = %lld\n", p);

	return 0;
}
