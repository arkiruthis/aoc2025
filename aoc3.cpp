#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <utility>

using namespace std;

void prettyPrintBank(std::string_view bank, pair<size_t, size_t> idx)
{
    printf("%c%c ==> ", bank[idx.first], bank[idx.second]);
    for (int i = 0; i < bank.length(); ++i)
    {
        const bool wrapping = (i == idx.first || i == idx.second);
        if (wrapping) printf(" [");
        printf("%c", bank[i]);
        if (wrapping) printf("] ");
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

        for (int i = str.length() - 1; i >= 0; --i)
        {
            // Due to ascii layout, they do run in sequence
            if (str[i] >= minCharRS)
            {
                minCharRS = str[i];
                minIdxRightSweep = i;
            }
        }

        // We now need to sweep in the other direction, but skip if we encounter the excluded value
        for (int i = 0; i < str.length(); ++i)
        {
            // Due to ascii layout, they do run in sequence
            if (i != minIdxRightSweep && str[i] > minCharLS)
            {
                minCharLS = str[i];
                minIdxLeftSweep = i;
            }
        }

        // Now, we just need to see if we crossed paths
        if (minIdxLeftSweep > minIdxRightSweep)
        {
            swap(minIdxLeftSweep, minIdxRightSweep);
            swap(minCharLS, minCharRS);
        }

        if (minCharLS < minCharRS && minCharRS < (str.length() - 2))
        {
            minCharLS = str[minIdxRightSweep + 1];
            for (int i = minIdxRightSweep + 1; i < str.length(); ++i)
            {
                if (str[i] >= minCharLS) {
                    minCharLS = str[i];
                    minIdxLeftSweep = i;
                }
            }

            swap(minIdxLeftSweep, minIdxRightSweep);
            swap(minCharLS, minCharRS);
        }

        // We have another curiosity, if lC < rC we need another sweep

        const auto strVal = string(1, minCharLS).append(string(1, minCharRS));
        
        prettyPrintBank(str, {minIdxLeftSweep, minIdxRightSweep});

        int combination = stoi(strVal);
        printf("-- %d\n", combination);
        tally += combination;
    }

    printf("Total Joltage: %d\n", tally);

    return 0;
}
