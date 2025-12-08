#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <list>

using namespace std;

typedef struct V3D
{
    float x;
    float y;
    float z;
    size_t id;
    bool connected = false;
} V3D;

// We don't need the ACTUAL distance here, just relative
float distSquared(const V3D &a, const V3D &b)
{
    return (b.x - a.x) * (b.x - a.x) +
           (b.y - a.y) * (b.y - a.y) +
           (b.z - a.z) * (b.z - a.z);
}

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<V3D> boxPositions;
    vector<V3D> circuits;

    in_file.open("assets/aoc8_example.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    string line;
    size_t idx = 0;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            auto start = line.find_first_of(',');
            auto end = line.find_last_of(',');

            boxPositions.push_back({
                x : stof(line.substr(0, start)),
                y : stof(line.substr(start + 1, end)),
                z : stof(line.substr(end + 1)),
                id : idx,
                connected : false
            });
        }
        ++idx;
    }
    in_file.close();

    float minDist = INFINITY;
    size_t closest_i = 0, closest_j = 0;
    vector<std::pair<size_t, size_t>> closestPairs;
    for (auto i = 0; i < boxPositions.size(); ++i)
    {
        const auto &v = boxPositions[i];
        if (v.connected) continue;
        auto nearest = min_element(begin(boxPositions), end(boxPositions),
                                   [&v, i](const auto &a, const auto &b)
                                   {
                                       // Skip comparing with self
                                       float distA = (&a == &v) ? INFINITY : distSquared(v, a);
                                       float distB = (&b == &v) ? INFINITY : distSquared(v, b);
                                       return distA < distB;
                                   });

        if (nearest != end(boxPositions) && !nearest->connected)
        {
            closestPairs.push_back({v.id, nearest->id});
            printf("Box %zu nearest to box %zu\n", v.id, nearest->id);
            boxPositions[i].connected = true;
            nearest->connected = true;
        }
    }

    // 


    return 0;
}
