// Algorithm 1: Alternating Disks (Interactive Version)
// Author: Joshua Zamora
// Description:
//   Rearranges a row of alternating light (L) and dark (D) disks so that
//   all D disks are on the left and all L disks are on the right,
//   using only adjacent swaps. It also counts how many swaps were made.
//
// Build: g++ -std=c++17 -O2 -Wall -Wextra -o disks main.cpp
// Run:   ./disks

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Struct to store the final arrangement, total swaps, and how many passes were made.
struct DiskResult {
    vector<char> disks;   // Final order of disks
    long long swaps;      // Number of swaps performed
    int runs;             // Number of directional passes (L->R or R->L)
};

// Function implementing the "first approach" (cocktail-shaker style).
DiskResult alternating_disks_first_approach(int n) {
    // Start with alternating sequence: L D L D ... (length = 2n)
    vector<char> a(2 * n);
    for (int i = 0; i < 2 * n; i++)
        a[i] = (i % 2 == 0 ? 'L' : 'D');

    long long swaps = 0;
    int runs = 0;

    // Helper: true if a pair is out of desired order (L should not be before D)
    auto out_of_order = [](char left, char right) {
        return left == 'L' && right == 'D';
        };

    bool changed = true;
    while (changed) {
        changed = false;

        // Pass from LEFT to RIGHT: push D left if it’s behind an L
        bool ch1 = false;
        for (int i = 0; i + 1 < (int)a.size(); ++i) {
            if (out_of_order(a[i], a[i + 1])) {
                swap(a[i], a[i + 1]);
                ++swaps;
                ch1 = true;
            }
        }
        ++runs;

        // Pass from RIGHT to LEFT: do the same from the other side
        bool ch2 = false;
        for (int i = (int)a.size() - 2; i >= 0; --i) {
            if (out_of_order(a[i], a[i + 1])) {
                swap(a[i], a[i + 1]);
                ++swaps;
                ch2 = true;
            }
        }
        ++runs;

        changed = ch1 || ch2;  // Continue if any swaps happened this round
    }

    return { a, swaps, runs };
}

int main() {
    int n;
    cout << "Enter the number of dark (and light) disks (n): ";
    cin >> n;

    if (n <= 0) {
        cout << "Please enter a positive integer.\n";
        return 1;
    }

    auto res = alternating_disks_first_approach(n);

    cout << "\nFinal arrangement: ";
    for (char c : res.disks) cout << c << ' ';
    cout << "\nTotal swaps performed: " << res.swaps;
    cout << "\nDirectional passes (left-to-right or right-to-left): " << res.runs << "\n";

    return 0;
}
