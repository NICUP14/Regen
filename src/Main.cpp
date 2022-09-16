#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "Parser.h"

int main()
{
	std::string expression;
	std::stack<RegenParser::contextType> ctxStack;

	std::vector<const char *> tokenTypeToStr{
		"UNDEFINED",
		"GROUP_BEGIN",
		"GROUP_END",
		"GROUP_SEP",
		"CHCLASS_WORD",
		"CHCLASS_SPACE",
		"CHCLASS_DIGIT",
		"CHCLASS_RANGE",
		"CHCLASS_BEGIN",
		"CHCLASS_END"};

	std::vector<const char *> contextTypeToStr{
		"CHCLASS",
		"GROUP"};

	std::getline(std::cin, expression);
	auto iter = expression.begin();

	while (iter != expression.end())
	{
		std::string ctxStr = (ctxStack.empty() ? "NONE" : contextTypeToStr.at((int)ctxStack.top()));
		std::string typeStr = tokenTypeToStr.at((int)RegenParser::scanToken(iter, ctxStack));

		std::cout << "CTX: " << ctxStr << ", ";
		std::cout << (ctxStr == "NONE" ? "   " : ""); //? Temporary width adjustment
		std::cout << (ctxStr == "GROUP" ? "  " : ""); //? Temporary width adjustment
		std::cout << "TYPE: " << typeStr << '\n';

		iter += typeStr == "UNDEFINED";
	}
	return 0;
}