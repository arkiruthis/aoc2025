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
    string swizzle; // Just somewhere to store per-Op junk in the initial read
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
                              { 
                                // Horrible, HORRIBLE hack to get this working... ;___;
                                if (a == 0) return b;
                                if (b == 0) return a;
                                return a * b; });
        default:
            return 0;
        }
    }
} Op;

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<Op> problems;
    int64_t problem_count = 0;

    in_file.open("assets/aoc6.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    string line;
    vector<string> cache;
    while (getline(in_file, line))
    {
        if (!line.empty())
        {
            cache.push_back(line);
        }
    }
    in_file.close();

    bool commitCurrentOp = false;
    size_t idx = 0;
    Op currentOp{};
    for (int i = 0; i <= cache[0].length(); ++i)
    {
        string vertical;

        if (i == cache[0].length()) // The last column won't be spaces ;____;
        {
            commitCurrentOp = true;
        }

        if (!commitCurrentOp)
        {
            for (int j = 0; j < cache.size(); ++j)
            {
                vertical.push_back(cache[j][i]);
            }

            bool ws = std::all_of(vertical.begin(), vertical.end(),
                                  [](unsigned char c)
                                  { return std::isspace(c); });

            if (ws) // End of block
            {
                idx = 0;
                commitCurrentOp = true;
            }
            else
            {
                // We have an operation, and the start of a new section
                if (vertical.back() != ' ')
                {
                    currentOp.op = vertical.back();
                }
                
                string val = vertical.substr(0, 4);
                try {
                    currentOp.val[idx] = stoi(val);
                }
                catch(exception &e) {
                    currentOp.val[idx] = 0;
                }
                ++idx;
            }
        }

        if (commitCurrentOp)
        {
            commitCurrentOp = false;
            problems.push_back(currentOp);
            currentOp = {};
        }
    }

    for (const auto &p : problems)
    {
        auto result = p.Process();
        problem_count += result;
        printf("Problem = %lld %c %lld %c %lld %c %lld = %lld\n", p.val[0], p.op, p.val[1], p.op, p.val[2], p.op, p.val[3], result);
    }

    printf("Problem Count: %lld\n", problem_count);

    return 0;
}
