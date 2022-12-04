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

	vector<vector<int> >opt(part_1.length() + 1, vector<int>(part_2.length() + 1, 0));

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