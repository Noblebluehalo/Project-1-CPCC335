// Algorithm 2: Matching Group Schedules (Interactive I/O version)
// Author: Joshua Zamora
// Note: Reads people/schedules/active windows/duration from stdin,
//       prints all common free slots in 24-hr "HH:MM" format.
//
// Build: g++ -std=c++14 -O2 -Wall -Wextra -o schedules main.cpp
// Run:   ./schedules  <  input.txt

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <cctype>

using namespace std;

using Interval = pair<int, int>; // [start, end) in minutes

// ===== Utility: minimal trimming (just in case user adds spaces) =====
static string trim(const string& s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    return s.substr(i, j - i);
}

// -------- Utility: time conversions --------
static int toMinutes(const string& hmRaw) {
    string hm = trim(hmRaw);
    int h = 0, m = 0;
    char colon = ':';
    stringstream ss(hm);
    ss >> h >> colon >> m;
    return h * 60 + m;
}

static string toHM(int minutes) {
    minutes = std::max(0, minutes);
    int h = minutes / 60;
    int m = minutes % 60;
    ostringstream os;
    os << setw(2) << setfill('0') << h << ":" << setw(2) << setfill('0') << m;
    return os.str();
}

// -------- Interval helpers --------
static std::vector<Interval> mergeIntervals(std::vector<Interval> v) {
    if (v.empty()) return std::vector<Interval>();
    std::sort(v.begin(), v.end());
    std::vector<Interval> out;
    out.reserve(v.size());
    for (size_t idx = 0; idx < v.size(); ++idx) {
        Interval iv = v[idx];
        if (out.empty() || iv.first > out.back().second) {
            out.push_back(iv);
        }
        else {
            out.back().second = std::max(out.back().second, iv.second);
        }
    }
    return out;
}

// Treat outside-active as busy + clip busy to active; then merge.
static std::vector<Interval> normalizeBusy(const std::vector<Interval>& busy, Interval active) {
    const int DAY = 24 * 60;
    std::vector<Interval> clipped;
    clipped.push_back(Interval(0, active.first));
    clipped.push_back(Interval(active.second, DAY));

    for (size_t i = 0; i < busy.size(); ++i) {
        int s = busy[i].first;
        int e = busy[i].second;
        s = std::max(s, active.first);
        e = std::min(e, active.second);
        if (s < e) clipped.push_back(Interval(s, e));
    }
    return mergeIntervals(clipped);
}

// Union of all busy intervals across people.
static std::vector<Interval> unionBusyAll(const std::vector<std::vector<Interval>>& allBusy) {
    std::vector<Interval> events;
    size_t total = 0;
    for (size_t i = 0; i < allBusy.size(); ++i) total += allBusy[i].size();
    events.reserve(total);
    for (size_t i = 0; i < allBusy.size(); ++i)
        events.insert(events.end(), allBusy[i].begin(), allBusy[i].end());
    return mergeIntervals(events);
}

// Complement of busy over the 24h day → global free slots.
static std::vector<Interval> invertToFree(const std::vector<Interval>& busy) {
    const int DAY = 24 * 60;
    std::vector<Interval> freeSlots;
    int prev = 0;
    for (size_t i = 0; i < busy.size(); ++i) {
        int s = busy[i].first;
        int e = busy[i].second;
        if (prev < s) freeSlots.push_back(Interval(prev, s));
        prev = std::max(prev, e);
    }
    if (prev < DAY) freeSlots.push_back(Interval(prev, DAY));
    return freeSlots;
}

// Core solver: schedules/actives in strings; returns free slots as strings.
static std::vector<pair<string, string>> findMeetingSlots(
    const std::vector<std::vector<pair<string, string>>>& schedules,
    const std::vector<pair<string, string>>& dailyActive,
    int durationMinutes
) {
    const size_t k = schedules.size();
    std::vector<std::vector<Interval>> busyLists(k);
    std::vector<Interval> actives(k);

    // Convert everything to minutes and sort per-person busy
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < schedules[i].size(); ++j) {
            busyLists[i].push_back(Interval(toMinutes(schedules[i][j].first), toMinutes(schedules[i][j].second)));
        }
        std::sort(busyLists[i].begin(), busyLists[i].end());
        actives[i] = Interval(toMinutes(dailyActive[i].first), toMinutes(dailyActive[i].second));
    }

    // Normalize each person's unavailability
    std::vector<std::vector<Interval>> normBusy(k);
    for (size_t i = 0; i < k; ++i)
        normBusy[i] = normalizeBusy(busyLists[i], actives[i]);

    // Union busy across everyone, then invert to free
    std::vector<Interval> mergedBusy = unionBusyAll(normBusy);
    std::vector<Interval> freeSlots = invertToFree(mergedBusy);

    // Shared active window across all people
    int globalStart = 0, globalEnd = 24 * 60;
    for (size_t i = 0; i < actives.size(); ++i) {
        globalStart = std::max(globalStart, actives[i].first);
        globalEnd = std::min(globalEnd, actives[i].second);
    }

    // Keep only slots within shared active window and ≥ duration
    std::vector<pair<string, string>> answer;
    for (size_t i = 0; i < freeSlots.size(); ++i) {
        int s = freeSlots[i].first;
        int e = freeSlots[i].second;
        int ss = std::max(s, globalStart);
        int ee = std::min(e, globalEnd);
        if (ee - ss >= durationMinutes)
            answer.push_back(std::make_pair(toHM(ss), toHM(ee)));
    }
    return answer;
}

// ===== Simple input helpers =====
static void readInput(
    std::vector<std::vector<pair<string, string>>>& schedules,
    std::vector<pair<string, string>>& actives,
    int& durationMinutes
) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cout << "Enter number of people: ";
    if (!(cin >> k) || k <= 0) {
        cerr << "Invalid number of people.\n";
        exit(1);
    }
    schedules.assign(k, std::vector<pair<string, string>>());
    actives.assign(k, std::make_pair("09:00","17:00"));

    for (int p = 0; p < k; ++p) {
        int m;
        cout << "Person " << (p + 1) << " - number of busy intervals: ";
        cin >> m;
        cin.ignore(); // <-- Add this line

        cout << "Enter " << m << " busy intervals as 'HH:MM HH:MM' (start end) per line:\n";
        for (int i = 0; i < m; ++i) {
            string s, e;
            cin >> s >> e;
            schedules[p].push_back(std::make_pair(s, e));
        }

        cout << "Enter daily active window 'HH:MM HH:MM' (earliest latest): ";
        string aStart, aEnd;
        cin >> aStart >> aEnd;
        actives[p] = std::make_pair(aStart, aEnd);
    }

    cout << "Enter required meeting duration in minutes: ";
    cin >> durationMinutes;
}

int main() {
    std::vector<std::vector<pair<string, string>>> schedules;
    std::vector<pair<string, string>> actives;
    int durationMinutes = 0;

    // Read input
    readInput(schedules, actives, durationMinutes);

    // Compute answer
    std::vector<pair<string, string>> ans = findMeetingSlots(schedules, actives, durationMinutes);

    // Print results
    cout << "\nAvailable common slots (>= " << durationMinutes << " min):\n";
    if (ans.empty()) {
        cout << "(none)\n";
    }
    else {
        for (size_t i = 0; i < ans.size(); ++i) {
            cout << "[" << ans[i].first << ", " << ans[i].second << "]\n";
        }
    }
    return 0;
}