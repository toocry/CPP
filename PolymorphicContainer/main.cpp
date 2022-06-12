#include <iostream>
#include <string>
#include "main.h"

using namespace std;

int main() {
	Seznam s1, s2;
	s1.add(make_unique<IntVal>(123));
	s1.add(make_unique<StringVal>("lorem ipsum"));
	s1.add(make_unique<DoubleVal>(3.14159));
	s1.add(make_unique<ComplexVal>(42, 2));
	s1.add(make_unique<FractionVal>(10, 16));

	s2 = s1;
	s2.print();
	cout << endl;
	
	Seznam s3(s1);
	s3.print();

	return 0;
}