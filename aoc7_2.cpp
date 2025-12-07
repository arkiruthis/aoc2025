#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <set>
#include <map>

using namespace std;

// Well well well... if it isn't 1997 Comp Sci coming back to haunt me.
typedef struct Node {
    unique_ptr<Node> left;
    unique_ptr<Node> right;
} Node;

size_t countJourneys(Node* node) {
    // In theory we shouldn't ever hit this...
    if (!node)
        return 0;

    // Hit the final leaf
    if (!node->left && !node->right) {
        return 1;
    }
    
    // Count journeys through left and right subtrees
    size_t leftJourneys = countJourneys(node->left.get());
    size_t rightJourneys = countJourneys(node->right.get());
    
    return leftJourneys + rightJourneys;
}

int main(int argc, char *argv[])
{
    ifstream in_file;
    size_t idx = 0;
    set<size_t> beams;

    in_file.open("assets/aoc7.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.\n");
        return 1;
    }

    string line;
    getline(in_file, line);
    if (!line.empty() && (idx = line.find("S")) != string::npos)
    {
        beams.insert(idx);
        printf("Starting point at index: %zu\n", idx);
    }
    else
    {
        printf("Empty file.\n");
        return 1;
    }

    // NICK BRAIN - this has the feel of an A* binary search about it. 

    unique_ptr<Node> parent = make_unique<Node>();
    // We need to track both beams, and node children, so we'll use a pair
    vector<pair<size_t, Node*>> activeBeams = {{idx, parent.get()}};
    
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            vector<pair<size_t, Node*>> nextBeams;

            for (auto [pos, current] : activeBeams) {
                if (line[pos] == '^') {
                    line[pos - 1] = '|';
                    line[pos + 1] = '|';
                    
                    // Create child nodes for the split
                    current->left = make_unique<Node>();
                    current->right = make_unique<Node>();
                    
                    nextBeams.push_back({pos - 1, current->left.get()});
                    nextBeams.push_back({pos + 1, current->right.get()});
                    
                } else {
                    line[pos] = '|';
                    nextBeams.push_back({pos, current});
                }
            }
            
            activeBeams = nextBeams;
            printf("%s\n", line.c_str());
        }
    }
    in_file.close();

    size_t journeys = countJourneys(parent.get());
    printf("\nUnique journeys: %zu\n", journeys);

    return 0;
}
