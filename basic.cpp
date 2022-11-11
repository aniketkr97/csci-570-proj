/********   All Required Header Files ********/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <queue>
#include <deque>
#include <bitset>
#include <iterator>
#include <list>
#include <stack>
#include <map>
#include <set>
#include <fstream>
#include <functional>
#include <numeric>
#include <utility>
#include <limits>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
using namespace std;

int main()
{
    string line , out , part_1 , part_2;
    ifstream input_file ("input_1.txt");
    int flag = 0;
    if(input_file.is_open()) {
        while(getline(input_file , line)) {
            if(line[0] == 'A' || line[0] == 'C' || line[0] == 'G' || line[0] == 'T') {
                part_1 = out;
                out = line;
            } else {
                int repetation = stoi(line);
                string first = out.substr( 0 , repetation+1);
                string after = out.substr(repetation+1 , out.length()-repetation);
                out = first + out + after;
            }
        }
        part_2 = out;
    }
    cout<<part_1<<" "<<part_2<<endl;
}
