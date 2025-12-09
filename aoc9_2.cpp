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

typedef struct Rect
{
    V2D topLeft;
    V2D bottomRight;
} Rect;

V2D minV2D(const V2D &a, const V2D &b)
{
    return { min(a.x, b.x), min(a.y, b.y) };
}

V2D maxV2D(const V2D &a, const V2D &b)
{
    return { max(a.x, b.x), max(a.y, b.y) };
}

bool isPointInRect(const V2D &pt, const Rect &r)
{
    // We will allow points on the edge to be considered outside
    return (pt.x > r.topLeft.x && pt.x < r.bottomRight.x && pt.y > r.topLeft.y && pt.y < r.bottomRight.y);
}

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

        if (rightTurn && (abs(area) > largestArea))
        {
            printf("VALID rect, area = %lld\n", abs(area));

            // Attempt to disqualify if any point in our list is INSIDE this rect
            Rect r = { minV2D(p1, p3), maxV2D(p1, p3) };
            printf(
                "Rect from (%lld, %lld) to (%lld, %lld)\n", r.topLeft.x, r.topLeft.y, r.bottomRight.x, r.bottomRight.y);

            bool invalidated = false;
            for (size_t j = 0; j < tileCoords.size(); ++j)
            {
                if (j != i && j != (i + 1) % tileCoords.size() && j != (i + 2) % tileCoords.size())
                {
                    const auto &pt = tileCoords[j];
                    if (isPointInRect(pt, r))
                    {
                        printf("======> Point (%lld, %lld) is inside rect, invalidating\n", pt.x, pt.y);
                        invalidated = true;
                        break;
                    }
                }
            }

            if (!invalidated)
            {
                largestArea = abs(area);
            }
        }
        else
        {
            printf("invalid rect\n");
        }
    }

    printf("Largest valid area: %lld\n", largestArea);

    return 0;
}
