#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream in_file;
    size_t idx = 0;
    map<size_t, size_t> beams; // position -> count

    in_file.open("assets/aoc7.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.\n");
        return 1;
    }

    string line;
    getline(in_file, line);
    if (!line.empty() && (idx = line.find("S")) != string::npos)
    {
        beams[idx] = 1;
        printf("Starting point at index: %zu\n", idx);
    }
    else
    {
        printf("Empty file.\n");
        return 1;
    }

    size_t splitCount = 0;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            map<size_t, size_t> newBeams;

            for (auto [pos, count] : beams) {
                if (line[pos] == '^') {
                    splitCount++;
                    line[pos - 1] = '|';
                    line[pos + 1] = '|';
                    newBeams[pos - 1] += count;
                    newBeams[pos + 1] += count;
                } else {
                    line[pos] = '|';
                    newBeams[pos] += count;
                }
            }
            beams = newBeams;
            printf("%s\n", line.c_str());
        }
    }
    in_file.close();

    size_t totalJourneys = 0;
    for (auto [pos, count] : beams) {
        totalJourneys += count;
    }

    printf("Split count: %zu\n", splitCount);
    printf("Unique journeys: %zu\n", totalJourneys);

    return 0;
}
