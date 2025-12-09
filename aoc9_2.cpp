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
    V2D min;
    V2D max;

    size_t area() const
    {
        assert(max.x >= min.x);
        assert(max.y >= min.y);
        return (max.x - min.x + 1) * (max.y - min.y + 1);
    }
} Rect;

Rect makeRect(const V2D &a, const V2D &b)
{
    Rect r;
    r.min.x = min(a.x, b.x);
    r.min.y = min(a.y, b.y);
    r.max.x = max(a.x, b.x);
    r.max.y = max(a.y, b.y);
    return r;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    ifstream in_file;
    vector<V2D> tileCoords;

    in_file.open("assets/aoc9.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    // Read in our vertices
    string line;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            auto comma = line.find_first_of(',');
            tileCoords.push_back({ stoll(line.substr(0, comma)), stoll(line.substr(comma + 1)) });
        }
    }
    in_file.close();

    // Take our candidate rectangle and test against all rectangles formed by a point and its neighbor
    // Basically a test against edge segments!!
    auto intersecting = [&tileCoords](const Rect &r) -> bool {
        for (size_t i = 0; i < tileCoords.size(); ++i)
        {
            // Get the next edge segment (wrap around) and create a thin rectangle for it
            Rect t = makeRect(tileCoords[i], tileCoords[(i + 1) % tileCoords.size()]);

            // We need the +1 and -1 here to account for touching edges
            if (t.max.y < (r.min.y + 1) || (t.min.y > r.max.y - 1) || t.max.x < (r.min.x + 1) ||
                (t.min.x > r.max.x - 1))
            {
                // No overlap, but keep going
                continue;
            }

            // We have an overlap
            return true;
        }
        return false;
    };

    // Right, screw this. Loop around the whole damn thing per vertex and brute force a rectangle test
    int64_t maxArea = 0;
    for (size_t i = 0; i < tileCoords.size(); ++i)
    {
        for (size_t j = i + 1; j < tileCoords.size(); ++j)
        {
            // This is our candidate rectangle
            Rect r = makeRect(tileCoords[i], tileCoords[j]);
            if (!intersecting(r) && r.area() > maxArea)
            {
                maxArea = r.area();
                printf(
                    "Found empty rectangle: Min(%lld,%lld) Max(%lld,%lld) Area: %zu\n",
                    r.min.x,
                    r.min.y,
                    r.max.x,
                    r.max.y,
                    r.area());
            }
        }
    }

    return 0;
}
