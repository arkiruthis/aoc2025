#include <algorithm>
#include <cstdio>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

typedef struct V2D
{
    int64_t x;
    int64_t y;
} V2D;

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
        return (t1ID == other.t1ID && t2ID == other.t2ID); // || (t1ID == other.t2ID && t2ID == other.t1ID);
    }

    // void print() const { printf("%zu, %zu, size: %zu\n", t1ID, t2ID, maxArea); }
    void print() const { printf("Tile IDs: %zu, %zu | Area: %zu\n", t1ID, t2ID, maxArea); }

    size_t t1ID;
    size_t t2ID;
    size_t maxArea;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    ifstream in_file;
    vector<V2D> tileCoords;

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

    int64_t largestArea = 0;

    // According to the example, these points are linked and wind in a clockwise manner.
    for (size_t i = 0; i < tileCoords.size(); ++i)
    {
        const auto &p1 = tileCoords[i];
        const auto &p2 = tileCoords[(i + 1) % tileCoords.size()];
        const auto &p3 = tileCoords[(i + 2) % tileCoords.size()];
        printf("Line from (%lld, %lld) to (%lld, %lld) to (%lld, %lld)\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

        int64_t dx     = 0;
        int64_t dy     = 0;
        bool rightTurn = false;

        if (p1.y == p2.y)
        {
            // Horizontal
            dx = p2.x - p1.x;
            assert(p2.y != p3.y);

            // Next one assumed to be vertical
            dy = p3.y - p2.y;

            // A right turn is +X followed by +Y, or -X followed by -Y
            rightTurn = (dx > 0 && dy > 0) || (dx < 0 && dy < 0);
        }
        else if (p1.x == p2.x)
        {
            // Vertical
            dy = p2.y - p1.y;
            assert(p2.x != p3.x);

            // Next one assumed to be vertical
            dx = p3.x - p2.x;

            // A right turn is +Y followed by -X, or -Y followed by +X
            rightTurn = (dy > 0 && dx < 0) || (dy < 0 && dx > 0);
        }
        else
        {
            assert("Non aligned coordinates discovered");
        }

        auto area = (abs(dx) + 1) * (abs(dy) + 1); // If this is positive, we calculated in a right turn corner

        if (rightTurn) //  && (abs(area) > largestArea))
        {
            largestArea = abs(area);

            printf("VALID rect, area = %lld\n", largestArea);

            // Attempt to disqualify if any point thus far is INSIDE this rect
        }
        else
        {
            printf("invalid rect\n");
        }
    }

    return 0;
}
