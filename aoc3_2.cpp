#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <utility>

using namespace std;

void prettyPrintBank(std::string_view bank, pair<size_t, size_t> idx)
{
    printf("==> ");
    for (int i = 0; i < bank.length(); ++i)
    {
        if (i == idx.first)
            printf(" [");
        printf("%c", bank[i]);
        if (i == idx.second)
            printf("] ");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<string> banks;
    int idx = 50;
    size_t zero_count = 0;
    uint64_t tally = 0;

    in_file.open("assets/aoc3.txt");
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
            banks.push_back(line);
        }
    }
    in_file.close();

    for (auto &str : banks)
    {
        size_t minIdxLeftSweep = 0;
        size_t minIdxRightSweep = str.length() - 1;
        char minCharLS = '1';
        char minCharRS = '1';
        char maxInvalidChar = numeric_limits<char>::max();

        // What is the biggest number we can find that will allow us
        // to have the largest starting number and be greater than 12 cells
        for (int i = str.length() - 12; i >= 0; --i)
        {
            // Due to ascii layout, they do run in sequence
            if (str[i] >= minCharLS)
            {
                minCharLS = str[i];
                minIdxLeftSweep = i;
            }
        }

        prettyPrintBank(str, {minIdxLeftSweep, minIdxRightSweep});
        // We now have a guaranteed substring that we need to whittle down to 12
        // ensuring the largest possible are on the left.

        auto whittleDownTo12 = [](const string &str)
        {
            constexpr int BANK_SIZE = 12;
            int numRemovable = str.length() - BANK_SIZE;
            string result;
            result.reserve(str.length());

            // Go through the original string and if the back element
            // of our result is less than the current character then
            // pop it off! We push the next character on regardless. 
            for (char c : str)
            {
                while (!result.empty() && (numRemovable > 0) && (result.back() < c))
                {
                    result.pop_back();
                    --numRemovable;
                }
                result.push_back(c);
            }

            // We might still be over the limit, so we can trim off the excess.
            if ((int)result.size() > BANK_SIZE)
            {
                result.resize(BANK_SIZE);
            }
            return result;
        };

        string subStr = str.substr(minIdxLeftSweep);
        subStr = whittleDownTo12(subStr);
        printf("Max 12 digit: %s\n", subStr.c_str());

        tally += stoull(subStr);
    }

    printf("Total Joltage: %llu\n", tally);

    return 0;
}
