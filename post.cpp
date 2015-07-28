#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string>
#include <cstring>
#include <fcgi_stdio.h>
using namespace std;
int main() {
	int count = 0;
	while (FCGI_Accept() >= 0) {
		count++;
		char* req_uri = getenv("REQUEST_URI");
		FCGI_printf("Content-type: text/html\r\n"
               		"\r\n");
		
		FCGI_printf("Request_uri:%s<br/>",req_uri);
		char* cont_type = getenv("CONTENT_TYPE");
		FCGI_printf("Content_type:%s<br/>",cont_type);
		char * method = getenv("REQUEST_METHOD");
		FCGI_printf("Method %s<br/>" , method);
		
	
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
			"Request number %d running on host %s Process ID:",count, getenv("SERVER_NAME")); 
 		 }
	}
	return 0;
}
