#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string>
#include <cstring>
#include <time.h>
#include <unordered_map>
#include <fcgi_stdio.h>
#include <ctemplate/template.h>
#include "include/public.h"
#include "include/database.h"
#include "json/json.h"

using namespace std;
Database *db = Database::getInstance();

bool CheckUser(string username){	
	char buf[1024]={0};
	snprintf(buf,sizeof(buf),"select * from users where username = '%s' ",username.c_str());
	string query(buf);
	vector<vector<string> > ans;
	bool flag = db->dbQuery(query,ans);
	if(flag){
		if(ans.size() == 0){
			return false;
		}else{
			return true;
		}
	}else{
		return false;
	}
}
int main() {

#define FCGI_NET_OK 0
#define FCGI_NET_ERROR 1
#define FCGI_NET_PARAM_ERROR 2

int main() {

	Database *db = Database::getInstance();

	while (FCGI_Accept() >= 0) {

		unordered_map<string,string> ans;
		string result("fail");
		string detail("");
		char * method = getenv("REQUEST_METHOD");

		FCGI_printf("Content-type: text/html\r\n"
               		"\r\n");

		if (strcmp(method,"POST") == 0) {

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
		
		} else if(strcmp(method,"GET")==0) {

			//char* str = getenv("QUERY_STRING");
	        	//string Param(str);
	        	//ParseParam(Param,ans);

	    		ctemplate::TemplateDictionary dict("signup");
    			std::string output;
    			ctemplate::ExpandTemplate("./dist/template/signup.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);

			FCGI_printf("Content-type: text/html\r\n" 
				      "\r\n \"\" %s",output.c_str());
			continue;
 		 }

		string type;
		unordered_map<string,string>::iterator it;
		it = ans.find("type");
		
		if(it == ans.end()) {
			detail = "参数错误！";
		} else {
			int argu_count = 0;
			string username;
			 if(ans.find("username") != ans.end())
	        		argu_count++;
			if(it->second == "1"){
				if(argu_count  < 1) {
					detail = "参数错误!";
				} else {
					    username=ans["username"];
						if(CheckUser(username)){
							detail = "用户已经存在!";
						}else{
							result = "success";
						}
				}
			}else if(it->second == "2"){
				if(ans.find("password") != ans.end())
					argu_count++;
				if(ans.find("nickname")!= ans.end())
	        			argu_count++;
				if(ans.find("sex") != ans.end())
					argu_count++;
				if(argu_count < 4) {
					detail = "参数错误！";
				}else{
						username=ans["username"];
						if(CheckUser(username)){
							detail = "用户已经存在!";	
						}else{

							string table = "users";
							ans.erase("type");
							bool flag = db->dbInsert(ans,table);
							if(flag)
								result = "success";
							else 
								detail = "网络错误!";
						}
				}
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
