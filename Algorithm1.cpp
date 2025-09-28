// -----------------------------------------------------------
// Algorithm 1: Alternating Disks
// Author: Joshua Zamora
//
//Let's the user enter a starting sequence of Light (L) and Dark (D) disks
// (e.g., L D L D D L ...). The program will reorder them so that all D
// disks end up on the left and all L disks on the right using adjacent swaps.
// It also counts how many swaps and directional passes it takes.
// -----------------------------------------------------------

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Simple struct to hold the result: final disk order, swap count, and passes
struct DiskResult {
    vector<char> disks;
    long long swaps;
    int runs; 
};

// sorting function: repeatedly scans the array left-to-right
DiskResult alternating_disks_first_approach(vector<char> a) {
    long long swaps = 0;  // count how many swaps we do
    int runs = 0;         // count how many full directional passes we make

    // Helper checks for an order pair that is L followed by D
    auto out_of_order = [](char left, char right) {
        return left == 'L' && right == 'D'; 
    };

    bool changed = true;  // keep going until no swaps happen in a full cycle
    while (changed) {
        changed = false;

        // Pass from LEFT to RIGHT
        bool ch1 = false;
        for (int i = 0; i + 1 < (int)a.size(); ++i) {
            if (out_of_order(a[i], a[i + 1])) {
                swap(a[i], a[i + 1]); // swap if L is before D
                ++swaps;
                ch1 = true;
            }
        }
        ++runs; // one pass completed

        // Pass from RIGHT to LEFT
        bool ch2 = false;
        for (int i = (int)a.size() - 2; i >= 0; --i) {
            if (out_of_order(a[i], a[i + 1])) {
                swap(a[i], a[i + 1]);
                ++swaps;
                ch2 = true;
            }
        }
        ++runs; // another pass

        changed = ch1 || ch2; // if no swaps in both directions, stop
    }

    return {a, swaps, runs};
}

int main() {
    int size;
    cout << "Enter total number of disks: ";
    cin >> size;

    // basic validation for user input
    if (size <= 0) {
        cout << "Please enter a positive integer.\n";
        return 1;
    }

    cout << "Enter the sequence of disks (L for Light, D for Dark) separated by spaces:\n";
    vector<char> disks(size);

    // read the sequence from the user
    for (int i = 0; i < size; ++i) {
        cin >> disks[i];
        disks[i] = toupper(disks[i]); // accept lowercase input too
        if (disks[i] != 'L' && disks[i] != 'D') {
            cout << "Invalid character. Use L or D only.\n";
            return 1;
        }
    }

    // run the sorting algorithm
    auto res = alternating_disks_first_approach(disks);

    // print results
    cout << "\nFinal arrangement: ";
    for (char c : res.disks) cout << c << ' ';
    cout << "\nTotal swaps performed: " << res.swaps;
    cout << "\nDirectional passes (left-to-right or right-to-left): " << res.runs << "\n";

    return 0;
}
