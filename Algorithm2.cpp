// ----------------- Algorithm 2: Matching Group Schedules --------------
// Finds all common free time slots of at least a given duration
// among multiple people, based on their busy intervals and daily
// active periods. Times are in "HH:MM" 24-hour format.

#include <bits/stdc++.h>
using namespace std;

using Interval = pair<int,int>; // [start, end) in minutes

// -------- Utility: time conversions --------
static int toMinutes(const string &hm) {
    int h = 0, m = 0;
    char colon;
    stringstream ss(hm);
    ss >> h >> colon >> m;
    return h * 60 + m;
}

static string toHM(int minutes) {
    minutes = max(0, minutes);
    int h = minutes / 60;
    int m = minutes % 60;
    ostringstream os;
    os << setw(2) << setfill('0') << h << ":" << setw(2) << setfill('0') << m;
    return os.str();
}

// -------- Helper functions --------
static vector<Interval> normalizeBusy(const vector<Interval>& busy, Interval active) {
    // Add time outside the active window as busy.
    const int DAY = 24 * 60;
    vector<Interval> clipped = { {0, active.first}, {active.second, DAY} };

    for (auto [s,e] : busy) {
        s = max(s, active.first);
        e = min(e, active.second);
        if (s < e) clipped.push_back({s, e});
    }

    sort(clipped.begin(), clipped.end());
    vector<Interval> merged;
    for (auto iv: clipped) {
        if (merged.empty() || iv.first > merged.back().second)
            merged.push_back(iv);
        else
            merged.back().second = max(merged.back().second, iv.second);
    }
    return merged;
}

static vector<Interval> unionBusyAll(const vector<vector<Interval>>& allBusy) {
    vector<Interval> events;
    for (auto &v: allBusy) events.insert(events.end(), v.begin(), v.end());
    if (events.empty()) return {};
    sort(events.begin(), events.end());
    vector<Interval> merged;
    for (auto iv: events) {
        if (merged.empty() || iv.first > merged.back().second)
            merged.push_back(iv);
        else
            merged.back().second = max(merged.back().second, iv.second);
    }
    return merged;
}

static vector<Interval> invertToFree(const vector<Interval>& busy) {
    const int DAY = 24 * 60;
    vector<Interval> freeSlots;
    int prev = 0;
    for (auto [s,e]: busy) {
        if (prev < s) freeSlots.push_back({prev, s});
        prev = max(prev, e);
    }
    if (prev < DAY) freeSlots.push_back({prev, DAY});
    return freeSlots;
}

static vector<pair<string,string>> findMeetingSlots(
    const vector<vector<pair<string,string>>>& schedules,
    const vector<pair<string,string>>& dailyActive,
    int durationMinutes
) {
    size_t k = schedules.size();
    vector<vector<Interval>> busyLists(k);
    vector<Interval> actives(k);

    for (size_t i = 0; i < k; ++i) {
        for (auto [s,e] : schedules[i])
            busyLists[i].push_back({toMinutes(s), toMinutes(e)});
        sort(busyLists[i].begin(), busyLists[i].end());
        actives[i] = {toMinutes(dailyActive[i].first), toMinutes(dailyActive[i].second)};
    }

    vector<vector<Interval>> normBusy(k);
    for (size_t i = 0; i < k; ++i)
        normBusy[i] = normalizeBusy(busyLists[i], actives[i]);

    auto mergedBusy = unionBusyAll(normBusy);
    auto freeSlots = invertToFree(mergedBusy);

    // Intersection of everyone's active times
    int globalStart = 0, globalEnd = 24*60;
    for (auto a : actives) {
        globalStart = max(globalStart, a.first);
        globalEnd   = min(globalEnd, a.second);
    }

    vector<pair<string,string>> answer;
    for (auto [s,e] : freeSlots) {
        int ss = max(s, globalStart);
        int ee = min(e, globalEnd);
        if (ee - ss >= durationMinutes)
            answer.push_back({toHM(ss), toHM(ee)});
    }
    return answer;
}

// -------- Demo main --------
int main() {
    // Sample data from project handout
    vector<vector<pair<string,string>>> schedules = {
        { {"7:00","8:30"}, {"12:00","13:00"}, {"16:00","18:00"} },
        { {"9:00","10:30"}, {"12:20","14:00"}, {"14:30","15:00"}, {"16:00","17:00"} }
    };
    vector<pair<string,string>> actives = {
        {"09:00","19:00"},
        {"09:00","18:30"}
    };
    int duration = 30;

    auto ans = findMeetingSlots(schedules, actives, duration);
    cout << "Available common slots (>= " << duration << " min):\n";
    for (auto [s,e]: ans)
        cout << "[" << s << ", " << e << "]\n";
    return 0;
}
