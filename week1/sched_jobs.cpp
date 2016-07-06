#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

struct job
{
	int weight;
	int length;
	int score;
	int completion;
};

bool operator<(const job &lhs, const job &rhs)
{
	if (lhs.score == rhs.score)
		return lhs.weight > rhs.weight;
	return lhs.score > rhs.score;	
}

std::vector<job> jobs;

int main(int argc, char *argv[])
{
	FILE *file;
	int n;

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "couldn't open file\n");
		return 1;
	}

	fscanf(file, "%d", &n);
	printf("%d total jobs to schedule\n", n);

	for (int i = 0; i < n; i++) {
		job j;
		fscanf(file, "%d %d", &j.weight, &j.length);	
		j.score = j.weight - j.length;	
		jobs.push_back(j);
	}
	std::sort(jobs.begin(), jobs.end());

	int c = 0;
	long long int weighted_completion = 0;
	for (int i = 0; i < n; i++) {
		c += jobs[i].length;
		jobs[i].completion = c;
		weighted_completion += jobs[i].completion * jobs[i].weight;
		//printf("score=%d weight=%d length=%d completion=%d\n", jobs[i].score, jobs[i].weight, jobs[i].length, jobs[i].completion);
	}
	printf("weighted completion time: %lld\n", weighted_completion);

	fclose(file);

	return 0;
}
