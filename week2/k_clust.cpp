#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <vector>

struct edge
{
	int node1, node2;
	int cost;
};

bool operator<(const edge &lhs, const edge &rhs)
{
	return lhs.cost < rhs.cost;
}

int main(int argc, char *argv[])
{
	FILE *file;
	int n, num_nodes;
	std::vector<edge> edges;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "couldn't open file\n");
		return 1;
	}

	fscanf(file, "%d", &num_nodes);
	n = (num_nodes - 1) * num_nodes / 2;
	printf("%d total number of edges, %d total number of nodes\n", n, num_nodes);

	for (int i = 0; i < n; i++) {
		struct edge e;
		fscanf(file, "%d %d %d", &e.node1, &e.node2, &e.cost);	
		e.node1--;
		e.node2--;
		edges.push_back(e);
	}
	fclose(file);
	std::sort(edges.begin(), edges.end());

	// number of clusters
	int nclust = num_nodes;
	// node -> cluster ID
	int nodes[10000];
	// number of nodes in cluster
	int cluster_size[10000];
	std::vector<edge>::iterator e;

	for (int i = 0; i < num_nodes; i++) {
		cluster_size[i] = 1;
		nodes[i] = i;
	}

	for (e = edges.begin(); e != edges.end(); ++e) {
		int n1 = e->node1;
		int n2 = e->node2;
		int cost = e->cost;

		// merge two clusters
		if (nodes[n1] != nodes[n2]) {
			if (nclust == 4) {
				printf("max spacing = %d (cluster1 = %d cluster2 = %d\n", cost, nodes[n1], nodes[n2]);
				for (int i = 0; i < num_nodes; i++)
					if (cluster_size[i])
						printf("cluster %d with %d nodes\n", i, cluster_size[i]);
				break;
			}

			// choose smaller cluster
			int small_cluster, big_cluster;
			if (cluster_size[nodes[n1]] > cluster_size[nodes[n2]]) {
				small_cluster = nodes[n2];
				big_cluster = nodes[n1];
			}
			else {
				small_cluster = nodes[n1];
				big_cluster = nodes[n2];
			}
			for (int i = 0; i < num_nodes; i++)
				if (nodes[i] == small_cluster)
					nodes[i] = big_cluster;
			cluster_size[big_cluster] += cluster_size[small_cluster];
			cluster_size[small_cluster] = 0;
			nclust--;
			printf("%d %d %d merge cluster %d and %d nclust=%d\n", 
				n1, n2, e->cost, small_cluster, big_cluster, nclust);
		}
	}

	return 0;
}
