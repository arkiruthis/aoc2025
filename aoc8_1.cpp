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

    string toString() const
    {
        return "V3D(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
    }
} V3D;

class BoxConnection
{
public:
    V3D boxA;
    V3D boxB;
    float distance;

    bool operator<(const BoxConnection &other) const
    {
        return distance < other.distance;
    }

    bool operator==(const BoxConnection &other)
    {
        return (boxA.id == other.boxA.id && boxB.id == other.boxB.id) ||
               (boxA.id == other.boxB.id && boxB.id == other.boxA.id);
    }
};

// We don't need the ACTUAL distance here, just relative
float distSquared(const V3D &a, const V3D &b)
{
    return (b.x - a.x) * (b.x - a.x) +
           (b.y - a.y) * (b.y - a.y) +
           (b.z - a.z) * (b.z - a.z);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
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
                pairs.insert({.boxA = v, .boxB = *nearest, .distance = distSquared(v, *nearest)});
            }
        }
        return pairs;
    };

    set<BoxConnection> pairsByDistance = getPairsSortedByDistance(boxPositions);

    printf("Connections by closest distance:\n");
    for (const auto &conn : pairsByDistance)
    {
        printf("Box %zu <--> Box %zu : Dist^2 = %.2f\n", conn.boxA.id, conn.boxB.id, conn.distance);
    }

    auto circuitFromConnections = [](const set<BoxConnection> &connections)
    {
        vector<size_t> circuit;
        circuit.push_back(connections.begin()->boxA.id);

        for (const auto &conn : connections)
        {
            if (find(circuit.begin(), circuit.end(), conn.boxA.id) != circuit.end())
            {
                // Found a candidate to connect
                printf("Pushing box %s to circuit\n", conn.boxB.toString().c_str());
                circuit.push_back(conn.boxB.id);
            }
            else if (find(circuit.begin(), circuit.end(), conn.boxB.id) != circuit.end())
            {
                // Found a candidate to connect
                printf("Pushing box %s to circuit\n", conn.boxA.toString().c_str());
                circuit.push_back(conn.boxA.id);
            }
        }
        return circuit;
    };

    vector<size_t> circuit = circuitFromConnections(pairsByDistance);

    printf("Circuit size: %zu\n", circuit.size());
    // printf("Circuit order:\n");
    // for (const auto &cid : circuit)
    // {
    //     printf("Box %zu\n", cid);
    // }

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
        printf("Box %zu <--> Box %zu : Dist^2 = %.2f\n", conn.boxA.id, conn.boxB.id, conn.distance);
    }

    circuit = circuitFromConnections(pairsByDistance);
    printf("Circuit size: %zu\n", circuit.size());

    return 0;
}
