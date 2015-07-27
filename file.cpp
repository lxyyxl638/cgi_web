#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

int main() {

	string a = "abcdef";
	string b = "ghi";
	a.erase(2,2);
	cout << a << endl;
	string c = a + b;
	cout << c << endl;
	cout << c.c_str() << endl;
	return 0;
}
