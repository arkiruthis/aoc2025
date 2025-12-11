#include <cstdio>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Day 11 feels like a node-based problem if ever I saw one.

typedef struct Node
{
    string id; // These should be small so benefit from string usage
    vector<Node *> connections;
} Node;

// Helper function to find or create a node
Node *findOrCreateNode(vector<Node *> &nodes, const string &id)
{
    auto it = find_if(nodes.begin(), nodes.end(), [&id](const Node *n) { return n->id == id; });
    if (it != nodes.end())
    {
        return *it;
    }
    // Node doesn't exist, create it
    Node *new_node = new Node();
    new_node->id   = id;
    nodes.push_back(new_node);
    return new_node;
}

size_t recurseCountOut(Node *root)
{
    if (!root)
    {
        return 0;
    }

    if (root && root->id == "out")
    {
        return 1;
    }

    size_t total = 0;
    for (auto conn : root->connections)
    {
        total += recurseCountOut(conn);
    }

    return total;
}

int main(int argc, char *argv[])
{
    ifstream in_file;
    Node *root = new Node();
    vector<string> data;
    vector<Node *> nodes;

    in_file.open("assets/aoc11_example.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    string line;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            data.push_back(line);
        }
    }
    in_file.close();

    // Parse each line and create connections
    for (const auto &line : data)
    {
        auto separator_pos = line.find(":");
        string node_id     = line.substr(0, separator_pos);
        string connections = line.substr(separator_pos + 2); // Skip ": "

        Node *current_node = findOrCreateNode(nodes, node_id);

        // Parse connections
        size_t pos = 0;
        while ((pos = connections.find(" ")) != string::npos)
        {
            string conn_id = connections.substr(0, pos);
            if (!conn_id.empty())
            {
                Node *conn_node = findOrCreateNode(nodes, conn_id);
                current_node->connections.push_back(conn_node);
            }
            connections.erase(0, pos + 1);
        }
        // Handle the last connection
        if (!connections.empty())
        {
            Node *conn_node = findOrCreateNode(nodes, connections);
            current_node->connections.push_back(conn_node);
        }
    }

    // For demonstration, print out the nodes and their connections
    for (const auto node : nodes)
    {
        printf("Node %s connections: ", node->id.c_str());
        for (const auto conn : node->connections)
        {
            printf("%s ", conn->id.c_str());
        }
        printf("\n");
    }

    root = findOrCreateNode(nodes, "you");
    size_t out_count = recurseCountOut(root);
    printf("--------------------------\n");
    printf("Total 'out' connections from 'you' to 'out': %zu\n", out_count);

    // Cleanup
    for (auto node : nodes)
    {
        delete node;
    }
    delete root;

    return 0;
}
