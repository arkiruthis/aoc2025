#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

#define MAPSIZE 136
#define MAX_ADJACENT_ROLLS 4
static char rollMap[MAPSIZE * MAPSIZE];
static char tempMap[MAPSIZE * MAPSIZE];

inline size_t mapIdx(int x, int y)
{
    assert(x >= 0 && x < MAPSIZE && y >= 0 && y < MAPSIZE);
    return (size_t)(y * MAPSIZE + x);
}

inline bool onGrid(int x, int y)
{
    return (x >= 0 && x < MAPSIZE && y >= 0 && y < MAPSIZE);
}

inline bool notMe(int x, int y, int i, int j)
{
    return !(x == i && y == j);
}

inline bool isRoll(int x, int y)
{
    return rollMap[mapIdx(x, y)] == '@';
}

inline bool isValid(int x, int y)
{
    int count = 0;
    if (!isRoll(x, y))
        return false;

    for (int i = x - 1; i <= x + 1; ++i)
    {
        for (int j = y - 1; j <= y + 1; ++j)
        {
            if (onGrid(i, j) && notMe(x, y, i, j) && isRoll(i, j))
            {
                ++count;
            }
        }
    }

    return count < MAX_ADJACENT_ROLLS;
}

bool canCleanOut(size_t &tally)
{
    size_t count = 0;
    for (int x = 0; x < MAPSIZE; ++x)
        for (int y = 0; y < MAPSIZE; ++y)
        {
            const bool flip = isValid(x, y);
            tempMap[mapIdx(x, y)] = flip ? '.' : rollMap[mapIdx(x, y)];
            if (flip)
            {
                ++count;
            }
        }

    memcpy(rollMap, tempMap, MAPSIZE * MAPSIZE);

    printf("Cleaned out %d rolls...\n", count);

    tally += count;

    return count > 0;
}

int main(int argc, char *argv[])
{
    ifstream in_file;
    size_t forklift_count = 0;

    in_file.open("assets/aoc4.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    string line;
    int idx = 0;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            assert(line.length() == MAPSIZE);
            memcpy(&rollMap[mapIdx(0, idx)], line.data(), line.length());
        }
        printf("%d - %s\n", idx, line.c_str());
        ++idx;
    }
    in_file.close();


    while (canCleanOut(forklift_count))
    {
        for (int i = 0; i < MAPSIZE; ++i)
        {
            for (int j = 0; j < MAPSIZE; ++j)
            {
                const bool valid = isValid(i, j);
                printf("%c", valid ? 'X' : rollMap[mapIdx(i, j)]);
            }
            printf("\n");
        }
    }

    printf("Can access %zu rolls of paper.\n", forklift_count);

    return 0;
}
