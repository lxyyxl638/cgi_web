#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <fcgi_stdio.h>
#include "include/database.h"
#include "include/public.h"
#include "include/session.h"
#include "json/json.h"
#include <ctemplate/template.h>
#include <unistd.h>

using namespace std;

int main() {

	Database *db = Database::getInstance();
	/*
	   Get a instance of session at first
	*/
	Session *session = Session::getInstance();
	
	while (FCGI_Accept() >= 0) {
		
		/*
		    call this method every time to initialize the session
		*/
		session->sessionInit();


		//FCGI_printf("Content-type: text/html\r\n"
               //		"\r\n");
		
		
		string result("fail");
		string detail("");

		unordered_map<string,string> ans;

		char * method = getenv("REQUEST_METHOD");

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
		
		} else if(strcmp(method,"GET")==0){

			//char* str = getenv("QUERY_STRING");
	        	//string Param(str);
	        	//ParseParam(Param,ans);
			ctemplate::TemplateDictionary dict("login");
			std::string output;
	                ctemplate::ExpandTemplate("./dist/template/login.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);
			
			FCGI_printf("Content-type : text/html\r\n"
			"\r\n"
			"%s",output.c_str());
			continue;
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
		       char query_buf[1024] = {0};
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
				result = "success";
				int c = db->dbQuery(update);
				/*
					need to set session and reply a cookie to client
				*/
				
				string cookie = session->getCookie();
				FCGI_printf("Set-Cookie:%s;PATH=/",cookie.c_str());
	      
	     			 /*
	     			 	set session
	     			 */
	     			 vector<unordered_map<string,string> > info;
	     			 db->dbQuery(query,info);
			
				session->setValue("username",info[0]["username"]);
				session->setValue("nickname",info[0]["nickname"]);
				session->setValue("sex",info[0]["sex"]);


			}else{
				detail = detail + " 用户名密码错误！";			
			}
		} 

		/*
			the boundary of header and body
		*/
		
		
		FCGI_printf("Content-type: application/json\r\n"
		"\r\n");
		Json::Value root;
		root["result"] = Json::Value(result);
		if(strcmp(result.c_str(),"fail") == 0){
			root["detail"] = Json::Value(detail);
		}
		Json::FastWriter fw;
		string str = fw.write(root);
		FCGI_printf("%s",str.c_str());
	
	}
	return 0;
}
