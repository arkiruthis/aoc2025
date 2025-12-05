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
    bool merge_required = false;

    while (!merge_required)
    {
        merge_required = false;
        for (auto it = cbegin(ranges); it != cend(ranges); ++it)
        {
            bool was_merged_in_this_iteration = false;
            for (auto it2 = cbegin(ranges); it2 != cend(ranges); ++it2)
            {
                if (it == it2)
                    continue; // Self comparison

                auto a = (*it);
                auto b = (*it2);

                if (b.first <= a.second && b.first >= a.first)
                {
                    // Merging LR
                    merged_ranges.push_back({a.first, b.second});
                    printf("LR: [%llu-%llu] from [%llu-%llu][%llu-%llu]\n", a.first, b.second, a.first, a.second, b.first, b.second);
                    was_merged_in_this_iteration = true;
                }
                else if (a.first <= b.second && a.first >= b.first)
                {
                    // Merging RL
                    merged_ranges.push_back({b.first, a.second});
                    printf("RL: [%llu-%llu] from [%llu-%llu][%llu-%llu]\n", b.first, a.second, a.first, a.second, b.first, b.second);
                    was_merged_in_this_iteration = true;
                }
            }

            if (was_merged_in_this_iteration)
            {
                printf("Merging!!\n");
                merge_required = true;
            }
            else
            {
                // Keep it in the new bucket
                merged_ranges.push_back(*it);
            }
        }

        if (merge_required)
        {
            // Overwrite original ranges
            ranges = merged_ranges;
        }

        ++iterations;
        printf("Have completed (%llu) iterations so far...\n", iterations);
    }

    // Now we just get the ranges (inclusive) and add them. :) 
    // [ 2-4 ] = (4 - 2) + 1, aka, {2, 3, 4}
    for (auto &r : ranges) {
        assert(r.second >= r.first);
        count += (r.second - r.first);
    }

    printf("%llu ingredients are fresh.\n", count);

    return 0;
}
