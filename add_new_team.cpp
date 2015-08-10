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
#include "include/public.h"

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
				ParseParam(post_val,ans);

			} else if(strcmp(method,"GET")==0){
				char* str = getenv("QUERY_STRING");
				string Param(str);
				ParseParam(Param,ans);
			}
			int argu_count = 0;
			
			
			if(ans.find("team_name") != ans.end())
				argu_count++;
			if(argu_count < 1) {
				detail = "参数错误！";
			}
			else
			{
				char query_buf[1024] = {0};
				string user_id,team_name;
				unordered_map<string,string>::iterator it;
				it = ans.find("team_name");
				team_name = it->second;

				user_id = session->getValue("user_id");

				//查询是否有重名
				snprintf(query_buf,sizeof(query_buf),"select team_id from teams where user_id=%d and team_name=%s",atoi(user_id.c_str()),team_name.c_str());
				string query_exist(query_buf);
				int num = db->dbQuery(query_exist);
				if (num == 0) {
					//插入
					memset(query_buf,0,sizeof(query_buf));
					snprintf(query_buf,sizeof(query_buf),"insert  teams (user_id,team_name,team_type,remark) values(%d,'%s',0,'无')",atoi(user_id.c_str()),team_name.c_str());
					string query(query_buf);
					int rows = db->dbQuery(query);
					if(rows){
						result = "success" ;

					}else{
						detail = "网络链接错误！";
					}
				} else {
					detail = "已有同名组，请换名";
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
