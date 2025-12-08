#include <algorithm>
#include <cstdio>
#include <fstream>
#include <list>
#include <set>
#include <string>
#include <vector>

using namespace std;

typedef struct V3D
{
    float x;
    float y;
    float z;
    size_t id;
    bool connected = false;

    bool operator<(const V3D &other) const { return id < other.id; }

    string toString() const
    {
        return "V3D(" + to_string((int)x) + ", " + to_string((int)y) + ", " + to_string((int)z) + ")";
    }
} V3D;

class BoxConnection
{
public:
    V3D boxA;
    V3D boxB;
    float distance;

    bool operator<(const BoxConnection &other) const { return distance < other.distance; }

    bool operator==(const BoxConnection &other)
    {
        return (boxA.id == other.boxA.id && boxB.id == other.boxB.id) ||
               (boxA.id == other.boxB.id && boxB.id == other.boxA.id);
    }
};

// We don't need the ACTUAL distance here, just relative
float distSquared(const V3D &a, const V3D &b)
{
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    ifstream in_file;
    vector<V3D> boxPositions;

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
            auto end   = line.find_last_of(',');

            boxPositions.push_back({ .x         = stof(line.substr(0, start)),
                                     .y         = stof(line.substr(start + 1, end)),
                                     .z         = stof(line.substr(end + 1)),
                                     .id        = idx,
                                     .connected = false });
        }
        ++idx;
    }
    in_file.close();

    // Create a container of sorted pairs by lowest distance
    auto getPairsSortedByDistance = [](const vector<V3D> &boxes) {
        set<BoxConnection> pairs;
        for (auto i = 0; i < boxes.size(); ++i)
        {
            const auto &v = boxes[i];
            auto nearest  = min_element(cbegin(boxes), cend(boxes), [&v](const auto &a, const auto &b) {
                // Skip comparing with self
                float distA = (&a == &v) ? INFINITY : distSquared(v, a);
                float distB = (&b == &v) ? INFINITY : distSquared(v, b);
                return distA < distB;
            });

            if (nearest != cend(boxes))
            {
                pairs.insert({ .boxA = v, .boxB = *nearest, .distance = distSquared(v, *nearest) });
            }
        }
        return pairs;
    };

    auto printCircuit = [](const set<V3D> &circuit) {
        printf("Circuit size: %zu\n", circuit.size());

        for (const auto &box : circuit)
        {
            printf("%s\n", box.toString().c_str());
        }
    };

    set<BoxConnection> pairsByDistance = getPairsSortedByDistance(boxPositions);

    auto circuitFromConnections = [](set<BoxConnection> &connections) {
        // Start circuit with first connection
        vector<set<V3D>> circuits = {{ connections.begin()->boxA, connections.begin()->boxB }};

        const set<BoxConnection> initialConnections = connections;

        // We'll look for other connections to add to the circuit
        for (const auto &conn : initialConnections)
        {
            bool found = false;

            for (auto &circuit : circuits)
            {
                if (find_if(circuit.begin(), circuit.end(), [&](const V3D& v) { return v.id == conn.boxA.id; }) != circuit.end())
                {
                    // Found a candidate to connect
                    circuit.insert(conn.boxB);
                    found = true;
                }
                else if (find_if(circuit.begin(), circuit.end(), [&](const V3D& v) { return v.id == conn.boxB.id; }) != circuit.end())
                {
                    // Found a candidate to connect
                    circuit.insert(conn.boxA);
                    found = true;
                }
            }

            if (!found)
            {
                // No existing circuit found, create a new one
                circuits.push_back({ conn.boxA, conn.boxB });
            }
        }

        return circuits;
    };

    vector<set<V3D>> circuits = circuitFromConnections(pairsByDistance);
    for (const auto& circuit : circuits) {
        printCircuit(circuit);
        printf("\n");
    }


    return 0;
}
