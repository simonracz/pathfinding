#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <memory>
#include <random>
#include "cxxopts.hpp"

class CMDOptions {
private:
	int n = 5;
	bool hexagonal = false;
	bool square = false;
	bool help = false;
	// These are optional
	long long seed = 0LL;
	double ratio = 0.1;
	// end of optional
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
	void ensureConsistency() {
		n = std::max(1, n);
		if (square && hexagonal) {
			square = false;
		}
		if (!square && !hexagonal) {
			hexagonal = true;
		}
	}
public:
	CMDOptions() : options("generate", " - Generator of simple maps") {
		options.add_options()
	  	  ("n,size", "size of the map", cxxopts::value<int>(n))
	  	  ("x,hexagonal", "generate hexagonal maps", cxxopts::value<bool>(hexagonal))
	  	  ("q,square", "generate square maps", cxxopts::value<bool>(square))
	  	  ("s,seed", "seed for the random generator", cxxopts::value<long long>(seed))
	  	  ("r,ratio", "wall block to free block ratio", cxxopts::value<double>(ratio))
	  	  ("h,help", "Prints help", cxxopts::value<bool>(help))
		;
	}
	bool parseCMDLine(int argc, char* argv[]) {
		if (!parse(argc, argv)) {
			return false;
		}
		ensureConsistency();
		return true;
	}
	int size() const {return n;}
	bool isHexagonal() const {return hexagonal;}
	bool isSquare() const {return square;}
	bool isHelp() const {return help;}
	bool hasSeed() const {return (options.count("seed") > 0);}
	bool hasRatio() const {return (options.count("ratio") > 0);}
	long long randomSeed() const {return seed;}
	double wallRatio() const {return ratio;}
	std::string helpMessage() const {return options.help({""});}
	void print() const {
		std::cout << "options = {" 
				  << "\n  size: " << n
				  << ",\n  hexagonal: " << hexagonal
				  << ",\n  square: " << square
				  << ",\n  help: " << help
				  << "\n}" << std::endl;
	}
};

class MapGenerator {
protected:
	int size;
public:
	MapGenerator(int size): size(size) {}
	virtual std::vector<int> generate() = 0;
	virtual std::string generateAsString() = 0;
};

class HexagonalMapGenerator : public MapGenerator {
public:
	HexagonalMapGenerator(int size): MapGenerator(size) {}
	virtual std::vector<int> generate() override {
		return std::vector<int>();
	}
	virtual std::string generateAsString() override {
		return "";
	}
};

class SquareMapGenerator : public MapGenerator {
public:
	SquareMapGenerator(int size): MapGenerator(size) {}
	virtual std::vector<int> generate() override {
		return std::vector<int>();
	}
	virtual std::string generateAsString() override {
		return "";
	}
};

std::unique_ptr<MapGenerator> generatorForOptions(const CMDOptions& opts) {
	if (opts.isHexagonal()) {
		return std::unique_ptr<MapGenerator>(new HexagonalMapGenerator(opts.size()));
	}
	if (opts.isSquare()) {
		return std::unique_ptr<MapGenerator>(new SquareMapGenerator(opts.size()));	
	}
	throw cxxopts::OptionException("CMDOptions are not consistent.");
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
	auto gen = generatorForOptions(opts);
	gen->generate();
}