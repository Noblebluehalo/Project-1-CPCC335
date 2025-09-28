// ----------------- Algorithm 2: Matching Group Schedules --------------
// Finds all common free time slots of at least a given duration
// among multiple people, based on their busy intervals and daily
// active periods. Times are in "HH:MM" 24-hour format.

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <utility>

using namespace std;

using Interval = pair<int, int>; // [start, end) in minutes

// -------- Utility: time conversions --------
static int toMinutes(const string& hm) {
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
    vector<Interval> clipped;
    clipped.push_back(make_pair(0, active.first));
    clipped.push_back(make_pair(active.second, DAY));

    for (size_t i = 0; i < busy.size(); ++i) {
        int s = max(busy[i].first, active.first);
        int e = min(busy[i].second, active.second);
        if (s < e) clipped.push_back(make_pair(s, e));
    }

    sort(clipped.begin(), clipped.end());
    vector<Interval> merged;
    for (size_t i = 0; i < clipped.size(); ++i) {
        Interval iv = clipped[i];
        if (merged.empty() || iv.first > merged.back().second)
            merged.push_back(iv);
        else
            merged.back().second = max(merged.back().second, iv.second);
    }
    return merged;
}

static vector<Interval> unionBusyAll(const vector<vector<Interval>>& allBusy) {
    vector<Interval> events;
    for (size_t i = 0; i < allBusy.size(); ++i)
        events.insert(events.end(), allBusy[i].begin(), allBusy[i].end());
    if (events.empty()) return vector<Interval>();
    sort(events.begin(), events.end());
    vector<Interval> merged;
    for (size_t i = 0; i < events.size(); ++i) {
        Interval iv = events[i];
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
    for (size_t i = 0; i < busy.size(); ++i) {
        int s = busy[i].first;
        int e = busy[i].second;
        if (prev < s) freeSlots.push_back(make_pair(prev, s));
        prev = max(prev, e);
    }
    if (prev < DAY) freeSlots.push_back(make_pair(prev, DAY));
    return freeSlots;
}

static vector<pair<string, string>> findMeetingSlots(
    const vector<vector<pair<string, string>>>& schedules,
    const vector<pair<string, string>>& dailyActive,
    int durationMinutes
) {
    size_t k = schedules.size();
    vector<vector<Interval>> busyLists(k);
    vector<Interval> actives(k);

    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < schedules[i].size(); ++j) {
            string s = schedules[i][j].first;
            string e = schedules[i][j].second;
            busyLists[i].push_back(make_pair(toMinutes(s), toMinutes(e)));
        }
        sort(busyLists[i].begin(), busyLists[i].end());
        actives[i] = make_pair(toMinutes(dailyActive[i].first), toMinutes(dailyActive[i].second));
    }

    vector<vector<Interval>> normBusy(k);
    for (size_t i = 0; i < k; ++i)
        normBusy[i] = normalizeBusy(busyLists[i], actives[i]);

    vector<Interval> mergedBusy = unionBusyAll(normBusy);
    vector<Interval> freeSlots = invertToFree(mergedBusy);

    // Intersection of everyone's active times
    int globalStart = 0, globalEnd = 24 * 60;
    for (size_t i = 0; i < actives.size(); ++i) {
        globalStart = max(globalStart, actives[i].first);
        globalEnd = min(globalEnd, actives[i].second);
    }

    vector<pair<string, string>> answer;
    for (size_t i = 0; i < freeSlots.size(); ++i) {
        int s = freeSlots[i].first;
        int e = freeSlots[i].second;
        int ss = max(s, globalStart);
        int ee = min(e, globalEnd);
        if (ee - ss >= durationMinutes)
            answer.push_back(make_pair(toHM(ss), toHM(ee)));
    }
    return answer;
}

// -------- Demo main --------
int main() {
    // Sample data from project handout
    vector<vector<pair<string, string>>> schedules = {
        { make_pair("7:00","8:30"), make_pair("12:00","13:00"), make_pair("16:00","18:00") },
        { make_pair("9:00","10:30"), make_pair("12:20","14:00"), make_pair("14:30","15:00"), make_pair("16:00","17:00") }
    };
    vector<pair<string, string>> actives = {
        make_pair("09:00","19:00"),
        make_pair("09:00","18:30")
    };
    int duration = 30;

    vector<pair<string, string>> ans = findMeetingSlots(schedules, actives, duration);
    cout << "Available common slots (>= " << duration << " min):\n";
    for (size_t i = 0; i < ans.size(); ++i)
        cout << "[" << ans[i].first << ", " << ans[i].second << "]\n";
    return 0;
}