// Algorithm 1: Alternating Disks (Custom Input Version)
// Author: Joshua Zamora
// Description:
//   Lets the user enter the starting sequence of Light (L) and Dark (D) disks
//   (e.g., L D L D D L ...). Then reorders so that all D are on the left and
//   all L are on the right using adjacent swaps, counting the swaps and passes.
//
// Build: g++ -std=c++17 -O2 -Wall -Wextra -o algorithm1 algorithm1.cpp
// Run:   ./algorithm1

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct DiskResult {
    vector<char> disks;
    long long swaps;
    int runs;
};

DiskResult alternating_disks_first_approach(vector<char> a) {
    long long swaps = 0;
    int runs = 0;

    auto out_of_order = [](char left, char right) {
        return left == 'L' && right == 'D'; // L before D is an inversion
    };

    bool changed = true;
    while (changed) {
        changed = false;

        // Left → Right pass
        bool ch1 = false;
        for (int i = 0; i + 1 < (int)a.size(); ++i) {
            if (out_of_order(a[i], a[i + 1])) {
                swap(a[i], a[i + 1]);
                ++swaps;
                ch1 = true;
            }
        }
        ++runs;

        // Right → Left pass
        bool ch2 = false;
        for (int i = (int)a.size() - 2; i >= 0; --i) {
            if (out_of_order(a[i], a[i + 1])) {
                swap(a[i], a[i + 1]);
                ++swaps;
                ch2 = true;
            }
        }
        ++runs;

        changed = ch1 || ch2;
    }

    return {a, swaps, runs};
}

int main() {
    int size;
    cout << "Enter total number of disks: ";
    cin >> size;

    if (size <= 0) {
        cout << "Please enter a positive integer.\n";
        return 1;
    }

    cout << "Enter the sequence of disks (L for Light, D for Dark) separated by spaces:\n";
    vector<char> disks(size);
    for (int i = 0; i < size; ++i) {
        cin >> disks[i];
        disks[i] = toupper(disks[i]); // allow lowercase input
        if (disks[i] != 'L' && disks[i] != 'D') {
            cout << "Invalid character. Use L or D only.\n";
            return 1;
        }
    }

    auto res = alternating_disks_first_approach(disks);

    cout << "\nFinal arrangement: ";
    for (char c : res.disks) cout << c << ' ';
    cout << "\nTotal swaps performed: " << res.swaps;
    cout << "\nDirectional passes (left-to-right or right-to-left): " << res.runs << "\n";

    return 0;
}
