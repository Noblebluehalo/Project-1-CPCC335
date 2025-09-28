// -----------------------------------------------------------
// Algorithm 2: Matching Group Schedules
// Author: Joshua Zamora
//
// This program asks for each person's busy schedule and their
// daily active hours (in 24-hour / military time, e.g., 09:00-17:00).
// It then finds all time slots when *everyone* is free for at least
// a required meeting duration.
// -----------------------------------------------------------
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

// trim spaces from user input 
static string trim(const string& s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    return s.substr(i, j - i);
}

// Time conversion 
// Converts "HH:MM" (24-hour military time) to total minutes from midnight
static int toMinutes(const string& hmRaw) {
    string hm = trim(hmRaw);
    int h = 0, m = 0;
    char colon = ':';
    stringstream ss(hm);
    ss >> h >> colon >> m;
    return h * 60 + m;
}

// Converts total minutes to 24-hour format
static string toHM(int minutes) {
    minutes = std::max(0, minutes);
    int h = minutes / 60;
    int m = minutes % 60;
    ostringstream os;
    os << setw(2) << setfill('0') << h << ":" << setw(2) << setfill('0') << m;
    return os.str();
}

// Merge overlapping or touching intervals into one continuous block
static std::vector<Interval> mergeIntervals(std::vector<Interval> v) {
    if (v.empty()) return {};
    std::sort(v.begin(), v.end());
    std::vector<Interval> out;
    out.reserve(v.size());
    for (size_t idx = 0; idx < v.size(); ++idx) {
        Interval iv = v[idx];
        if (out.empty() || iv.first > out.back().second) {
            out.push_back(iv);
        } else {
            out.back().second = std::max(out.back().second, iv.second);
        }
    }
    return out;
}

// Normalize one person's schedule:
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

// Combine all busy intervals from all people into one merged list
static std::vector<Interval> unionBusyAll(const std::vector<std::vector<Interval>>& allBusy) {
    std::vector<Interval> events;
    size_t total = 0;
    for (size_t i = 0; i < allBusy.size(); ++i) total += allBusy[i].size();
    events.reserve(total);
    for (size_t i = 0; i < allBusy.size(); ++i)
        events.insert(events.end(), allBusy[i].begin(), allBusy[i].end());
    return mergeIntervals(events);
}

// Invert busy intervals over the full 24h day to get free time slots
static std::vector<Interval> invertToFree(const std::vector<Interval>& busy) {
    const int DAY = 24 * 60;
    std::vector<Interval> freeSlots;
    int prev = 0;
    for (size_t i = 0; i < busy.size(); ++i) {
        int s = busy[i].first;
        if (prev < s) freeSlots.push_back(Interval(prev, s));
        prev = std::max(prev, busy[i].second);
    }
    if (prev < DAY) freeSlots.push_back(Interval(prev, DAY));
    return freeSlots;
}

// schedules: each person's busy intervals 
// dailyActive: each person's [start,end] active hours
static std::vector<pair<string, string>> findMeetingSlots(
    const std::vector<std::vector<pair<string, string>>>& schedules,
    const std::vector<pair<string, string>>& dailyActive,
    int durationMinutes
) {
    const size_t k = schedules.size();
    std::vector<std::vector<Interval>> busyLists(k);
    std::vector<Interval> actives(k);

    // Convert all input to minutes and sort each person's busy list
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < schedules[i].size(); ++j) {
            busyLists[i].push_back(Interval(
                toMinutes(schedules[i][j].first),
                toMinutes(schedules[i][j].second)
            ));
        }
        std::sort(busyLists[i].begin(), busyLists[i].end());
        actives[i] = Interval(
            toMinutes(dailyActive[i].first),
            toMinutes(dailyActive[i].second)
        );
    }

    // Normalize each person's unavailability
    std::vector<std::vector<Interval>> normBusy(k);
    for (size_t i = 0; i < k; ++i)
        normBusy[i] = normalizeBusy(busyLists[i], actives[i]);

    // Combine everyone's busy times, then invert to find free slots
    std::vector<Interval> mergedBusy = unionBusyAll(normBusy);
    std::vector<Interval> freeSlots = invertToFree(mergedBusy);

    // Shared active window (latest start and earliest end across all people)
    int globalStart = 0, globalEnd = 24 * 60;
    for (size_t i = 0; i < actives.size(); ++i) {
        globalStart = std::max(globalStart, actives[i].first);
        globalEnd = std::min(globalEnd, actives[i].second);
    }

    // Keep only slots within shared active window and long enough
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

// Input handling 
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
    schedules.assign(k, {});
    actives.assign(k, make_pair("09:00","17:00"));

    for (int p = 0; p < k; ++p) {
        int m;
        cout << "Person " << (p + 1) << " - number of busy intervals: ";
        cin >> m;

        cout << "Enter " << m << " busy intervals as 'HH:MM HH:MM' (start end) per line:\n";
        for (int i = 0; i < m; ++i) {
            string s, e;
            cin >> s >> e;
            schedules[p].push_back(make_pair(s, e));
        }

        cout << "Enter daily active window 'HH:MM HH:MM' (earliest latest): ";
        string aStart, aEnd;
        cin >> aStart >> aEnd;
        actives[p] = make_pair(aStart, aEnd);
    }

    cout << "Enter required meeting duration in minutes: ";
    cin >> durationMinutes;
}

int main() {
    std::vector<std::vector<pair<string, string>>> schedules;
    std::vector<pair<string, string>> actives;
    int durationMinutes = 0;

    // Read schedules and active windows (in 24-hour military time)
    readInput(schedules, actives, durationMinutes);

    // Compute common free slots
    std::vector<pair<string, string>> ans = findMeetingSlots(schedules, actives, durationMinutes);

    // Print the result
    cout << "\nAvailable common slots (>= " << durationMinutes << " min):\n";
    if (ans.empty()) {
        cout << "(none)\n";
    } else {
        for (size_t i = 0; i < ans.size(); ++i) {
            cout << "[" << ans[i].first << ", " << ans[i].second << "]\n";
        }
    }
    return 0;
}

