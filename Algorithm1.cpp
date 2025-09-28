// -------------------- Algorithm 1: Alternating Disks ------------------
// We represent light as 'L' and dark as 'D'.
// Start configuration: L D L D ... (2n elements).
// Allowed move: swap neighboring disks when out of order relative to goal D...D L...L.
//
// This "first approach" is essentially a cocktail-shaker pass that performs adjacent
// swaps when encountering "L D" (from left->right) and "L D" (from right->left).
// For the alternating start, this completes in exactly n^2 swaps.

#include <bits/stdc++.h>
using namespace std;

struct DiskResult {
    vector<char> disks;
    long long swaps;
    int runs; // number of directional passes performed
};

DiskResult alternating_disks_first_approach(int n) {
    vector<char> a(2 * n);
    for (int i = 0; i < 2 * n; i++)
        a[i] = (i % 2 == 0 ? 'L' : 'D'); // start with L

    long long swaps = 0;
    int runs = 0;

    // Target order comparator: 'D' should come before 'L'.
    auto out_of_order = [](char left, char right) {
        return left == 'L' && right == 'D';
    };

    bool changed = true;
    while (changed) {
        changed = false;
        // left -> right
        bool ch1 = false;
        for (int i = 0; i + 1 < (int)a.size(); ++i) {
            if (out_of_order(a[i], a[i + 1])) {
                swap(a[i], a[i + 1]);
                ++swaps;
                ch1 = true;
            }
        }
        ++runs;
        // right -> left
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

// Simple demo
int main() {
    int n = 4; // change as needed
    auto res = alternating_disks_first_approach(n);
    cout << "Final arrangement: ";
    for (char c : res.disks) cout << c << ' ';
    cout << "\nSwaps: " << res.swaps
         << "\nRuns (directional passes): " << res.runs << "\n";
    return 0;
}
