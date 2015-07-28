#include "public.h"

int main() {
	
	unordered_map<string,string> Param;
	
	string str="username=lin&password=&nickname=carson&sex=male";
	
	ParseParam(str,Param);
	
	for (auto p : Param) {
		cout << p.first << ":" << p.second << endl;
	}
	return 0;
}