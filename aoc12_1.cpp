#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>
#include <map>

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

    void reset() { memcpy(mask, original, sizeof(mask)); }
};

class Grid
{
public:
    size_t W;
    size_t H;
    vector<size_t> shape_counts;
    vector<uint8_t> data;

    size_t index(int x, int y) { return x + y * W; }

    Grid(size_t w, size_t h, vector<size_t> counts)
        : W(w)
        , H(h)
        , shape_counts(counts)
    {
        data.resize(w * h);
        clear();
    }

    bool canPlaceShape(const Shape &shape, int x, int y)
    {
        assert(x >= 0 && x + 3 <= W);
        assert(y >= 0 && y + 3 <= H);

        // SLLLLOOWWWWWWWW... OPTIMIZE LATER

        for (size_t r = 0; r < 3; r++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                if (shape.mask[r][c])
                {
                    if (data[index(x + c, y + r)] != 0)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void placeShape(const Shape &shape, int x, int y, uint8_t c)
    {
        assert(x >= 0 && x + 3 <= W);
        assert(y >= 0 && y + 3 <= H);

        // SLLLLOOWWWWWWWW... OPTIMIZE LATER

        for (size_t row = 0; row < 3; row++)
        {
            for (size_t col = 0; col < 3; col++)
            {
                if (shape.mask[row][col])
                {
                    data[index(x + col, y + row)] = c;
                }
            }
        }
    }

    void clear() { memset(data.data(), 0, data.size()); }

    void print()
    {
        printf("Grid %zux%zu\n", W, H);
        printf("Shape counts: ");
        for (size_t i = 0; i < shape_counts.size(); i++)
        {
            printf("%zu ", shape_counts[i]);
        }
        printf("\n");
        for (size_t r = 0; r < H; r++)
        {
            for (size_t c = 0; c < W; c++)
            {
                uint8_t val = data[index(c, r)];
                printf("%c", val ? ('A' + val - 1) : '.');
            }
            printf("\n");
        }
    }

    void attempToSolve(vector<Shape> &shapes)
    {
        // Try to fit all shapes into the grid according to shape_counts
        vector<size_t> placed_counts(shapes.size(), 0);

        for (size_t s = 0; s < shapes.size(); s++)
        {
            Shape &shape = shapes[s];
            size_t count = shape_counts[s];

            // Try all rotations/flips
            for (size_t rot = 0; rot < 4; rot++)
            {
                for (size_t flip = 0; flip < 2; flip++)
                {
                    // Try to place 'count' number of this shape
                    for (size_t placed = 0; placed < count; placed++)
                    {
                        bool placed_this_shape = false;
                        for (size_t y = 0; y <= H - 3; y++)
                        {
                            for (size_t x = 0; x <= W - 3; x++)
                            {
                                if (canPlaceShape(shape, x, y))
                                {
                                    placeShape(shape, x, y, (uint8_t)(s + 1));
                                    placed_this_shape = true;
                                    placed_counts[s]++;
                                    goto next_shape_placement; // Break out of nested loops
                                }
                            }
                        }
                    next_shape_placement:
                        if (!placed_this_shape)
                        {
                            // Could not place this shape in this orientation
                            break;
                        }
                    }
                    shape.flip();
                }
                shape.rotateCCW();
            }
        }

        // Check if all shapes were placed successfully
        bool success = true;
        for (size_t s = 0; s < shapes.size(); s++)
        {
            if (placed_counts[s] != shape_counts[s])
            {
                printf("Failed to place shape %zu: placed %zu, needed %zu\n", 
                       s, placed_counts[s], shape_counts[s]);
                success = false;
            }
        }
        
        if (!success)
        {
            printf("Solution incomplete!\n");
        }
    }
};

// bool canFitAllShapes(Grid grid, vector<Shape> &shapes)
// {

// }

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<Grid> grids;

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

    // Next we have a series of WxH: a b c d e f where each letter is the count of that shape
    while (getline(in_file, line))
    {
        auto sep_pos = line.find(':');
        if (sep_pos == string::npos)
            break;

        string size_str  = line.substr(0, sep_pos);
        size_t space_pos = size_str.find('x');
        assert(space_pos != string::npos);
        size_t W = stoi(size_str.substr(0, space_pos));
        size_t H = stoi(size_str.substr(space_pos + 1));
        printf("Board size: %zux%zu\n", W, H);

        // Trim the line to just the counts
        string counts_str = line.substr(sep_pos + 2); // inc space
        vector<size_t> shape_counts;

        do
        {
            auto pos = counts_str.find_first_of(" ");
            if (pos == string::npos)
            {
                shape_counts.push_back(stoi(counts_str));
                break;
            }
            string count_str = counts_str.substr(0, pos);
            shape_counts.push_back(stoi(count_str));
            counts_str = counts_str.substr(pos + 1);
        } while (true);

        grids.emplace_back(W, H, shape_counts);
    }

    in_file.close();

    // Print all shapes and their rotations/flips
    for (size_t s = 0; s < shapes.size(); s++)
    {
        Shape &shape = shapes[s];
        printf("Shape %zu:\n", s);
        // for (size_t rot = 0; rot < 4; rot++)
        // {
        //     for (size_t flip = 0; flip < 2; flip++)
        //     {
        shape.print();
        printf("\n");
        shape.flip();
        //     }
        //     shape.rotateCCW();
        // }

        printf("--------\n");
    }

    // Print all grids
    for (size_t g = 0; g < grids.size(); g++)
    {
        Grid &grid = grids[g];
        grid.attempToSolve(shapes);
        grid.print();
        printf("--------\n");
    }

    return 0;
}
