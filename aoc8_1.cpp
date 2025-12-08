#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <set>

using namespace std;

typedef struct V3D
{
    float x;
    float y;
    float z;
    size_t id;
    bool connected = false;
} V3D;

class BoxConnection
{
public:
    size_t boxID1;
    size_t boxID2;
    float distance;

    bool operator<(const BoxConnection &other) const
    {
        return distance < other.distance;
    }

    bool operator==(const BoxConnection &other)
    {
        return (boxID1 == other.boxID1 && boxID2 == other.boxID2) ||
               (boxID1 == other.boxID2 && boxID2 == other.boxID1);
    }
};

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

    // Read in our vertices
    string line;
    size_t idx = 0;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            auto start = line.find_first_of(',');
            auto end = line.find_last_of(',');

            boxPositions.push_back({.x = stof(line.substr(0, start)),
                                    .y = stof(line.substr(start + 1, end)),
                                    .z = stof(line.substr(end + 1)),
                                    .id = idx,
                                    .connected = false});
        }
        ++idx;
    }
    in_file.close();

    // Create a container of sorted pairs by lowest distance
    auto getPairsSortedByDistance = [](const vector<V3D> &boxes)
    {
        set<BoxConnection> pairs;
        for (auto i = 0; i < boxes.size(); ++i)
        {
            const auto &v = boxes[i];
            auto nearest = min_element(cbegin(boxes), cend(boxes),
                                       [&v](const auto &a, const auto &b)
                                       {
                                           // Skip comparing with self
                                           float distA = (&a == &v) ? INFINITY : distSquared(v, a);
                                           float distB = (&b == &v) ? INFINITY : distSquared(v, b);
                                           return distA < distB;
                                       });

            if (nearest != cend(boxes))
            {
                pairs.insert({.boxID1 = v.id, .boxID2 = nearest->id, .distance = distSquared(v, *nearest)});
            }
        }
        return pairs;
    };

    set<BoxConnection> pairsByDistance = getPairsSortedByDistance(boxPositions);

    printf("Connections by closest distance:\n");
    for (const auto &conn : pairsByDistance)
    {
        printf("Box %zu <--> Box %zu : Dist^2 = %.2f\n", conn.boxID1, conn.boxID2, conn.distance);
    }

    auto circuitFromConnections = [](const set<BoxConnection> &connections)
    {
        vector<size_t> circuit;
        circuit.push_back(connections.begin()->boxID1);

        for (const auto &conn : connections)
        {
            if (find(circuit.begin(), circuit.end(), conn.boxID1) != circuit.end())
            {
                // Found a candidate to connect
                printf("Pushing box %zu to circuit\n", conn.boxID2);
                circuit.push_back(conn.boxID2);
            }
            else if (find(circuit.begin(), circuit.end(), conn.boxID2) != circuit.end())
            {
                // Found a candidate to connect
                printf("Pushing box %zu to circuit\n", conn.boxID1);
                circuit.push_back(conn.boxID1);
            }
        }
        return circuit;
    };

    vector<size_t> circuit = circuitFromConnections(pairsByDistance);

    printf("Circuit order:\n");
    for (const auto &cid : circuit)
    {
        printf("Box %zu\n", cid);
    }

    // Remove all ids in circuit from boxPositions
    for (const auto &cid : circuit)
    {
        boxPositions.erase(remove_if(boxPositions.begin(), boxPositions.end(),
                                     [cid](const V3D &v)
                                     { return v.id == cid; }),
                           boxPositions.end());
    }

    // Print remaining boxes
    printf("Remaining boxes:\n");
    for (const auto &box : boxPositions)
    {
        printf("Box %zu at (%.2f, %.2f, %.2f)\n", box.id, box.x, box.y, box.z);
    }

    pairsByDistance = getPairsSortedByDistance(boxPositions);

    printf("Connections by closest distance:\n");
    for (const auto &conn : pairsByDistance)
    {
        printf("Box %zu <--> Box %zu : Dist^2 = %.2f\n", conn.boxID1, conn.boxID2, conn.distance);
    }

    return 0;
}
