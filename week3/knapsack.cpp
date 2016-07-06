#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <map>

#define MAX(x,y) (x) < (y) ? (y) : (x)

struct item
{
	int value;
	int weight;
};

bool operator<(const item &lhs, const item &rhs)
{
	return lhs.weight > rhs.weight;
}

// map number of items to weight map
std::map <int, std::map<int, int> > m;

int lookup(int i, int w)
{
	std::map<int, std::map<int, int> >::iterator ii;
	std::map<int, int>::iterator wi;

	if (i < 0)
		return 0;
	
	ii = m.find(i);
	if (ii == m.end())
		return 0;

	wi = ii->second.find(w);
	if (wi == ii->second.end())
		return 0;
	
	return wi->second;
}

void insert(int i, int w, int v)
{
	m[i][w] = v;
}

int main(int argc, char *argv[])
{
	FILE *file;
	int size, n;
	std::vector<item> items;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "couldn't open file\n");
		return 1;
	}

	fscanf(file, "%d %d", &size, &n);
	printf("knapsack size = %d, number of items = %d\n", size, n);

	for (int i = 0; i < n; i++) {
		item it;
		fscanf(file, "%d %d", &it.value, &it.weight);	
		items.push_back(it);
	}
	fclose(file);
	std::sort(items.begin(), items.end());

		for (int w = 0; w <= size; w++) {
	for (int i = 1; i <= n; i++) {
			int n1 = lookup(i-1, w);
			int n2 = 0;

			if (items[i].weight <= w)
				n2 = lookup(i-1, w - items[i].weight) + items[i].value;
			insert(i, w, MAX(n1, n2));
		}
	}

	int val = lookup(n, size);
	printf("value=%d\n", val);

	return 0;
}
