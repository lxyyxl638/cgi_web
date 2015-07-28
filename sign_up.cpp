#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctemplate/template.h>
#include <fcgi_stdio.h>
#include <unistd.h>
using namespace std;
int main(int argc, char** argv) {
	
while (FCGI_Accept() >= 0) {  

    	ctemplate::TemplateDictionary dict("signup");
    	std::string output;
    	ctemplate::ExpandTemplate("./dist/template/signup.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);

    	//output="Content-type: text/html\r\n \r\n" + output;
	cout << output << endl;
	FCGI_printf("Content-type: text/html\r\n \r\n \"\" %s",output.c_str());
    }
    return 0;
}



