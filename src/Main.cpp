#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <memory>
#include "AST.h"
#include "Parser.h"

// TODO: Refactor the parser + rename exception messages
// TODO: Test the parser using doctest

const size_t nodeIdSetSize = 50;
std::bitset<nodeIdSetSize> nodeIdSet;

const std::vector<std::string> nodeTypeToStr{
	"ENTRY",
	"LITERAL",
	"CHCLASS"};

void printASTNode(std::shared_ptr<RegenAST::ASTNode> nodeRef)
{
	std::cout << "Node " << nodeRef.get()->GetId() << ":\n";
	if (nodeRef.get()->GetParent() != nullptr)
		std::cout << '\t';
	std::cout << "Type -> " << nodeTypeToStr.at((int)nodeRef.get()->Data.GetNodeType()) << ";\n";
	if (nodeRef.get()->GetParent() != nullptr)
		std::cout << '\t';
	std::cout << "Literal -> " << nodeRef.get()->Data.GetLiteral() << ";\n";

	for (const auto &child : nodeRef.get()->GetChildren())
	{
		if (nodeRef.get()->GetParent() != nullptr)
			std::cout << '\t';
		std::cout << "Child -> " << child.get()->GetId() << ";\n";
	}

	std::cout << '\n';
}

void traverseAST(std::shared_ptr<RegenAST::ASTNode> node)
{
	nodeIdSet.set(node.get()->GetId(), true);
	printASTNode(node);

	for (const auto &child : node.get()->GetChildren())
		if (!nodeIdSet.test(child.get()->GetId()))
			traverseAST(child);
}

int main()
{

	std::string expression;
	std::getline(std::cin, expression);

	auto root = RegenParser::parseExpression(expression);
	traverseAST(root);

	return 0;
}