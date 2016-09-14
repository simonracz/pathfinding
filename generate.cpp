#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <memory>
#include <random>
#include <chrono>
#include <numeric>
#include <iterator>

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
		if (size <= 1) {
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

/*
	Example hexagonal map for size == 1
	    (0,-1) (1,-1)
	  (-1,0) (0,0) (1, 0)
	    (-1,1)  (0,1)
*/
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
	int mapSize = 3 * mSize * mSize + 3 * mSize + 1;
	// generate (size - 2) num of blocks using ratio
	// 2 blocks are reserved for the start and target position
	std::vector<int> blocks(mapSize - 2);
	for (int i = 0; i < mRatio * (mapSize - 2); ++i) {
		blocks[i] = 1;
	}
	// randomize the blocks
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 m(seed);
	std::shuffle(blocks.begin(), blocks.end(), m);
	// enumerate coordinates where target can be
	// 1/6 slice of a pizza
	// where (x, y) x>0, y>=0
	std::uniform_int_distribution<int> dist(1, (mSize * mSize + mSize) / 2);
	auto dice = std::bind(dist, m);
	int target = dice();
	// assemble return vector:
	// a) rand elements from array
	// b) fixed source element (which is a free block)
	// c) picked target element (which is a -1 block)
	std::vector<int> ret;
	ret.reserve(mapSize);
	int countToTarget = 0;
	for (int y = -mSize; y <= mSize; ++y) {
		for (int x = -mSize; x <= mSize; ++x) {
			int z = -x - y;
			std::cout << "x: " << x << " y: " << y << "\n";
			if (z > mSize || z < -mSize) {
				// We are off the grid
				continue;
			}
			if (x > 0 && y >= 0) {
				++countToTarget;
				if (countToTarget == target) {
					ret.push_back(-1);
					continue;
				}
			}
			if (x == 0 && y == 0) {
				ret.push_back(0);
				continue;
			}
			ret.push_back(blocks.back());
			blocks.pop_back();
		}
	}
	return ret;
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






