#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <memory>
#include "AST.h"
#include "Parser.h"

/// TODO LIST
// TODO: Refactor the parser's logic.
// TODO: Implement AST normalization node transformations. (character class inside character class)
// TODO: Implement character class intersections.
// TODO: Warn user user to use \W instead of [^\w]. (bitset hash might help)
// TODO: Warn user user to use \d instead of [0-9].
// TODO: Test the parser's behaviour via doctest.
// TODO: Create the documentation via doxygen.

const bool REGEN_REGEX_COMPLIANT = false;

const size_t nodeIdSetSize = 50;
std::bitset<nodeIdSetSize> nodeIdSet;

const std::vector<std::string> nodeTypeToStr{
	"ENTRY",
	"LITERAL",
	"CHCLASS"};

void printASTNode(std::shared_ptr<RegenAST::ASTNode> nodeRef)
{
	std::cout << "Node " << nodeRef.get()->GetId() << ":\n";
	if (nodeRef.get()->GetParentRef() != nullptr)
		std::cout << '\t';
	std::cout << "Type -> " << nodeTypeToStr.at((int)nodeRef.get()->GetDataRef().GetNodeType()) << ";\n";
	if (nodeRef.get()->GetParentRef() != nullptr)
		std::cout << '\t';
	std::cout << "Literal -> " << nodeRef.get()->GetDataRef().GetLiteral() << ";\n";

	for (const auto &child : nodeRef.get()->GetChildrenRef())
	{
		if (nodeRef.get()->GetParentRef() != nullptr)
			std::cout << '\t';

		std::cout << "Child -> " << child.get()->GetId() << '\n';
	}

	std::cout << '\n';
}

void traverseAST(std::shared_ptr<RegenAST::ASTNode> node)
{
	nodeIdSet.set(node.get()->GetId(), true);
	printASTNode(node);

	for (const auto &child : node.get()->GetChildrenRef())
		if (!nodeIdSet.test(child.get()->GetId()))
			traverseAST(child);
}

int main()
{

	std::string expression;
	std::getline(std::cin, expression);

	auto root = RegenParser::Parser::ParseExpression(expression);
	traverseAST(root);

	return 0;
}