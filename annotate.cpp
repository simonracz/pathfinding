#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <exception>
#include <memory>
#include <random>
#include <chrono>
#include <numeric>
#include <iterator>
#include <iomanip>

#include "cxxopts.hpp"

class CMDOptions {
private:
    bool mHelp = false;
    std::string mFilePath;
    cxxopts::Options options;
    bool parse(int argc, char* argv[]) {
        try {
            options.parse(argc, argv);
            return true;
        } catch(const cxxopts::OptionException& e) {
            std::cerr << "error parsing options: " << e.what() << std::endl;
            return false;
        }
    }
public:
    CMDOptions() : options("annotate", "Pathfinding annotator for maps") {
        options.add_options()
          ("f,file", "Appends results to file", cxxopts::value<std::string>(mFilePath)
                ->default_value("ann.txt"))
          ("h,help", "Prints help", cxxopts::value<bool>(mHelp))
        ;
    }
    bool parseCMDLine(int argc, char* argv[]) {
        if (!parse(argc, argv)) {
            return false;
        }
        if (mFilePath.length() == 0) {
            return false;
        }
        return true;
    }
    std::string getFilePath() const {return mFilePath;}
    bool isHelp() const {return mHelp;}
    std::string helpMessage() const {return options.help({""});}
    void print() const {
        std::cout << "options = {" 
                  << "\n  file: " << mFilePath
                  << ",\n  help: " << mHelp
                  << "\n}" << std::endl;
    }
};

template <typename Sequence>
void print(const Sequence& sequence)
{
    std::cout << "[";
    bool notFirst = false;
    for (const auto& s : sequence) {
        if (notFirst) {
            std::cout << ", ";
        }
        std::cout << s;
        notFirst = true;
    }
    std::cout << "]\n";
}

std::vector<int> readInput() {
    std::vector<int> v;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream ss{line};
    char c;
    ss >> c;
    if (c != '[') {
        throw cxxopts::OptionException("Input error. Should start with '['.");
    }
    int i;
    c = ',';
    while (c != ']') {
        if (c != ',') {
            throw cxxopts::OptionException("Input error. Should separate elements by ','.");
        }
        ss >> i >> c;       
        v.push_back(i);
    }
    return v;
}

std::pair<int, int> prettyPrintMap(std::vector<int> map) {
    std::cout << "\nCoordinate system: (x, y)\n\n"
              << "   (0,-1) (1,-1)\n"
              << " (-1,0) (0,0) (1, 0)\n"
              << "   (-1,1)  (0,1)\n";
    std::cout << "\nAnnotate the best first path from S to T.\n\n";
    int mSize = (-3 + std::sqrt(9 - 4 * 3 * (1 - map.size()))) / 6;
    int counter = 0;
    int Tx = 0;
    int Ty = 0;
    for (int y = -mSize; y <= 0; ++y) {
        for (int x = -mSize; x <= mSize; ++x) {
            int z = -x - y;
            if (z > mSize || z < -mSize) {
                // We are off the grid
                std::cout << " ";
                continue;
            }
            if (x == 0 && y == 0) {
                std::cout << " S";
                ++counter;
                continue;
            }
            if (map[counter] == -1) {
                Tx = x;
                Ty = y;
                std::cout << " T";
                ++counter;
                continue;
            }
            std::cout << " " << map[counter];
            ++counter;
        }
        std::cout << "\n";
    }
    for (int y = 1; y <= mSize; ++y) {
        for (int i = 0; i < y; ++i) {
            std::cout << " ";
        }
        for (int x = -mSize; x <= mSize; ++x) {
            int z = -x - y;
            if (z > mSize || z < -mSize) {
                // We are off the grid
                continue;
            }
            if (map[counter] == -1) {
                Tx = x;
                Ty = y;
                std::cout << " T";
                ++counter;
                continue;
            }
            std::cout << " " << map[counter];
            ++counter;
        }
        std::cout << "\n";
    }
    return std::make_pair(Tx, Ty);
}

int main(int argc, char* argv[]) {
    CMDOptions opts;
    if (!opts.parseCMDLine(argc, argv)) {   
        return 1;
    }
    if (opts.isHelp())
    {
        std::cout << opts.helpMessage() << std::endl;
        return 0;
    }
    // opts.print();
    auto v = readInput();
    auto T = prettyPrintMap(v);
    auto P = std::pair<int, int>{T};
    std::cout << "\nT = (" << T.first << ", " << T.second << ")\n";
    std::cout << "Please type in the path coordinates as 'x, y' (default T): ";
    std::string line;
    std::getline(std::cin, line);
    std::istringstream ss{line};
    char c;
    if (line.length() == 0) {
        std::cout << "Line length = 0, P = (" << P.first << ", " << P.second << ")\n";
        return 0;
    }
    ss >> P.first >> c;
    if (c != ',') {
        std::cout << "\nBad input. Should separate elements with ','.\n";   
        return 1;
    }
    ss >> P.second;
    std::cout << "P = (" << P.first << ", " << P.second << ")\n";
    return 0;   
}







