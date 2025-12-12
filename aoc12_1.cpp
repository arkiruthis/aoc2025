#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

// Day 12 - Use bitmasks and see if we can just brute force shape placement.
// I think we can rotate, flip, rotate, flip?
class Shape
{
public:
    uint8_t original[3][3];
    uint8_t mask[3][3];
    uint8_t temp[3][3];

    void flip()
    {
        std::swap(mask[0][0], mask[0][2]);
        std::swap(mask[1][0], mask[1][2]);
        std::swap(mask[2][0], mask[2][2]);
    }

    void rotateCCW()
    {
        for (size_t r = 0; r < 3; r++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                temp[2 - c][r] = mask[r][c];
            }
        }

        memcpy(mask, temp, sizeof(mask));
    }

    void print()
    {
        for (size_t r = 0; r < 3; r++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                printf("%c", mask[r][c] ? '#' : '.');
            }
            printf("\n");
        }
    }

    void reset()
    {
        memcpy(mask, original, sizeof(mask));
    }
};

int main(int argc, char *argv[])
{
    ifstream in_file;

    in_file.open("assets/aoc12_example.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    string line;
    // Both example and full data have 6 shapes with a space line between
    vector<Shape> shapes;
    size_t shape_count = 0;

    for (size_t i = 0; i < 6; i++)
    {
        Shape shape;

        getline(in_file, line); // Ignore label line
        for (size_t r = 0; r < 3; r++)
        {
            getline(in_file, line);
            for (size_t c = 0; c < 3; c++)
            {
                char ch = line[c];
                assert(ch == '#' || ch == '.');
                shape.original[r][c] = (ch == '#') ? 1 : 0;
                shape.mask[r][c]     = shape.original[r][c];
            }
        }
        shapes.push_back(shape);

        // Read the blank line
        getline(in_file, line);
    }
    in_file.close();

    // Print all shapes and their rotations/flips
    for (size_t s = 0; s < shapes.size(); s++)
    {
        Shape &shape = shapes[s];
        printf("Shape %zu:\n", s);
        for (size_t rot = 0; rot < 4; rot++)
        {
            for (size_t flip = 0; flip < 2; flip++)
            {
                shape.print();
                printf("\n");
                shape.flip();
            }
            shape.rotateCCW();
        }

        printf("--------\n");
    }

    return 0;
}
