#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <Regen/Parser.h>
#include <Regen/AST.h>

#ifndef REGEN_HEADER_ONLY
#define REGEN_HEADER_ONLY

//? Stream-related variable definitions
const bool USE_STDIN = true;
const bool USE_STDOUT = true;
const char *const TEST_FILE = "test/cases_chclass.txt";
const char *const OUT_FILE = "test/cases_output.txt";

//? Regen-related flag definition
const bool REGEN_REGEX_COMPLIANT_FLAG = false;

const size_t NODE_ID_SET_SIZE = 50;

std::string typeToStr(RegenAST::NodeData::Type type)
{
	static std::vector<std::string> CONVERSION_VEC{"ENTRY", "LITERAL", "CHCLASS"};

	return CONVERSION_VEC.at((int)type);
}

void printNode(std::ostream &oStreamRef, std::shared_ptr<RegenAST::Node> nodeRef)
{
	fmt::print(oStreamRef, "Node {}:\n", nodeRef->GetId());
	fmt::print(oStreamRef, "\tType: {};\n", typeToStr(nodeRef->GetNodeDataPtr()->GetType()));

	//! fmt::print throws an "invalid utf-8" exception for negated character classes
	oStreamRef << "\tLiteral: " << nodeRef->GetNodeDataPtr()->GetLiteral() << '\n';

	for (const auto &child : nodeRef->GetChildrenListRef())
		fmt::print(oStreamRef, "\tChild: {};\n", child->GetId());
}

void traverseAST(std::ostream &oStreamRef, std::bitset<NODE_ID_SET_SIZE> &nodeIdSetRef, std::shared_ptr<RegenAST::Node> node)
{
	nodeIdSetRef.set(node->GetId(), true);
	printNode(oStreamRef, node);

	for (const auto &child : node->GetChildrenListRef())
		if (!nodeIdSetRef.test(child->GetId()))
			traverseAST(oStreamRef, nodeIdSetRef, child);
}

int main()
{
	size_t count = 1;
	std::string expression;
	std::bitset<NODE_ID_SET_SIZE> nodeIdSet;

	std::ifstream testStream;
	std::ofstream outputStream;

	if (!USE_STDIN)
	{
		testStream.open(TEST_FILE);

		if (!testStream.is_open())
		{
			std::cerr << "Cannot open the test file\n";
			return -1;
		}
	}

	if (!USE_STDOUT)
	{
		outputStream.open(OUT_FILE);

		if (!outputStream.is_open())
		{
			std::cerr << "Cannot open the output file\n";
			return -1;
		}
	}

	auto &iStreamRef = USE_STDIN ? std::cin : testStream;
	auto &oStreamRef = USE_STDOUT ? std::cout : outputStream;
	while (std::getline(iStreamRef, expression))
	{
		if (expression.empty() || expression.at(0) == '#')
		{
			fmt::print(oStreamRef, expression);
			continue;
		}

		fmt::print(oStreamRef, "Expression #{}: \"{}\"\n", count, expression);
		count++;

		try
		{
			auto root = RegenParser::Parser::ParseExpression(expression);

			nodeIdSet.reset();
			traverseAST(oStreamRef, nodeIdSet, root);
		}
		catch (const std::exception &e)
		{
			fmt::print(oStreamRef, "ERROR: \"{}\"\n", e.what());
		}

		fmt::print(oStreamRef, "\n");
		oStreamRef.flush();
	}

	return 0;
}

#endif