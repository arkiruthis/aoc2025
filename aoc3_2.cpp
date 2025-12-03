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

        string subStr = str.substr(minIdxLeftSweep);
        char minFound = '1';
        while (subStr.length() > 12 )
        {
            printf("Looking for %c in %s\n", minFound, subStr.c_str());
            vector<int> foundIndices;
            for (int i = 0; i < subStr.length(); ++i)
            {
                if (subStr[i] == minFound)
                {
                    foundIndices.push_back(i);
                }
            }

            printf("found %d instances of %c\n", foundIndices.size(), minFound);

            while (!foundIndices.empty() && subStr.length() > 12) 
            {
                subStr.erase(foundIndices.back(), 1);
                foundIndices.pop_back();
            }

            minFound++;
            if (minFound > '9') break;
            printf("Current subStr [ %s ]\n", subStr.c_str());
        }
        
        tally += stoull(subStr);
    }

    printf("Total Joltage: %llu\n", tally);

    return 0;
}
