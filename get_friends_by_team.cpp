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
int main() {
	Database *db = Database::getInstance();
	Session *session = Session::getInstance();

	while (FCGI_Accept() >= 0) {
		Json::FastWriter fw; 
		Json::Value root;
		string result("fail");
		string detail("");
		session->sessionInit();
		vector<unordered_map<string,string> >   query_result;
		if(session->checkSession() == false){
			detail = detail + "unlogin";

		}else{
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

			if(ans.find("team_id") != ans.end())
				argu_count++;
			if(argu_count < 1) {
				detail = "参数错误！";
			}
			else
			{
				char query_buf[1024] = {0};
				string user_id,team_id;
				unordered_map<string,string>::iterator it;
				it = ans.find("team_id");
				team_id = it->second;

				user_id = session->getValue("user_id");

				snprintf(query_buf,sizeof(query_buf),"select friend_id,friend_name,friend_nickname from friendship  where user_id  =  %d  and team_id = %d ",atoi(user_id.c_str()),atoi(team_id.c_str()));
				string query(query_buf);
				int flag = db->dbQuery(query,query_result);
				if(flag ){
					result = "success" ;

				}else{
					detail = "用户名密码错误！";
				}
			}
		}
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"success") == 0){
			Json::Value friend_list;
			for(size_t i = 0 ; i != query_result.size(); i++ ){
				Json::Value friend_info;
				friend_info["friend_id"] = Json::Value(query_result[i]["friend_id"]);
				friend_info["friend_name"] = Json::Value(query_result[i]["friend_name"]);
				friend_info["friend_nickname"] = Json::Value(query_result[i]["friend_nickname"]);
				root["friend_list"].append(friend_info);
			}

		}else{
			root["detail"] = Json::Value(detail);
		}
		string str = fw.write(root);
		FCGI_printf("Content-type: text/html\r\n"
			"\r\n"
			"%s<br/>",str.c_str());
		
		
	}
	return 0;
}
