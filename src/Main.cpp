//? C library imports
#include <stdio.h>
#include <string.h>

//? C++ library imports
#include <string>
#include <vector>
#include <bitset>
#include <memory>
#include <fmt/core.h>
#include "Parser.h"
#include "AST.h"

//! BUG LIST (ordered by importance)
//! The regen expression [[^abc]] throws an unexpected exception.

// TODO LIST (ordered by importance)
// TODO: Replace fmt::fprintf with fmt::print.
// TODO: Implement character class intersections.
// TODO: Require slashes to be escaped when the REGEN_REGEX_FLAG is true.
// TODO: Refactor the codebase

const bool USE_STDIN = true;
const bool USE_STDOUT = true;
const size_t EXPRESSION_SIZE = 50;
const size_t NODE_ID_SET_SIZE = 50;
const char *const TEST_FILE = "test/cases.txt";
const char *const OUT_FILE = "test/cases_output.txt";
const char *const ERR_FILE = "test/cases_error.txt";

//? Regen-related flag definition
const bool REGEN_REGEX_COMPLIANT_FLAG = false;

void printASTNode(FILE *outputFD, std::shared_ptr<RegenAST::ASTNode> nodeRef)
{

	fmt::fprintf(outputFD, "Node %d:\n", nodeRef->GetId());
	fmt::fprintf(outputFD, "\tType: %s;\n", NodeTypeToStr(nodeRef->GetDataRef().GetNodeType()));
	fmt::fprintf(outputFD, "\tLiteral: %s;\n", nodeRef->GetDataRef().GetLiteral());

	for (const auto &child : nodeRef->GetChildrenRef())
		fmt::fprintf(outputFD, "\tChild: %d;\n", child->GetId());
}

void traverseAST(FILE *outputFD, std::bitset<NODE_ID_SET_SIZE> &nodeIdSetRef, std::shared_ptr<RegenAST::ASTNode> node)
{
	nodeIdSetRef.set(node->GetId(), true);
	printASTNode(outputFD, node);

	for (const auto &child : node->GetChildrenRef())
		if (!nodeIdSetRef.test(child->GetId()))
			traverseAST(outputFD, nodeIdSetRef, child);
}

int main()
{
	size_t count = 1;
	char expression[EXPRESSION_SIZE];
	std::bitset<NODE_ID_SET_SIZE> nodeIdSet;

	auto testFileFD = fopen(TEST_FILE, "r");
	auto outputFileFD = fopen(OUT_FILE, "w");

	if (testFileFD == nullptr && !USE_STDIN)
	{
		std::cerr << "Cannot open the specified file\n";
		fclose(testFileFD);
		return -1;
	}

	if (outputFileFD == nullptr && !USE_STDOUT)
	{
		std::cerr << "Cannot open the specified file\n";
		fclose(outputFileFD);
		return -1;
	}

	auto inputFD = USE_STDIN ? stdin : testFileFD;
	auto outputFD = USE_STDOUT ? stdout : outputFileFD;
	while (fgets(expression, EXPRESSION_SIZE, inputFD) != nullptr)
	{
		if (strlen(expression) == 0 || expression[0] == '#')
		{
			fmt::fprintf(outputFD, expression);
			continue;
		}

		expression[strlen(expression) - 1] = '\0';

		fmt::fprintf(outputFD, "Expression #%d: \"%s\"\n", count, expression);
		count++;

		try
		{
			auto expressionParam = std::string(expression);
			auto root = RegenParser::Parser::ParseExpression(expressionParam);

			nodeIdSet.reset();
			traverseAST(outputFD, nodeIdSet, root);
		}
		catch (const std::exception &e)
		{
			fmt::fprintf(outputFD, "ERROR: \"%s\"\n", e.what());
		}

		fmt::fprintf(outputFD, "\n");

		//? Similar to std::endl
		fflush(outputFD);
	}

	fclose(testFileFD);
	fclose(outputFileFD);

	return 0;
}