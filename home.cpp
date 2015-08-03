#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctemplate/template.h>
#include <fcgi_stdio.h>
#include <unistd.h>
using namespace std;
int main(int argc, char** argv) {
	

    	ctemplate::TemplateDictionary dict("home");

	while (FCGI_Accept() >= 0) {  

    	std::string output;
    	ctemplate::ExpandTemplate("./dist/template/home.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);

    
	FCGI_printf("Content-type: text/html\r\n"
			"\r\n %s",output.c_str());
    }
    return 0;
}



