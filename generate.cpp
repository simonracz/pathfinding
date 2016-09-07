#include <iostream>
#include <algorithm>
#include "cxxopts.hpp"

class cmd_options {
private:
	int n = 5;
	bool hexagonal = false;
	bool square = false;
	bool help = false;
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
	void ensure_consistency() {
		n = std::max(1, n);
		if (square && hexagonal) {
			square = false;
		}
		if (!square && !hexagonal) {
			hexagonal = true;
		}
	}
public:
	cmd_options() : options("generate", " - Generator of simple maps") {
		options.add_options()
	  	  ("n,size", "size of the map", cxxopts::value<int>(n))
	  	  ("x,hexagonal", "generate hexagonal maps", cxxopts::value<bool>(hexagonal))
	  	  ("s,square", "generate square maps", cxxopts::value<bool>(square))
	  	  ("h,help", "Prints help", cxxopts::value<bool>(help))
		;
	}
	bool parse_cmd_line(int argc, char* argv[]) {
		if (!parse(argc, argv)) {
			return false;
		}
		ensure_consistency();
		return true;
	}
	int size() {return n;}
	bool isHexagonal() {return hexagonal;}
	bool isSquare() {return square;}
	bool isHelp() {return help;}
	std::string help_message() {return options.help({""});}
	void print() {
		std::cout << "options = {" 
				  << "\n  size: " << n
				  << ",\n  hexagonal: " << hexagonal
				  << ",\n  square: " << square
				  << ",\n  help: " << help
				  << "\n}" << std::endl;
	}
};

int main(int argc, char* argv[]) {
	cmd_options opts;
	bool ret = opts.parse_cmd_line(argc, argv);
	if (!ret) {
		std::cout << opts.help_message() << std::endl;
		return 1;
	}
	opts.print();
	if (opts.isHelp())
	{
		std::cout << opts.help_message() << std::endl;
		return 0;
	}
	
}