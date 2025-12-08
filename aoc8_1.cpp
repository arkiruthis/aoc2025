#include <algorithm>
#include <cstdio>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

typedef struct V3D
{
    float x;
    float y;
    float z;
    size_t id;

    bool operator<(const V3D &other) const { return id < other.id; }

    string toString() const
    {
        return "V3D(" + to_string((int)x) + ", " + to_string((int)y) + ", " + to_string((int)z) + ")";
    }
} V3D;

class BoxConnection
{
public:
    size_t boxAId;
    size_t boxBId;
    float distance;

    bool operator<(const BoxConnection &other) const { return distance < other.distance; }
};

// We don't need the ACTUAL distance here, just relative
float distSquared(const V3D &a, const V3D &b)
{
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z);
}

// Union-Find data structure
// https://en.wikipedia.org/wiki/Disjoint-set_data_structure
// Honestly, unless you're an academic, I have no idea how you'd know this.
class UnionFind
{
private:
    vector<size_t> parent;
    vector<size_t> rank;

public:
    UnionFind(size_t n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        for (size_t i = 0; i < n; ++i)
        {
            parent[i] = i;
        }
    }

    size_t find(size_t x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }

    bool unite(size_t x, size_t y)
    {
        size_t rootX = find(x);
        size_t rootY = find(y);

        if (rootX == rootY)
        {
            return false; // Already in same set
        }

        // Union by rank
        if (rank[rootX] < rank[rootY])
        {
            parent[rootX] = rootY;
        }
        else if (rank[rootX] > rank[rootY])
        {
            parent[rootY] = rootX;
        }
        else
        {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    }

    map<size_t, size_t> getCircuitSizes()
    {
        map<size_t, size_t> sizes;
        for (size_t i = 0; i < parent.size(); ++i)
        {
            sizes[find(i)]++;
        }
        return sizes;
    }
};

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

            boxPositions.push_back({ .x  = stof(line.substr(0, start)),
                                     .y  = stof(line.substr(start + 1, end)),
                                     .z  = stof(line.substr(end + 1)),
                                     .id = idx });
        }
        ++idx;
    }
    in_file.close();

    // Create a container of sorted pairs by lowest distance
    vector<BoxConnection> pairs;
    for (size_t i = 0; i < boxPositions.size(); ++i)
    {
        for (size_t j = i + 1; j < boxPositions.size(); ++j)
        {
            pairs.push_back({ .boxAId = i, .boxBId = j, .distance = distSquared(boxPositions[i], boxPositions[j]) });
        }
    }

    sort(pairs.begin(), pairs.end()); // Back to vectors. Yee-haa...

    // Make connections using the flippin' Union-Find
    UnionFind uf(boxPositions.size());
    int connectionsAttempted    = 0;
    const int targetConnections = 10;

    for (const auto &pair : pairs)
    {
        uf.unite(pair.boxAId, pair.boxBId);
        connectionsAttempted++;
        if (connectionsAttempted >= targetConnections)
        {
            break;
        }
    }

    // Get circuit sizes
    auto circuitSizes = uf.getCircuitSizes();
    vector<size_t> sizes;
    for (const auto &[root, size] : circuitSizes)
    {
        sizes.push_back(size);
    }

    sort(sizes.rbegin(), sizes.rend()); // Largest first

    printf("Made %d connection attempts\n", connectionsAttempted);
    printf("Number of circuits: %zu\n", circuitSizes.size());
    printf("Three largest circuits: %zu, %zu, %zu\n", sizes[0], sizes[1], sizes[2]);
    printf("Product: %zu\n", sizes[0] * sizes[1] * sizes[2]);

    return 0;
}
