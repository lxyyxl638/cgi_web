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
			if(ans.find("request_uid") != ans.end())
				argu_count++;
			if(ans.find("operation_type") != ans.end())
				argu_count++;
			if(argu_count < 2) {
				detail = "参数错误！";
			}
			else
			{
				char update_buf[1024] = {0};
				char insert_buf[1024] = {0};
				string request_uid,response_uid,team_id,operation_type;
				unordered_map<string,string>::iterator it;
				it = ans.find("request_uid");
				request_uid = it->second;
				it = ans.find("operation_type");
				operation_type = it->second;

				response_uid = session->getValue("user_id");

				if(strcmp(operation_type.c_str(),"refused")==0){
					snprintf(update_buf,sizeof(update_buf),"update notification set handling_time = '%s' where send_id = %d and rece_id = %d ",GetCurrentTime().c_str(),atoi(request_uid.c_str()),atoi(response_uid.c_str()));
					string update(update_buf);
					int rows = db->dbQuery(update);
					if(rows){
						result = "success" ;

					}else{
						detail = "数据库操作错误！";
					}
				}else if(strcmp(operation_type.c_str(),"accept")==0){
					if(ans.find("team_id") == ans.end()){
						detail = "参数错误！";
					}else{
						it = ans.find("team_id");
						team_id = it->second;
						snprintf(update_buf,sizeof(update_buf),"update notification set handling_time = '%s', state = 1  where send_id = %d and rece_id = %d ",GetCurrentTime().c_str(),atoi(request_uid.c_str()),atoi(response_uid.c_str())); 
						snprintf(insert_buf,sizeof(insert_buf),"insert friendship (user_id,friend_id,team_id) values( %d , %d, %d)",atoi(response_uid.c_str()),atoi(request_uid.c_str()),atoi(team_id.c_str()));
						string update(update_buf);
						string insert(insert_buf);
						if(db->dbQuery(update) && db->dbQuery(insert)){
							result = "success";
						}else{
							detail = "数据库操作错误！！";
						}
					}	
				}
			} 
		}

		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"fail") == 0){
			root["detail"] = Json::Value(detail);
		}
		string str = fw.write(root);
		FCGI_printf("Content-type: application/json\r\n"
			"\r\n"
			"%s",str.c_str());
		
	}
	return 0;
}
