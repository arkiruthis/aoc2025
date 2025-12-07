#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <set>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream in_file;
    size_t idx = 0;
    set<size_t> beams;

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
        beams.insert(idx);
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
            set<size_t> newBeams;

            for (auto beam : beams) {
                if (line[beam] == '^') {
                    splitCount++;
                    line[beam - 1] = '|';
                    line[beam + 1] = '|';
                    newBeams.insert(beam - 1);
                    newBeams.insert(beam + 1);
                } else {
                    line[beam] = '|';
                    newBeams.insert(beam);
                }
            }
            beams = newBeams;
            printf("%s\n", line.c_str());
        }
    }
    in_file.close();

    printf("Split count: %zu\n", splitCount);

    return 0;
}
