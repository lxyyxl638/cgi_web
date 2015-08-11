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
	string arg = UrlDecode(query_string.substr(x,s-x));
	string key = UrlDecode(query_string.substr(s+1,len-s-1));
	Param.insert(make_pair(arg,key));
	
	return true;
}
vector<unordered_map<string,string> > ParseArrayParam(string query_string)
{
	vector<unordered_map<string,string> > Param;
	size_t x=0;
	string first_param;
	unordered_map<string,string> level;

	size_t end =  query_string.find('&',x);
	size_t start  = query_string.find('=',x);
	string argu =  query_string.substr(x,start - x);
	string key = query_string.substr(start+1,end-start-1);
	pair<string,string> p = make_pair(argu,key);
	level.insert(p);
	first_param = argu;
	x = end+1;
	for( ; query_string.find('&',x) != query_string.npos;)
	{
		size_t end =  query_string.find('&',x);
		size_t start  = query_string.find('=',x);
		string argu =  query_string.substr(x,start - x);
		string key = query_string.substr(start+1,end-start-1);
		pair<string,string> p = make_pair(argu,key);
		if(argu == first_param){
			Param.push_back(level);
			level.clear();
		}
		level.insert(p);
		x = end+1;
	}
	size_t len = query_string.length();
	size_t s = query_string.find('=',x);
	argu = query_string.substr(x,s-x);
	key = query_string.substr(s+1,len-s-1);
	if(argu == first_param){
		Param.push_back(level);
		level.clear();
	}
	level.insert(make_pair(argu,key));
	Param.push_back (level);

	return Param;
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
string UrlEncode(const string& szToEncode) 
{
   	std::string src = szToEncode;
    	char hex[] = "0123456789ABCDEF";
   	string dst;
   	for(size_t i = 0; i < src.size();++i){
   		unsigned char cc = src[i];
   		if(isascii(cc)){
   			if(cc == ' '){
   				dst += "%20";
   			}else{
   				dst += cc;
   			}
   		}else{
   			unsigned char c = static_cast<unsigned char>(src[i]);
   			dst += '%';
   			dst += hex[c/16];
   			dst += hex[c%16];
   		}
   	}
   	return dst; 
}
string UrlDecode(const string& szToDecode) 
{
    	std::string result;
    	int hex = 0;
   	for (size_t i = 0; i < szToDecode.length(); ++i)
    	{
        	switch (szToDecode[i])
        	{
            		case '+':
            			result += ' ';
            			break;
            		case '%':
            		if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
            		{
                		std::string hexStr = szToDecode.substr(i + 1, 2);
                		hex = strtol(hexStr.c_str(), 0, 16);
                		//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]
                		//可以不经过编码直接用于URL
                		if (!((hex >= 48 && hex <= 57) || //0-9
                    		(hex >=97 && hex <= 122) ||   //a-z
                    		(hex >=65 && hex <= 90) ||    //A-Z
                    		//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]
                    		hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
                    		|| hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
                    		|| hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
                    		))
                		{
                    			result += char(hex);
                    			i += 2;
               		 	}else result += '%';
            		}else {
                		result += '%';
           		 }
            		break;
            		default:
            			result += szToDecode[i];
            			break;
        	}
    	}
    	return result;
}