#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <fcgi_stdio.h>
#include "include/database.h"
#include "json/json.h"

using namespace std;

unordered_map<string,string>  ParseParam(string query_string)
{
	unordered_map<string,string> Param;
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
	
	return Param;
}
int main() {
	Database *db = Database::getInstance();
	while (FCGI_Accept() >= 0) {
		FCGI_printf("Content-type: text/html\r\n"
               		"\r\n");
   		string result("fail");
		string detail("");
		unordered_map<string,string> ans;
		char * method = getenv("REQUEST_METHOD");
		if ( strcmp(method,"POST") == 0) {
			char *contentLength = getenv("CONTENT_LENGTH");
			int len;
			if (contentLength != NULL) {
				len = strtol(contentLength, NULL, 10);
			} else {
				len = 0;
			}
			int i, ch;
			string post_val="";
			for (i = 0; i < len; i++) {
				if ((ch = getchar()) < 0) {
					break;
				}
				post_val = post_val + (char) ch ;
			}
			ans= ParseParam(post_val);
		
		} else if(strcmp(method,"GET")==0){
			char* str = getenv("QUERY_STRING");
	        	string Param(str);
	        	ans= ParseParam(Param);
 		 }
 		 int argu_count = 0;
	        if(ans.find("username") != ans.end())
	        	argu_count++;
	        if(ans.find("password") != ans.end())
	        	argu_count++;
		if(argu_count < 2) {
			detail = "参数错误！";
		}
		else
		{
		       char query_buf[66] = {0};
			char update_buf[1024] = {0};
			string username,password;
			unordered_map<string,string>::iterator it;
			it = ans.find("username");
			username = it->second;
			it = ans.find("password");
			password = it->second;
			snprintf(query_buf,sizeof(query_buf),"select * from users where username = '%s' and password = '%s' ",username.c_str(),password.c_str());
			snprintf(update_buf,sizeof(update_buf),"update users set state = '1' where username = '%s' and password = '%s' ",username.c_str(),password.c_str());
			string query(query_buf);
			string update(update_buf);
			int rows = db->dbQuery(query);
			if(rows > 0){
				result = "success" ;
				int c = db->dbQuery(update);
			}else{
				detail = "用户名密码错误！";
			}
		} 
		Json::Value root;
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"fail") == 0){
			root["detail"] = Json::Value(detail);
		}
		Json::FastWriter fw;
		string str = fw.write(root);
		FCGI_printf("%s<br/>",str.c_str());
	}
	return 0;
}
