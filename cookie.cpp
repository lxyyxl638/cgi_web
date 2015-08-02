#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string>
#include <cstring>
#include <fcgi_stdio.h>
#include "include/session.h"
#include "include/public.h"
using namespace std;
int main() {
	
	
	Session * session = Session::getInstance();
	while (FCGI_Accept() >= 0) {
		
		FCGI_printf("Content-type: text/html\n");
		session->sessionInit();
		
		string str = session->getCookie();
		
		if (session->checkSession()) {
		    //session already exist
			 FCGI_printf("\r\n"
			"\r\n"
			"exist</br>");
	//		  continue;
		     session->setValue("username","carson");
		     session->setValue("age",21);
		     FCGI_printf("username is %s\n",session->getValue("username").c_str());
		     FCGI_printf("age is %s\n", session->getValue("age").c_str());
		    // continue;
		     unordered_map<string,string> my_map;
		     session->getAllValue(my_map);
		     
		     FCGI_printf("the size of my_map is %d</br>",my_map.size());
	             for (auto & p : my_map) {
			FCGI_printf("%s is %s</br>",p.first.c_str(),p.second.c_str());
		     }
		     //FCGI_printf("username in map is %s\n",my_map["username"].c_str());
		     //FCGI_printf("age in map is %s\n", my_map["age"].c_str());
		     
		} else {
		    
		//     FCGI_printf("\r\n"
		//		"\r\n"
		//	"else");
		    //continue;
		     //session doesn't exist,need to generate one
		     FCGI_printf("Set-Cookie:%s;PATH=/"
				"\r\n"
               			"\r\n",session->getCookie().c_str());
		     FCGI_printf("cookie is %s</br>",session->getCookie().c_str());
		}
		char* req_uri = getenv("REQUEST_URI");
		FCGI_printf("Request_uri:%s<br/>",req_uri);
		char* cont_type = getenv("CONTENT_TYPE");
		FCGI_printf("Content_type:%s<br/>",cont_type);
		char * method = getenv("REQUEST_METHOD");
		FCGI_printf("Method %s<br/>" , method);
		
		FCGI_printf("Cookie is %s",session->getCookie().c_str());

		if ( strcmp(method,"POST") == 0) {
			char *contentLength = getenv("CONTENT_LENGTH");
			int len;
			if (contentLength != NULL) {
				len = strtol(contentLength, NULL, 10);
			} else {
				len = 0;
			}
			FCGI_printf("Content_length:%d<br/>",len);
			int i, ch;
			string post_val="";
			for (i = 0; i < len; i++) {
				if ((ch = getchar()) < 0) {
					FCGI_printf("Error: Not enough bytes received on standard input<p>/n");
					break;
				}
			//	putchar(ch);
				post_val = post_val + (char) ch ;
			}
			FCGI_printf("%s\n",post_val.c_str());
			FCGI_printf("</pre>");

		} else {
			FCGI_printf("FastCGI Hello! (C, fcgi_stdio library)"
			"Request number running on host %s Process ID:", getenv("SERVER_NAME")); 
 		 }
	}
	return 0;
}
