#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include <map>

using namespace std;

// Day 12 - Use bitmasks and see if we can just brute force shape placement.
// I think we can rotate, flip, rotate, flip?
// Update - nope. The _actual_ data set is much larger area, so it's just an
// area calculation. Which makes the test data a bit of a troll.

int main(int argc, char *argv[])
{
    ifstream in_file;

    in_file.open("assets/aoc12.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    string line;
    size_t answer = 0;

    // Update - whole thing has been a glorious troll. This is just an area problem. :/
    // Ignore all the shape nonsense.
    // Next we have a series of WxH: a b c d e f where each letter is the count of that shape
    while (getline(in_file, line))
    {
        // Skip lines that don't have 'x' (shape definitions, blank lines, etc.)
        size_t space_pos = line.find('x');
        if (space_pos == string::npos)
            continue;

        auto sep_pos = line.find(':');
        if (sep_pos == string::npos)
            continue;

        string size_str = line.substr(0, sep_pos);
        size_t W        = stoi(size_str.substr(0, space_pos));
        size_t H        = stoi(size_str.substr(space_pos + 1));
        size_t area     = W * H;

        // Sum up all the shape counts
        string counts_str = line.substr(sep_pos + 2); // inc space
        size_t presents   = 0;

        do
        {
            auto pos = counts_str.find_first_of(" ");
            if (pos == string::npos)
            {
                presents += stoi(counts_str);
                break;
            }
            string count_str = counts_str.substr(0, pos);
            presents += stoi(count_str);
            counts_str = counts_str.substr(pos + 1);
        } while (true);

        bool canFit = (area >= presents * 9);
        printf("Board %zux%zu: area=%zu, presents=%zu, canFit=%s\n", W, H, area, presents, canFit ? "YES" : "NO");

        if (canFit)
            answer++;
    }

    printf("\nAnswer: %zu\n", answer);
    in_file.close();

    return 0;
}
