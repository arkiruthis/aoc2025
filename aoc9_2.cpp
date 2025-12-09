#include <algorithm>
#include <cstdio>
#include <fstream>
#include <set>
#include <string>
#include <vector>

using namespace std;

using TileCoord = pair<int64_t, int64_t>;

struct Rect
{
    size_t minX, minY, maxX, maxY;

    bool intersects(const Rect &other) const
    {
        // They can touch edges not be considered intersecting
        const bool outside = maxX <= other.minX || other.maxX <= minX || maxY <= other.minY || other.maxY <= minY;
        return !outside;
    }

    bool contains_corner(const Rect &other) const
    {
        return (other.minX >= minX && other.minX <= maxX && other.minY >= minY && other.minY <= maxY) ||
               (other.maxX >= minX && other.maxX <= maxX && other.minY >= minY && other.minY <= maxY) ||
               (other.minX >= minX && other.minX <= maxX && other.maxY >= minY && other.maxY <= maxY) ||
               (other.maxX >= minX && other.maxX <= maxX && other.maxY >= minY && other.maxY <= maxY);
    }
};

class TileSet
{
public:
    TileSet(size_t id1, size_t id2, size_t area, TileCoord t1, TileCoord t2)
        : t1ID(id1)
        , t2ID(id2)
        , maxArea(area)
    {
        bounds.minX = static_cast<size_t>(min(t1.first, t2.first));
        bounds.minY = static_cast<size_t>(min(t1.second, t2.second));
        bounds.maxX = static_cast<size_t>(max(t1.first, t2.first));
        bounds.maxY = static_cast<size_t>(max(t1.second, t2.second));

        t1Coord = t1;
        t2Coord = t2;
    }

    bool operator<(const TileSet &other) const { return maxArea < other.maxArea; }

    bool operator==(const TileSet &other) const
    {
        return (t1ID == other.t1ID && t2ID == other.t2ID); // || (t1ID == other.t2ID && t2ID == other.t1ID);
    }

    // void print() const { printf("%zu, %zu, size: %zu\n", t1ID, t2ID, maxArea); }
    void print() const
    {
        printf(
            "Tile IDs: %zu, %zu | Area: %zu | Bounds: (%zu, %zu) to (%zu, %zu)\n",
            t1ID,
            t2ID,
            maxArea,
            t1Coord.first,
            t1Coord.second,
            t2Coord.first,
            t2Coord.second);
    }

    size_t t1ID;
    size_t t2ID;
    size_t maxArea;
    Rect bounds;
    TileCoord t1Coord;
    TileCoord t2Coord;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    ifstream in_file;
    vector<TileCoord> tileCoords;

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
                const auto &t1 = tileCoords[i];
                const auto &t2 = tileCoords[j];

                if (t1.first <= t2.first && t1.second <= t2.second)
                {
                    // Valid tile
                    const auto w      = t2.first - t1.first + 1LL;
                    const auto h      = t2.second - t1.second + 1LL;
                    const size_t area = static_cast<size_t>(w * h);
                    tileMatrix.insert({ i, j, area, tileCoords[i], tileCoords[j] });
                }
            }
        }
    }

    // Go through all tilesets, starting from the largest area, and find the first
    // that does not intersect with any other tileset.
    for (auto it = tileMatrix.rbegin(); it != tileMatrix.rend(); ++it)
    {
        bool intersects = false;
        for (auto it2 = tileMatrix.rbegin(); it2 != tileMatrix.rend(); ++it2)
        {
            if (it != it2 && it->bounds.contains_corner(it2->bounds))
            {
                printf("Aww dang... we intersected...\n");
                it->print();
                it2->print();
                intersects = true;
            }
        }

        if (!intersects)
        {
            printf("Largest non-intersecting tile set:\n");
            it->print();
            break;
        }
    }

    return 0;
}
