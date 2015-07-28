#include <fcgi_stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;


void sign_up() {

        FCGI_printf("Content-type: text/html\r\n"
                "\r\n"
		""
                "<form name=\"input\" action=\"http://localhost/sign_up.cgi\" method=\"post\">"
                "Username:"
                "<input type=\"text\" name=\"username\"/>"
                "<input type=\"submit\" value=\"Submit\"/>"
                "</form>"
               );
	return;
}

int main() {

	string method;
        while (FCGI_Accept() >= 0) {
        	method = getenv("CONTENT_LENGTH");
		sign_up();
		FCGI_printf(method.c_str());
	}

	return 0;
}
              
