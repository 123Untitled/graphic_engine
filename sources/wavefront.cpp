#include "wavefront.hpp"

constexpr const char* const debug_state[] {
	"start",
	"error",
	"end",
	"comment",
	"return",
	"escape",
	"keyword",
	"after keyword",
	"wait number",
};

static void debug(const char* msg, const char* str, const std::size_t size) {
	std::cout << "\x1b[33m" << msg << "\x1b[0m: ";
	for (std::size_t i = 0; i < size; ++i) {
		char c = str[i];

		if (std::iscntrl(c)) {
			std::cout << "\x1b[33m";
			if (c == '\n')
				std::cout << "\\n";
			else if (c == '\t')
				std::cout << "\\t";
			else if (c == '\r')
				std::cout << "\\r";
			else if (c == '\v')
				std::cout << "\\v";
			else if (c == '\f')
				std::cout << "\\f";
			else if (c == '\b')
				std::cout << "\\b";
			else if (c == '\a')
				std::cout << "\\a";
			else if (c == '\0')
				std::cout << "\\0";
			else {
				std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)c << std::dec;
			}
			std::cout << "\x1b[0m";
		}
		else if (c == ' ')
			std::cout << "\x1b[31m·\x1b[0m";
		else
			std::cout << c;
	}
	std::cout << std::endl;
}


