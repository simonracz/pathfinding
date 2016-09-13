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
	double mRatio = 0.1;
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
		mSize = std::max(2, mSize);
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
	int getSize() const {return mSize;}
	bool isHexagonal() const {return mHexagonal;}
	bool isSquare() const {return mSquare;}
	bool isHelp() const {return mHelp;}
	double getRatio() const {return mRatio;}
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
	int mSize = 5;
	double mRatio = 0.1;
public:
	void setRatio(double ratio) {
		if (ratio < 0 || ratio > 1.0) {
			return;
		}
		mRatio = ratio;
	}
	void setSize(int size) {
		if (size <= 2) {
			return;
		}
		mSize = size;
	}
	MapGenerator(int size, double ratio) {
		setSize(size);
		setRatio(ratio);
	}
	virtual std::vector<int> generate() = 0;
};

class HexagonalMapGenerator : public MapGenerator {	
public:	
	HexagonalMapGenerator(int size, double ratio): MapGenerator(size, ratio) {}
	virtual std::vector<int> generate() override;
};

class SquareMapGenerator : public MapGenerator {
public:
	SquareMapGenerator(int size, double ratio): MapGenerator(size, ratio) {}
	virtual std::vector<int> generate() override;
};

std::unique_ptr<MapGenerator> generatorForCMDOptions(const CMDOptions& opts) {
	if (opts.isHexagonal()) {
		return std::unique_ptr<MapGenerator>(
				new HexagonalMapGenerator(opts.getSize(), opts.getRatio())
			   );
	}
	if (opts.isSquare()) {
		return std::unique_ptr<MapGenerator>(
				new SquareMapGenerator(opts.getSize(), opts.getRatio())
			   );	
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

std::vector<int> HexagonalMapGenerator::generate()
{
	// 1.
	// size = 3n^2 + 3n + ?
	// 2.
	// gen. array of (size - 2) elements using ratio
	// 3.
	// use random shuffle on array
	// 4.
	// num of coordinates where target can be
	// 5.
	// sort target coordinates
	// 6.
	// randomly pick one
	// 7.
	// assemble return vector:
	// a) rand elements from array
	// b) fixed source element (which is a free block)
	// c) picked target element (which is a * block)

	return std::vector<int>();
}

std::vector<int> SquareMapGenerator::generate()
{
	return std::vector<int>();
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