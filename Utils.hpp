#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <string>

using namespace std;

const auto start_time = chrono::high_resolution_clock::now();

void time_taken(){
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end_time - start_time;
    cerr << "\n\nTime Taken: " << (diff.count()) << " s\n";
}