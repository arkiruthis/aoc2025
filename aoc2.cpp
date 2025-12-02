#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <optional>

using namespace std;

using PHATBOY = uint64_t;

#define PART_TWO

#ifndef PART_TWO
optional<PHATBOY> isValidId(PHATBOY id)
{
    optional<PHATBOY> result;
    string idStr = to_string(id);
    if (idStr.length() & 1 || idStr.length() < 2) // Odd or too short
    {
        return nullopt;
    }

    int halfLen = idStr.length() >> 1;

    for (int i = 0; i < halfLen; ++i)
    {
        if (idStr[i] != idStr[i + halfLen])
        {
            return nullopt;
        }
    }

    return id;
}
#else
optional<PHATBOY> isValidId(PHATBOY id)
{
    optional<PHATBOY> result;
    string idStr = to_string(id);
    if (idStr.length() < 2) // Too short
    {
        return nullopt;
    }

    // What is the biggest number that cleanly divides the length?
    int len = idStr.length();
    int divisor = len / 2; // Gotta start somewhere...

    while (divisor > 0)
    {
        if (len % divisor == 0)
        {
            bool repeating = true;
            for (int i = divisor; i < len && repeating; ++i)
            {
                // Basically clipping each comparison to the first section
                if (idStr[i] != idStr[i % divisor])
                {
                    repeating = false;
                }
            }
            if (repeating)
            {
                return id;
            }
        }
        divisor--;
    }

    return nullopt;
}
#endif

vector<PHATBOY> vecFromRange(string rangeStr)
{
    vector<PHATBOY> result;
    size_t dashPos = rangeStr.find('-');
    if (dashPos != string::npos)
    {
        PHATBOY start = stoull(rangeStr.substr(0, dashPos));
        PHATBOY end = stoull(rangeStr.substr(dashPos + 1));
        for (PHATBOY i = start; i <= end; ++i)
        {
            result.push_back(i);
        }
    }
    else
    {
        result.push_back(stoull(rangeStr));
    }
    return result;
}

int main(int argc, char *argv[])
{
    ifstream in_file;
    PHATBOY tally = 0;

    in_file.open("assets/aoc2.txt");
    if (!in_file.good())
    {
        printf("Unable to read input file.");
        return 1;
    }

    // Read the single line from the file
    string line;
    getline(in_file, line);
    in_file.close();

    // Tokenize based on comma separation
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (getline(ss, token, ','))
    {
        tokens.push_back(token);
    }

    // Print the tokens (for verification)
    printf("Found %zu tokens:\n", tokens.size());
    for (const auto &t : tokens)
    {
        printf("--------\n");
        printf("Token: %s\n", t.c_str());

        vector<PHATBOY> values = vecFromRange(t);
        for (const auto &val : values)
        {
            if (const auto result = isValidId(val))
            {
                tally += *result;
                printf("%llu,", *result);
            }
        }
        printf("\n");
        printf("Tally: %llu\n", tally);
    }

    return 0;
}
