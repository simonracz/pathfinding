#include <iostream>
#include <algorithm>
#include "cxxopts.hpp"

class options {
private:
	int n = 0;
	bool hexagonal = false;
	bool square = false;
	bool help = false;
	std::string help_msg = "";
public:
	options() {}
	bool parse_options(int argc, char* argv[]) {
		try {
			// Setting up cmd line options
			cxxopts::Options options(argv[0], " - Generator of simple maps");
			options.add_options()
		  	  ("n,size", "size of the map", cxxopts::value<int>()
		  		->default_value("5"))
		  	  ("x,hexagonal", "generate hexagonal maps", cxxopts::value<bool>())
		  	  ("s,square", "generate square maps", cxxopts::value<bool>())
		  	  ("h,help", "Prints help", cxxopts::value<bool>())
			;

			help_msg = options.help({""});

			// Parsing options
			options.parse(argc, argv);			

			help = options["help"].as<bool>();
			hexagonal = options["hexagonal"].as<bool>();
			square = options["square"].as<bool>();
			n = options["size"].as<int>();

			// Ensuring consistency
			n = std::max(1, n);
			if (square && hexagonal) {
				square = false;
			}
			if (!square && !hexagonal) {
				hexagonal = true;
			}
			return true;
		} catch (const cxxopts::OptionException& e) {
			std::cout << "error parsing options: " << e.what() << std::endl;
			return false;
		}
	}
	int size() {return n;}
	bool isHexagonal() {return hexagonal;}
	bool isSquare() {return square;}
	bool isHelp() {return help;}
	const std::string& help_message() {return help_msg;}
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
	options opts;
	bool ret = opts.parse_options(argc, argv);
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