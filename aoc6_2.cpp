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

    void Populate()
    {
        // I guess we iterate down and shift stuff base10 if it's not a space? :/
        // The data seems to be only + and * which is commutative(spl??) so order does't matter, just value?
        // printf("%d\n", swizzle.length());
        assert(swizzle.length() == 16);
        for (int i = 0; i < 4; ++i) { // across 4 words
            for (int j = 0; j < 4; ++j) { // Starting at index 0 of word
                int n = 0;
                char c = swizzle[(j * 4) + i];
                if (c != ' ')
                    n = c - '0'; // This will pull 0-9 down from ASCII position to origin
                
                val[i] += n * pow(10, 3 - j); // Big endian when reading >___<
            }
        }
    }

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
                                return a * b; 
                            });
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
            stringstream ss(line);
            string tmp;
            size_t idx = 0;

            if (pass < op_pass) // Numbers
            {
                const auto pad = [](string &s)
                {
                    return s + string(4 - s.size(), ' ');
                };

                while (ss >> tmp)
                {
                    switch (pass)
                    {
                    case 0: // populate
                        problems.push_back({pad(tmp), {}, 0});
                        break;
                    default:
                        assert(idx < problems.size());
                        problems[idx].swizzle += pad(tmp);
                        break;
                    };
                    ++idx;
                }
            }
            else if (pass == op_pass)
            {
                
                while (ss >> tmp)
                {
                    printf("%s\n", problems[idx].swizzle.c_str());
                    assert(idx < problems.size());
                    problems[idx].op = tmp[0];
                    ++idx;
                }

            }
        }
        ++pass;
    }
    in_file.close();

    // Phew, now Populate our vals from the string cache
    for_each(begin(problems), end(problems), mem_fn(&Op::Populate));

    for (const auto &p : problems)
    {
        auto result = p.Process();
        problem_count += result;
        printf("Problem = %lld %c %lld %c %lld %c %lld = %lld\n", p.val[0], p.op, p.val[1], p.op, p.val[2], p.op, p.val[3], result);
    }

    printf("Problem Count: %lld\n", problem_count);

    return 0;
}
