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
#include "include/session.h"

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
string GetCurrentTime()
{
	time_t now_time;
	now_time = time(NULL);	
	struct tm *local;
	
	local=localtime(&now_time);
	char buf[80];
	strftime(buf,80,"%Y-%m-%d %H:%M:%S",local);
	return buf;
}
int main() {
	Database *db = Database::getInstance();
	Session *session = Session::getInstance();
	while (FCGI_Accept() >= 0) {
		session->sessionInit();
		string result("fail");
		string detail("");
		Json::FastWriter fw;
		Json::Value root;
		vector<unordered_map<string,string> > ans;
		if(session->checkSession() == false){
			detail = detail + "unlogin";
		}else{
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
				ans= ParseArrayParam(post_val);

			} else if(strcmp(method,"GET")==0){
				char* str = getenv("QUERY_STRING");
				string Param(str);
				ans= ParseArrayParam(Param);
			}
			string friend_id,state;
			bool flag;
			
			for(int i = 0; i != ans.size(); i++)
			{
				friend_id = ans[i]["friend_id"];
				
				flag = session->getOnline(atoi(friend_id.c_str()));
				state = flag ? "1":"0";
				ans[i].insert(make_pair("state",state));
				
			}
			result = "success";

		}
		root["result"] = Json::Value(result);
		Json::Value friend_list;
		if(strcmp(result.c_str(),"success") == 0){
			for(int i  = 0 ; i != ans.size();i++){
				Json::Value friend_state;
				friend_state["friend_id"] = ans[i]["friend_id"];
				friend_state["state"] =  ans[i]["state"];

				root["friend_list"].append(friend_state);
			}
			
		}else{
			root["detail"] = Json::Value(detail);
		} 
		string str = fw.write(root);
		FCGI_printf("Content-type: application/json\r\n"
			"\r\n"
			"%s",str.c_str());
		
	}
	return 0;
}
