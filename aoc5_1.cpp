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
    vector<uint64_t> ids;

    in_file.open("assets/aoc5.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

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
            else
            {
                ids.push_back(stoull(line));
            }
        }
    }
    in_file.close();

    uint64_t count = 0;
    for (auto &ingredient : ids)
    {
        for (auto &r : ranges)
        {
            if (ingredient >= r.first && ingredient <= r.second)
            {
                ++count;
                printf("Found %llu in range [%llu-%llu]\n", ingredient, r.first, r.second);
                break;
            }
        }
    }

    printf("%llu ingredients are fresh.\n", count);

    return 0;
}
