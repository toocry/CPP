#ifndef MAIN_
#define MAIN_
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class AbstractVal 
{
public:
	virtual ~AbstractVal(){}
	virtual void print() = 0;
	virtual unique_ptr<AbstractVal> clone() = 0;
};
using valptr = unique_ptr<AbstractVal>;

class Seznam 
{
public:
	void add(valptr p) 
	{
		pole.push_back(move(p));
	}
	void print() 
	{
		for (auto&& x : pole) 
		{
			x->print(); cout << endl;
		}
	}
	
	
	Seznam(){}
	Seznam(const Seznam& s) 
	{
		clone(s);
	}

	Seznam& operator=(const Seznam& s) 
	{
		if (this == &s) 
			return *this;
		pole.clear();
		clone(s);
		return *this;
	}
private:
	vector<valptr> pole;
	void clone(const Seznam& s) 
	{
		for (auto&& x : s.pole)
			pole.push_back(x->clone());
	}
};


class IntVal : public AbstractVal 
{
public:
	IntVal(int x) : x_(x){}
	void print() override { cout << x_; }
	valptr clone() override 
	{
		return make_unique<IntVal>(*this);
	}
private:
	int x_{};
};

class StringVal : public AbstractVal 
{
public:
	StringVal(string x): x_(x){}
	void print() override { cout << x_; }
	valptr clone() override 
	{
		return make_unique<StringVal>(*this);
	}

private:
	string x_;
};

class FractionVal : public AbstractVal
{
public:
	FractionVal(int num, int denom) : num_(num), denom_(denom) {}
	void print() override { cout << num_ << '/' << denom_; }
	valptr clone() override
	{
		return make_unique<FractionVal>(*this);
	}

private:
	int num_;
	int denom_;
};

class DoubleVal : public AbstractVal
{
public:
	DoubleVal(double x): x_(x) {}
	void print() override {cout << x_;}
	valptr clone() override {
		return make_unique<DoubleVal>(*this);
	}

private:
	double x_;

};

class ComplexVal : public AbstractVal
{
public:
	ComplexVal(int real, int img) : real_(real), img_(img) {}
	void print() override { cout << real_ << '+' << img_ << 'i'; }
	valptr clone() override
	{
		return make_unique<ComplexVal>(*this);
	}

private:
	int real_;
	int img_;
};

#endif
