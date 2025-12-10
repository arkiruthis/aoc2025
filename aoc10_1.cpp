#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

using ButtonSequences = vector<uint16_t>;

void printSequenceBits(uint16_t bits, bool newLine = false)
{
    // print in binary but LSB on left
    for (int i = 0; i < 16; i++)
    {
        if (bits & (1ULL << (i)))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }

    if (newLine)
        printf("\n");
}

class Machine
{
public:
    uint16_t indicatorBits        = 0;
    uint16_t desiredIndicatorBits = 0;
    ButtonSequences buttonSequences;
    bool resolved               = false;
    uint16_t minButtonsRequired = 0;

    void indicatorStatus()
    {
        if (indicatorBits == desiredIndicatorBits)
        {
            printf("Success! We have our desired sequence.\n");
        }
        else
        {
            printf("Oh dear... ;___;\n");
        }
    }

    void rotateResolve()
    {
        // From the examples, it looks like we can solve by starting at the right index of the sequences

        const auto maxRotations = buttonSequences.size();
        auto tmpButtons         = buttonSequences;
        minButtonsRequired      = maxRotations; // Assuming theory about sequential buttons is correct
        resolved                = false;
        // printf("Starting with initial sequence...\n");

        for (int i = 0; i < maxRotations; ++i)
        {
            for (size_t skip = 0; skip < 256; ++skip)
            {
                auto attempts = 0;
                indicatorBits = 0;
                for (const auto &bitSeq : tmpButtons)
                {
                    if ((1 << i) & skip)
                    {
                        continue;
                    }

                    pushButtonSequence(bitSeq);
                    // printSequenceBits(indicatorBits, true);
                    ++attempts;
                    if (indicatorBits == desiredIndicatorBits)
                    {
                        if (attempts < minButtonsRequired)
                        {
                            minButtonsRequired = attempts;
                            resolved           = true;

                            printf("Success after %d attempts! Best so far.\n", attempts);
                        }
                        break;
                    }
                }
            }
            rotate(begin(tmpButtons), begin(tmpButtons) + 1, end(tmpButtons));
        }

        if (resolved)
        {
            printf("===> Solution found with minimum %hu button presses.\n", minButtonsRequired);
        }
    }

    void pushButtonSequence(uint16_t seqBits) { indicatorBits ^= seqBits; }
};

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<Machine> machines;
    size_t minButtonsSum = 0;

    in_file.open("assets/aoc10.txt");
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
            machines.push_back(Machine());
            auto &machine = machines.back();

            // First lets grab our desired indicator sequene
            auto indicators = line.substr(0, line.find(' '));
            assert(indicators[0] = '[' && indicators.back() == ']');
            indicators = indicators.substr(1, indicators.size() - 2);

            uint16_t indicatorBits = 0;
            for (size_t i = 0; i < indicators.size(); i++)
            {
                if (indicators[i] == '#')
                {
                    indicatorBits |= (1ULL << i);
                }
            }

            machine.desiredIndicatorBits = indicatorBits;

            // Now grab our button sequences, which start and end with ()
            size_t seq_start = 0, seq_end = 0;
            while (seq_start != string::npos && seq_end != string::npos)
            {
                seq_start = line.find_first_of('(');
                seq_end   = line.find_first_of(')');
                if (seq_start != string::npos && seq_end != string::npos && seq_end > seq_start)
                {
                    // extract sequence from original line and erase from line
                    string seq_str = line.substr(seq_start + 1, seq_end - seq_start - 1);
                    line.erase(seq_start, seq_end - seq_start + 1);
                    uint16_t seqBits = 0;
                    for (char c : seq_str)
                    {
                        if (c >= '0' && c <= '9')
                        {
                            seqBits |= (1ULL << (c - '0'));
                        }
                    }

                    machine.buttonSequences.push_back(seqBits);
                }
            }
        }
    }
    in_file.close();

    size_t failRate = 0;

    // Let's experiment
    for (auto i = 0; i < machines.size(); ++i)
    {
        machines[i].rotateResolve();
        if (machines[i].resolved)
        {
            minButtonsSum += machines[i].minButtonsRequired;
        }
        else
        {
            printf("!!! We didn't find a solution by rotating ;___;\n");
            failRate++;
        }
    }

    printf("---------------------------------------\n");
    printf("Total Min Buttons Required: %zu\n", minButtonsSum);
    printf("---------------------------------------\n");
    printf("Total fail rate %zu\n", failRate);

    return 0;
}
