#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <memory>
#include <random>
#include "cxxopts.hpp"

class CMDOptions {
private:
	int mSize = 5;
	bool mHexagonal = false;
	bool mSquare = false;
	bool mHelp = false;
	// These are optional
	long mSeed = 0LL;
	double mRatio = 0.1;
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
		mSize = std::max(1, mSize);
		if (mSquare && mHexagonal) {
			mSquare = false;
		}
		if (!mSquare && !mHexagonal) {
			mHexagonal = true;
		}
	}
public:
	CMDOptions() : options("generate", " - Generator of simple maps") {
		options.add_options()
	  	  ("n,size", "size of the map (default: 5)", cxxopts::value<int>(mSize))
	  	  ("x,hexagonal", "generate hexagonal maps (default)", cxxopts::value<bool>(mHexagonal))
	  	  ("q,square", "generate square maps", cxxopts::value<bool>(mSquare))
	  	  ("s,seed", "seed for the random generator (optional)", cxxopts::value<long>(mSeed))
	  	  ("r,ratio", "wall block to free block ratio (default: 0.1)", cxxopts::value<double>(mRatio))
	  	  ("h,help", "Prints help", cxxopts::value<bool>(mHelp))
		;
	}
	bool parseCMDLine(int argc, char* argv[]) {
		if (!parse(argc, argv)) {
			return false;
		}
		ensureConsistency();
		return true;
	}
	int size() const {return mSize;}
	bool isHexagonal() const {return mHexagonal;}
	bool isSquare() const {return mSquare;}
	bool isHelp() const {return mHelp;}
	bool hasSeed() const {return (options.count("seed") > 0);}
	bool hasRatio() const {return (options.count("ratio") > 0);}
	long randomSeed() const {return mSeed;}
	double wallRatio() const {return mRatio;}
	std::string helpMessage() const {return options.help({""});}
	void print() const {
		std::cout << "options = {" 
				  << "\n  size: " << mSize
				  << ",\n  hexagonal: " << mHexagonal
				  << ",\n  square: " << mSquare
				  << ",\n  help: " << mHelp
				  << "\n}" << std::endl;
	}
};

class MapGenerator {
protected:
	int mSize;
public:
	MapGenerator(int size): mSize(size) {}
	virtual std::vector<int> generate() = 0;
};

class HexagonalMapGenerator : public MapGenerator {
public:
	HexagonalMapGenerator(int size): MapGenerator(size) {}
	virtual std::vector<int> generate() override {
		return std::vector<int>();
	}
};

class SquareMapGenerator : public MapGenerator {
public:
	SquareMapGenerator(int size): MapGenerator(size) {}
	virtual std::vector<int> generate() override {
		return std::vector<int>();
	}
};

std::unique_ptr<MapGenerator> generatorForCMDOptions(const CMDOptions& opts) {
	if (opts.isHexagonal()) {
		return std::unique_ptr<MapGenerator>(new HexagonalMapGenerator(opts.size()));
	}
	if (opts.isSquare()) {
		return std::unique_ptr<MapGenerator>(new SquareMapGenerator(opts.size()));	
	}
	throw cxxopts::OptionException("CMDOptions are not consistent.");
}

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
	auto gen = generatorForCMDOptions(opts);
	auto map = gen->generate();
	print(map);
}