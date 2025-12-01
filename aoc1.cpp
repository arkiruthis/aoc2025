#include <cstdio>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream in_file;
    vector<string> combinations;
    int idx = 50;
    size_t zero_count = 0;

    in_file.open("assets/combinations.txt");
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
            combinations.push_back(line);
        }
    }
    in_file.close();

    // combinations.resize(20);

    for (auto &str : combinations)
    {
        int inc = (str[0] == 'L') ? -1 : 1;
        int count = stoi(str.substr(1));

        do
        {
            idx = (idx + inc);
            if (idx < 0)
            {
                idx = 99;
            }
            idx %= 100;

            if (idx == 0)
            {
                zero_count++;
            }
        } while (--count > 0);
    }

    printf("Zero Count: %d\n", zero_count);

    return 0;
}
