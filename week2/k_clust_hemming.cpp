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

struct bitstring
{
	int bits[24];
	int ones;
	void count_ones() {
		ones = 0;
		for (int i = 0; i < 24; i++)
			if (bits[i] == 1)
				ones++;
	}
	int hemming_dist( const bitstring &rhs ) {
		int d = 0;
		for (int i = 0; i < 24; i++)
			if ( bits[i] xor rhs.bits[i] )
				d++;
		return d;
	}
};

bool operator<(const edge &lhs, const edge &rhs)
{
	return lhs.cost < rhs.cost;
}

int main(int argc, char *argv[])
{
	FILE *file;
	int n = 0, num_nodes, num_bits;
	std::vector<edge> edges;
	std::vector<bitstring> bitstrings;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "couldn't open file\n");
		return 1;
	}

	fscanf(file, "%d %d", &num_nodes, &num_bits);
	printf("number of nodes=%d and bits=%d\n", num_nodes, num_bits);
	for (int i = 0; i < num_nodes; i++) {
		bitstring s;
		for (int j = 0; j < 24; j++) 
			fscanf(file, "%d", &s.bits[j]);
		s.count_ones();
		bitstrings.push_back(s);
	}
	fclose(file);

	// calculate hemming distance if distance < 3
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++) {
			// hemming distance >= 3
			if( abs(bitstrings[i].ones - bitstrings[j].ones) < 3 ) {
				edge e;
				e.cost = bitstrings[i].hemming_dist(bitstrings[j]);
				e.node1 = i;
				e.node2 = j;
				if (e.cost < 3) {
					edges.push_back(e);
					n++;
				}
			}
		}


	printf("%d total number of edges \n", n);

	std::sort(edges.begin(), edges.end());

	// number of clusters
	int nclust = num_nodes;
	// node -> cluster ID
	int nodes[30000];
	// number of nodes in cluster
	int cluster_size[30000];
	std::vector<edge>::iterator e;

	for (int i = 0; i < num_nodes; i++) {
		cluster_size[i] = 1;
		nodes[i] = i;
	}

	for (e = edges.begin(); e != edges.end(); ++e) {
		int n1 = e->node1;
		int n2 = e->node2;

		// merge two clusters
		if (nodes[n1] != nodes[n2]) {
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
			cluster_size[small_cluster] = 0;
			nclust--;
			printf("%d %d %d merge cluster %d and %d nclust=%d\n", 
				n1, n2, e->cost, small_cluster, big_cluster, nclust);
		}
	}

	printf("number of clusters = %d\n", nclust);

	return 0;
}
