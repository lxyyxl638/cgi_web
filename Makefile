all : hello sign_up post

hello : hello.cpp include/database.cpp include/database.h
	g++ -o hello hello.cpp include/database.cpp -std=c++11 `mysql_config --cflags --libs`

#example : demo.cpp example.tpl
#	g++ -o example demo.cpp -lctemplate_nothreads -lfcgi	

sign_up : sign_up.cpp dist/template/signup.tpl
	g++ -o sign_up sign_up.cpp include/database.cpp -std=c++11 `mysql_config --cflags --libs` -lctemplate_nothreads -lfcgi

post : post.cpp
	g++ -o post post.cpp -g -lfcgi
