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
			
			string send_id,rece_id,message,send_time,state;
			send_id = session->getValue("user_id");
			rece_id = ans["rece_id"];
			message = ans["message"];
			state = ans["state"];
			//send_time =  ans["send_time"];
			int flag; 
			char query_buf[1024]={0};
			snprintf(query_buf,sizeof(query_buf),"insert  p2p_messages ( send_id,rece_id,message,state ) values ( %d,%d,'%s',%d)",atoi(send_id.c_str()),atoi(rece_id.c_str()),message.c_str(),atoi(state.c_str()));
			string query(query_buf);
			flag = db->dbQuery(query);
			if(flag > 0){
				result = "success";
			}else{
				detail = "数据库操作失败";
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
