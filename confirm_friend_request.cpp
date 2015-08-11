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
				ParseParam(post_val,ans);

			} else if(strcmp(method,"GET")==0){
				char* str = getenv("QUERY_STRING");
				string Param(str);
				ParseParam(Param,ans);
			}

			int argu_count = 0;
			if(ans.find("request_uid") != ans.end()) {
				argu_count++;
			}
			if(ans.find("no_id") != ans.end()) {
				argu_count++;
			}

			if(argu_count < 2) {
				detail = "参数错误！";
			} else {
				//更新通知，state + 1
				int no_id = atoi(ans["no_id"].c_str());
				int request_uid = atoi(ans["request_uid"].c_str());
				string message = ans["message"];
				string team_id = ans["team_id"];
				int my_uid = atoi(session->getValue("user_id").c_str());

				char buffer[1024];
				memset(buffer,0,sizeof(buffer));
				snprintf(buffer,sizeof(buffer),"select send_id,state from notification where no_id=%d",no_id);
				string query_state(buffer);
				vector<unordered_map<string,string> > res;
				db->dbQuery(query_state,res);
				if (res.size() > 0) {
					int send_id = atoi(res[0]["send_id"].c_str());
					int state = atoi(res[0]["state"].c_str());
					if ((send_id != my_uid && state == 0) || (send_id == my_uid && state == 1)) {
						result = "success";
					}
				} else {
					result = "fail";
					detail = "服务器出错";
				}

				if (result == "success") {
					memset(buffer,0,sizeof(buffer));
					snprintf(buffer,sizeof(buffer),"update notification set state = state + 1,additional_message='%s' where no_id=%d",message.c_str(),no_id);
					string query(buffer);
					db->dbQuery(query);
						result = "success";

					//添加好友
					if (message == "accept") {
						memset(buffer,0,sizeof(buffer));
						snprintf(buffer,sizeof(buffer),"select id where (user_id=%d and friend_id=%d)or(user_id=%d and friend_id=%d)",my_uid,request_uid,request_uid,my_uid);
						string query_friend_exist(buffer);
						if (0 == db->dbQuery(query_friend_exist)) {
							memset(buffer,0,sizeof(buffer));
							snprintf(buffer,sizeof(buffer),"insert friendship(user_id,friend_id,team_id) value(%d,%d,%d)",my_uid,request_uid,atoi(team_id.c_str()));
							string query_insert(buffer);
							db->dbQuery(query_insert);
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
