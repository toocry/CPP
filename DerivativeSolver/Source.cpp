#include <iostream>
#include <stack>
#include <memory>
#include <string>
#include <vector>

using namespace std;
struct Node;
using ptr = shared_ptr<Node>;

struct Node
{
	string val;
	ptr left;
	ptr right;
	Node(string data)
	{
		val = data;
		left = nullptr;
		right = nullptr;
	}
	Node(string data, ptr left_, ptr right_)
	{
		val = data;
		left = left_;
		right = right_;
	}
};

void replace_node(Node& node, string value, ptr left, ptr right) {
	node.val = value;
	node.left = left;
	node.right = right;
}

bool is_number(const string& s) 
{
	try
	{
		stod(s);	
	}
	catch (exception& e)     
	{
		return false;
	}
	return true;
}

bool is_operator(const string& s)
{
	if (s == "+" || s == "-" || s == "*" || s == "/")
		return true;
	else
		return false;
}

bool isZero(const string& str) {
	try
	{
		double d = stod(str);
		if (d != 0) {
			return false;
		}
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

bool isOne(const string& str) {
	try
	{
		double d = stod(str);
		if (d != 1) {
			return false;
		}
	}
	catch (exception& e)
	{
		return false;
	}
	return true;
}

int precedence(string& c) {
	if (c == "+" || c == "-")
		return 1;
	else if (c == "*" || c == "/")
		return 2;
	else
		return 0;
}

vector<string> parse(const string& str) 
{
	string help;
	vector<string> parse_;
	for (auto itr = str.begin(); itr != str.end(); ++itr) 
	{
		if (*itr == ' ' && !help.empty())
		{
			parse_.push_back(help);
			help = "";
		}
		else 
		{
			help += *itr;

		}
		
	}
	if(!help.empty()){
		parse_.push_back(help);
	}
	return parse_;
}


string infix_to_postfix(const string& str)
{
	string exception_ = "Input error";
	string top;
	stack<string> stack_;
	vector<string> parsed_infix = parse(str);
	string postfix;

	for (auto itr = parsed_infix.begin(); itr != parsed_infix.end(); ++itr)
	{
		if (*itr == "(")
		{
			stack_.push(*itr);
		}
		else if (is_operator(*itr))
		{
			while ((!stack_.empty())
				&& (stack_.top() != "(")
				&& precedence(stack_.top()) >= precedence(*itr))
			{
				postfix += stack_.top() + " ";
				stack_.pop();
			}
			stack_.push(*itr);
		}
		else if ( *itr ==  ")" )
		{
			if (stack_.empty())
				return exception_;
			while (stack_.top() != "(")
			{
				postfix += stack_.top() + " ";
				stack_.pop();
				if (stack_.empty())
					return exception_;
			}
			stack_.pop();
		}
		else if (is_number(*itr) || *itr == "x")
		{
			postfix += (*itr + " ");
		}
		else
		{
			return exception_;
		}
	}
	while (!stack_.empty())
	{
		top = stack_.top();
		if (top != "+" && top != "-" && top != "*" && top != "/")
		{
			return exception_;
		}
		postfix += stack_.top() + " ";
		stack_.pop();
	}
	return postfix;
}

ptr postfix_to_tree(const string& str)
{
	if (str == "Input error")
		return nullptr;

	vector<string> parsed_postfix = parse(str);
	stack<ptr> ptr_to_nodes;
	
	for (auto itr = parsed_postfix.begin(); itr != parsed_postfix.end(); ++itr)
	{
		ptr node = make_shared<Node>(*itr);
		if (is_number(*itr) || *itr == "x")
		{
			node->right = nullptr;
			node->left = nullptr;
			
		}
		else if (is_operator(*itr)) 
		{
			if (ptr_to_nodes.size() < 2) 
				return nullptr;
			node->right = ptr_to_nodes.top();
			ptr_to_nodes.pop();
			node->left = ptr_to_nodes.top();
			ptr_to_nodes.pop();
		}
		ptr_to_nodes.push(node);
	}
	if (ptr_to_nodes.size() != 1) 
		return nullptr;
	
	return ptr_to_nodes.top();

}

ptr derivative(const ptr node) {
	auto val = node->val;
	ptr node_der;
	
	if (val == "x"){
		ptr node_der = make_shared<Node>("1");
		return node_der;
	}
		
	else if (is_number(val)){
		ptr node_der = make_shared<Node>("0");
		return node_der;
	}
		
	else if (is_operator(val)) 
	{
		auto old_left = node->left;
		auto old_right = node->right;
		auto left_der = derivative(make_shared<Node>(*old_left));
		auto right_der = derivative(make_shared<Node>(*old_right));

		if (val == "+") 
		{
			ptr node_der = make_shared<Node>("+",left_der,right_der);
			return node_der;
		}
		else if (val == "-")
		{
			ptr node_der = make_shared<Node>("-",left_der,right_der);
			return node_der;
		}
		// a' * b + a * b'
		else if (val == "*") 
		{
			ptr node_der = 
				make_shared<Node>("+",
					make_shared<Node>("*", left_der, old_right),
					make_shared<Node>("*", old_left, right_der));
			return node_der;
		}

		else if (val == "/")
		{
			ptr node_der = 
				make_shared<Node>("/",
					make_shared<Node>("-",
						make_shared<Node>("*", left_der, old_right),
						make_shared<Node>("*", old_left, right_der)),
					make_shared<Node>("*",old_right,old_right));
			return node_der;
		}
		
	}return node_der;
		
}

void narrow_tree(ptr node)
{
	auto val = node->val;
	auto left = node->left;
	auto right = node->right;
	if (is_operator(left->val))
		narrow_tree(left);
	if (is_operator(right->val))
		narrow_tree(right);
	
	if (val == "+") 
	{
		if (is_number(left->val) && is_number(right->val)) 
			replace_node(*node, to_string(stod(left->val) + stod(right->val)), nullptr, nullptr);
		else if(isZero(left->val))
			replace_node(*node, right->val, right->left, right->right);
		else if (isZero(right->val))
			replace_node(*node, left->val, left->left, left->right);
		
	}
	else if (val == "-")
	{
		if (is_number(left->val) && is_number(right->val))
			replace_node(*node, to_string(stod(left->val) - stod(right->val)), nullptr, nullptr);
		
		else if (isZero(right->val))
			replace_node(*node, left->val, left->left, left->right);
		
	}
	else if (val == "*")
	{
		if (is_number(left->val) && is_number(right->val))
			replace_node(*node, to_string(stod(left->val) * stod(right->val)), nullptr, nullptr);
		else if (isZero(left->val) || isZero(right->val))
			replace_node(*node, "0", nullptr, nullptr);
		else if (isOne(left->val))
			replace_node(*node, right->val, right->left, right->right);
		else if(isOne(right->val))
			replace_node(*node, left->val, left->left, left->right);
	}
	else if (val == "/") 
	{
		if (is_number(left->val) && is_number(right->val))
			replace_node(*node, to_string(stod(left->val) / stod(right->val)), nullptr, nullptr);
		else if (isZero(left->val))
			replace_node(*node, "0", nullptr, nullptr);
		else if (isOne(right->val))
			replace_node(*node, left->val, left->left, left->right);
	}
};

void dfs(ptr node) {
	if (node->left != nullptr) {
		cout << "(";
		dfs(node->left);
	}

	if (is_number(node->val) || node->val == "x") {
		is_number(node->val) ? cout << stod(node->val) : cout << node->val;
	}
	else {
		cout << " " << node->val << " ";
	}

	if (node->right != nullptr) {
		dfs(node->right);
		cout << ")";
	}

}

void output(const string& infix) {
	if (infix.empty()) 
		return;
		 
	auto node = postfix_to_tree(infix_to_postfix(infix));

	if (node == nullptr) {
		cout << "Input error" << endl;
		return;
	}
	auto node2 = derivative(node);
	narrow_tree(node2);
	dfs(node2);
	cout << endl;

}

int main()
{
	

	string line;
	while (getline(cin, line)) {
		output(line);
	}
	return 0;
}