/********   All Required Header Files ********/
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <map>
#include <set>
#include <fstream>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/time.h>

extern int errno;

using namespace std;

struct result {
	string alignment_1;
	string alignment_2;
	int similarity;
};

int GAP = 30;
int MISMATCH[4][4] = {{0, 110, 48, 94 },{110, 0, 118, 48}, {48, 118, 0, 110}, {94, 48, 110, 0}};
map<char, int> INDEX;

result basic(string part_1, string part_2);
result generate_alignment(string part_1, string part_2, vector<vector<int> >& opt);
int find_best_split(string part_1, string part_2);
result efficient(string part_1, string part_2);
long getTotalMemory();

int main(int argc , char ** argv) {

	struct timeval begin , end;
	string line, out, part_1, part_2;
	ifstream input_file(argv[1]);
	int flag = 0;
	if (input_file.is_open()) {
		while (getline(input_file, line)) {
			if (line[0] == 'A' || line[0] == 'C' || line[0] == 'G' || line[0] == 'T') {
				part_1 = out;
				out = line;
			}
			else {
				int repetation = stoi(line);
				string first = out.substr(0, repetation + 1);
				string after = out.substr(repetation + 1, out.length() - repetation);
				out = first + out + after;
			}
		}
		part_2 = out;
	}
	INDEX['A'] = 0;
	INDEX['C'] = 1;
	INDEX['G'] = 2;
	INDEX['T'] = 3;

	ofstream output_file;
	output_file.open(argv[2]);
	gettimeofday(&begin, 0);
	result res = basic(part_1, part_2);
	gettimeofday(&end, 0);
	long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double totaltime = seconds*1000 + microseconds*1e-3;
	output_file << res.similarity << endl;
	output_file << res.alignment_1 << endl;
	output_file << res.alignment_2 << endl;
	double totalmemory = getTotalMemory();
	output_file << totaltime << endl;
	output_file << totalmemory << endl;
	output_file.close();
}

result basic(string part_1, string part_2) {

	vector<vector<int> > opt(part_1.length() + 1, vector<int>(part_2.length() + 1, 0));
	// initialization
	for (int i = 1; i < part_1.length() + 1; i++) {
		opt[i][0] = GAP * i;
	}
	for (int j = 1; j < part_2.length() + 1; j++) {
		opt[0][j] = GAP * j;
	}

	// calculate opt
	for (int i = 1; i < part_1.length() + 1; i++) {
		for (int j = 1; j < part_2.length() + 1; j++) {
			opt[i][j] = min(MISMATCH[INDEX[part_1[i - 1]]][INDEX[part_2[j - 1]]] + opt[i - 1][j - 1], min(GAP + opt[i - 1][j], GAP + opt[i][j - 1]));
		}
	}

	result res = generate_alignment(part_1, part_2, opt);
	res.similarity = opt[part_1.length()][part_2.length()];

	return res;
}

result generate_alignment(string part_1, string part_2, vector<vector<int> >& opt) {
	string alignment_1 = "";
	string alignment_2 = "";
	int i = part_1.length();
	int j = part_2.length();

	while (i > 0 && j > 0) {
		if (MISMATCH[INDEX[part_1[i - 1]]][INDEX[part_2[j - 1]]] + opt[i - 1][j - 1] <= GAP + opt[i - 1][j] &&
			MISMATCH[INDEX[part_1[i - 1]]][INDEX[part_2[j - 1]]] + opt[i - 1][j - 1] <= GAP + opt[i][j - 1]) {
			alignment_1 += part_1[i - 1];
			alignment_2 += part_2[j - 1];
			j--;
			i--;
		}
		else if (GAP + opt[i - 1][j] <= MISMATCH[INDEX[part_1[i - 1]]][INDEX[part_2[j - 1]]] + opt[i - 1][j - 1] &&
			GAP + opt[i - 1][j] <= GAP + opt[i][j - 1]) {
			alignment_1 += part_1[i - 1];
			alignment_2 += "_";
			i--;
		}
		else if (GAP + opt[i][j - 1] <= MISMATCH[INDEX[part_1[i - 1]]][INDEX[part_2[j - 1]]] + opt[i - 1][j - 1] &&
			GAP + opt[i][j - 1] <= GAP + opt[i - 1][j]) {
			alignment_1 += "_";
			alignment_2 += part_2[j - 1];
			j--;
		}

	}

	while (i > 0) {
		alignment_1 += part_1[i - 1];
		alignment_2 += "_";
		i--;
	}

	while (j > 0) {
		alignment_1 += "_";
		alignment_2 += part_2[j - 1];
		j--;
	}

	// reverse
	string alignment_1_r(alignment_1.rbegin(), alignment_1.rend());
	string alignment_2_r(alignment_2.rbegin(), alignment_2.rend());

	result res;
	res.alignment_1 = alignment_1_r;
	res.alignment_2 = alignment_2_r;
	res.similarity = -1;
	return res;
}
int find_best_split(string part_1, string part_2) {
	// here we calculate opt_1 for Xleft and Y first, and then opt_2 for Xright and Y.

	// maintain only two rows to reduce space complexity
	vector<vector<int> > opt_1 (2, vector<int>(part_2.length() + 1, 0));

	// initialization for opt_1
	opt_1[1][0] = GAP;
	for (int j = 1; j < part_2.length() + 1; j++)
		opt_1[0][j] = GAP * j;

	for (int i = 1; i < part_1.length() / 2 + 1; i++) {
		// calculate opt_1
		for (int j = 1; j < part_2.length() + 1; j++) {
			opt_1[1][j] = min(MISMATCH[INDEX[part_1[i - 1]]][INDEX[part_2[j - 1]]] + opt_1[0][j - 1], min(GAP + opt_1[0][j], GAP + opt_1[1][j - 1]));
		}

		// move the row
		for (int j = 0; j < part_2.length() + 1; j++)
			opt_1[0][j] = opt_1[1][j];

		// initialization
		opt_1[1][0] = GAP * (i + 1); 
	}

	// reverse the two strings to calculate opt_2
	string part_1_r(part_1.rbegin(), part_1.rend());
	string part_2_r(part_2.rbegin(), part_2.rend());

	vector<vector<int> > opt_2(2, vector<int>(part_2_r.length() + 1, 0));

	// initialization for opt_2
	opt_2[1][0] = GAP;
	for (int j = 1; j < part_2_r.length() + 1; j++)
		opt_2[0][j] = GAP * j;

	for (int i = 1; i < part_1_r.length() - part_1_r.length() / 2 + 1; i++) {
		// calculate opt_2
		for (int j = 1; j < part_2_r.length() + 1; j++) {
			opt_2[1][j] = min(MISMATCH[INDEX[part_1_r[i - 1]]][INDEX[part_2_r[j - 1]]] + opt_2[0][j - 1], min(GAP + opt_2[0][j], GAP + opt_2[1][j - 1]));
		}

		// move the row
		for (int j = 0; j < part_2_r.length() + 1; j++)
			opt_2[0][j] = opt_2[1][j];

		// initialization
		opt_2[1][0] = GAP * (i + 1);
	}

	// find the best split index with opt_1 and opt_2
	int opt_min = INT_MAX;
	int index = -1;

	for (int i = 0; i < part_2.length() + 1; i++) {
		if (opt_1[0][i] + opt_2[0][part_2.length() - i] < opt_min) {
			opt_min = opt_1[0][i] + opt_2[0][part_2.length() - i];
			index = i;
		}
	}
	return index;
}

result efficient(string part_1, string part_2) {
	// if one of the two strings is shorter than 2, just reuse the basic method since it will not use more than 2 rows, which is still efficient
	if (part_1.length() <= 2 || part_2.length() <= 2)
		return basic(part_1, part_2);

	// divide
	int split_1 = part_1.length() / 2;
	int split_2 = find_best_split(part_1, part_2);

	// solve subproblems
	result result_1 = efficient(part_1.substr(0, split_1), part_2.substr(0, split_2));
	result result_2 = efficient(part_1.substr(split_1), part_2.substr(split_2));

	// merge
	result res;
	res.alignment_1 = result_1.alignment_1 + result_2.alignment_1;
	res.alignment_2 = result_1.alignment_2 + result_2.alignment_2;
	res.similarity = result_1.similarity + result_2.similarity;

	return res;
}
long getTotalMemory() {
   struct rusage usage;
   int returnCode = getrusage(RUSAGE_SELF, &usage);
   if(returnCode == 0) {
       return usage.ru_maxrss;
   } else {
       //It should never occur. Check man getrusage for more info to debug.
       // printf("error %d", errno);
		return -1; 
	}
}