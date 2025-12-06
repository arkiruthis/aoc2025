#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <numeric>

using namespace std;

typedef struct Op
{
    int64_t val[4];
    char op;

    int64_t Process() const
    {
        switch (op)
        {
        case '+':
            return accumulate(val, val + 4, 0LL, [](int64_t a, int64_t b)
                              { return a + b; });
        case '-':
            return accumulate(val, val + 4, 0LL, [](int64_t a, int64_t b)
                              { return a - b; });
        case '*':
            return accumulate(val, val + 4, 1LL, [](int64_t a, int64_t b)
                              { return a * b; });
        default:
            return 0;
        }
    }
} Op;

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<Op> problems;
    int idx = 50;
    int64_t problem_count = 0;

    in_file.open("assets/aoc6.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    string line;
    size_t pass = 0;
    const size_t op_pass = 4;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            // Optimize this. This is not great.

            if (pass < op_pass) // Numbers
            {
                istringstream iss(line);
                int64_t tmp;
                size_t idx = 0;
                while (iss >> tmp)
                {
                    switch (pass)
                    {
                    case 0: // populate
                        problems.push_back({{tmp, 0, 0, 0}, 0});
                        break;
                    default:
                        assert(idx < problems.size());
                        problems[idx].val[pass] = tmp;
                        break;
                    };
                    ++idx;
                }
            }
            else if (pass == op_pass)
            {
                stringstream iss(line);
                string tmp;
                size_t idx = 0;
                while (iss >> tmp)
                {
                    assert(idx < problems.size());
                    problems[idx].op = tmp[0];
                    ++idx;
                }
            }
        }
        ++pass;
    }
    in_file.close();

    for (const auto &p : problems)
    {
        auto result = p.Process();
        problem_count += result;
        printf("Problem = %d %c %d %c %d %c %d = %d\n", p.val[0], p.op, p.val[1], p.op, p.val[2], p.op, p.val[3], result);
    }

    printf("Problem Count: %lld\n", problem_count);

    return 0;
}
