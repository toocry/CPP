#include <iostream>
#include <stack>
#include <memory>
#include <string>
#include <vector>
#include <string>


struct Node
{
	std::string val;
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;
	Node(std::string data){}
	Node(std::string data, std::shared_ptr<Node> left_, std::shared_ptr<Node> right_){}
};

void replace_node(Node& node, std::string value, std::shared_ptr<Node> left, std::shared_ptr<Node> right);

bool is_number(const std::string& s);

bool is_operator(const std::string& s);

bool isZero(const std::string& str);

bool isOne(const std::string& str);

int precedence(std::string& str);

std::vector<std::string> parse(const std::string& str);

std::string infix_to_postfix(const std::string& str);

std::shared_ptr<Node> postfix_to_tree(const std::string& str);

std::shared_ptr<Node> derivative(const std::shared_ptr<Node> node);

void narrow_tree(std::shared_ptr<Node> node);

void dfs(std::shared_ptr<Node> node);

void output(const std::string& infix);

