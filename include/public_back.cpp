#include "public.h"
#include <sys/time.h>
#include <sstream>
bool ParseParam(string & query_string,unordered_map<string,string> & Param)
{
	Param.clear();
	size_t x;
	for( x = 0; query_string.find('&',x) != query_string.npos;)
	{
		size_t end =  query_string.find('&',x);
		size_t start  = query_string.find('=',x);
		string argu =  query_string.substr(x,start - x);
		string key = query_string.substr(start+1,end-start-1);
		pair<string,string>p = make_pair(argu,key);
		
		Param.insert(p);
		 x = end+1;
	}
	size_t len = query_string.length();
	size_t s = query_string.find('=',x);
	string arg = query_string.substr(x,s-x);
	string key = query_string.substr(s+1,len-s-1);
	Param.insert(make_pair(arg,key));
	
	return true;
}


string getCurrentTime() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	stringstream ss;
	ss << tv.tv_usec;
	return ss.str();
}

string getEnvir(string query) {

    char *result = getenv(query.c_str());
    if (0 == result) {
	return "";
     }	else {
	return result;
     }
}
