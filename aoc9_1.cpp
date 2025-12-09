#include <algorithm>
#include <cstdio>
#include <fstream>
#include <set>
#include <string>
#include <vector>

using namespace std;

class TileSet
{
public:
    TileSet(size_t id1, size_t id2, size_t area)
        : t1ID(id1)
        , t2ID(id2)
        , maxArea(area)
    {
    }

    bool operator<(const TileSet &other) const { return maxArea < other.maxArea; }

    bool operator==(const TileSet &other) const
    {
        return (t1ID == other.t1ID && t2ID == other.t2ID) || (t1ID == other.t2ID && t2ID == other.t1ID);
    }

    void print() const { printf("%zu, %zu, size: %zu\n", t1ID, t2ID, maxArea); }

private:
    size_t t1ID;
    size_t t2ID;
    size_t maxArea;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    ifstream in_file;
    vector<pair<int64_t, int64_t>> tileCoords;

    in_file.open("assets/aoc9_example.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    // Read in our vertices
    string line;
    size_t idx = 0;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            auto comma = line.find_first_of(',');

            tileCoords.push_back({ stoll(line.substr(0, comma)), stoll(line.substr(comma + 1)) });
        }
        ++idx;
    }
    in_file.close();

    set<TileSet> tileMatrix;
    for (size_t i = 0; i < tileCoords.size(); ++i)
    {
        for (size_t j = 0; j < tileCoords.size(); ++j)
        {
            if (i != j)
            {
                const auto w = abs(tileCoords[i].first - tileCoords[j].first) + 1LL;
                const auto h = abs(tileCoords[i].second - tileCoords[j].second) + 1LL;

                const size_t area = static_cast<size_t>(w * h);
                // printf(
                //     "(%lld, %lld) to (%lld, %lld) has area: %lld\n",
                //     tileCoords[i].first,
                //     tileCoords[i].second,
                //     tileCoords[j].first,
                //     tileCoords[j].second,
                //     area);

                tileMatrix.insert({ i, j, area });
            }
        }
    }

    for (const auto &ts : tileMatrix)
    {
        ts.print();
    }

    return 0;
}
