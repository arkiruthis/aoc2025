#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include <utility>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<pair<uint64_t, uint64_t>> ranges;
    vector<pair<uint64_t, uint64_t>> merged_ranges;

    in_file.open("assets/aoc5.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    // This time, we just want the ranges and ignore IDs.
    string line;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            const auto pos = line.find('-');

            if (pos != std::string::npos)
            {
                const auto minR = stoull(line.substr(0, pos));
                const auto maxR = stoull(line.substr(pos + 1));
                ranges.push_back({minR, maxR});
            }
        }
    }
    in_file.close();

    uint64_t count = 0, iterations = 0;

    std::sort(begin(ranges), end(ranges), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    // Merge together
    for (auto it = begin(ranges); it != end(ranges); ++it)
    {
        if (merged_ranges.empty())
        {
            merged_ranges.push_back(*it);
        }
        else
        {
            auto &last = merged_ranges.back();
            if (it->first <= last.second + 1)
            {
                // Overlap or contiguous, merge
                if (it->second > last.second)
                {
                    last.second = it->second;
                }
            }
            else
            {
                // No overlap, add new range
                merged_ranges.push_back(*it);
            }
        }
    }

    // Now we just get the ranges (inclusive) and add them. :) 
    // [ 2-4 ] = (4 - 2) + 1, aka, {2, 3, 4}
    for (auto &r : merged_ranges) {
        assert(r.second >= r.first);
        count += (r.second - r.first) + 1;
    }

    printf("%llu ingredients are fresh.\n", count);

    return 0;
}
