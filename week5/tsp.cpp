#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <vector>
#include <limits.h>
#include <math.h>
#include <float.h>

#define SQR(x) ((x)*(x))
#define SUBSETS (1 << 25)

// graph nodes
struct node
{
	float x, y;
} nodes[30];

// graph in adjencency matrix form
float edges[30][30];

float state[SUBSETS][25];

inline int setsize(int bits)
{
	int c = 0;
	for (int i = 0; i < 32; i++)
		if ((bits >> i) & 0x1)
			c++;
	return c;
}

int main(int argc, char *argv[])
{
	FILE *file;
	int num_edges, num_nodes;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "couldn't open file\n");
		return 1;
	}

	fscanf(file, "%d", &num_nodes);
	num_edges = (num_nodes - 1) * num_nodes / 2;
	printf("%d total number of edges, %d total number of nodes\n", num_edges, num_nodes);

	for (int i = 0; i < num_nodes; i++) {
		fscanf(file, "%f %f", &nodes[i].x, &nodes[i].y);	
	}
	fclose(file); 

	// calculate edge length
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j <  num_nodes; j++) {
			edges[i][j] = sqrtf(SQR(nodes[i].x - nodes[j].x) + SQR(nodes[i].y - nodes[j].y));
			printf("dist %d to %d = %f\n", i, j, edges[i][j]);
		}

	// initialize state array 
	for (int s = 0; s < SUBSETS; s++)
		for (int i = 0; i < num_nodes; i++)
			state[s][i] = FLT_MAX;
	state[0x1][0] = 0;
	printf("state array initialized\n");
	
	// solve TPS for subset size 2 to 25
	int m;
	for (m = 2; m <= num_nodes; m++) {
		printf("subset size = %d\n", m);
		for (int s = 1; s < SUBSETS; s++)
			if ((s & 0x1) && m == setsize(s))   // for all sets of size m including node 0
				for (int j = 1; j < num_nodes; j++) // j must not be 0
					if ((s >> j) & 0x1) {  // j element of subset
						float dist = FLT_MAX;
						for (int k = 0; k < num_nodes; k++)
							if (k != j && ((s >> k) & 0x1)) {   // k element of subset and not j
								int ss = s & ~(1 << j);   // remove j from subset
								if (state[ss][k] != FLT_MAX && edges[k][j] != FLT_MAX) {
									float dist2 = state[ss][k] + edges[k][j];
									if (dist2 < dist)
										dist = dist2;
								}
							}
						state[s][j] = dist;
					}
	}

	printf("find shortest distance\n");
	int f = 0;
	for (int i = 0; i < num_nodes; i++)
		f |= (1 << i);
	float dist = FLT_MAX;
	for (int j = 1; j < num_nodes; j++) {
		float dist2 = state[f][j] + edges[j][0];
		if (dist2 < dist) 
			dist = dist2;
	}
	printf("distance = %f\n", dist);

	return 0;
}
