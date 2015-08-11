#include <cstring>
#include <string>
#include <unordered_map>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;

bool ParseParam(string &, unordered_map<string,string> &);

vector<unordered_map<string,string> > ParseArrayParam(string query_string);

unordered_map<string,string>  ParseParam(string query_string);

string getCurrentTime();

string getEnvir(string query);

string UrlEncode(const string& szToEncode);

string UrlDecode(const string& szToDecode);
