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

std::vector<item> items;

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

int calc_value(int i, int w)
{
	int n1 = 0;
	int n2 = 0;
	int v = 0;

	if (i < 0)
		return 0;

	v = lookup(i, w);
	if (v)
		return v;
	
	n1 = calc_value(i-1, w);
	if (items[i].weight <= w)
		n2 = calc_value(i-1, w - items[i].weight) + items[i].value;
	v = MAX(n1, n2);
	insert(i, w, v);

	return v;
}

int main(int argc, char *argv[])
{
	FILE *file;
	int size, n;

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

	int val = calc_value(n, size);
	printf("value=%d\n", val);

	return 0;
}
