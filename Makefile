all : sign_in post db_init cookie db_test

sign_in : sign_in.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o sign_in sign_in.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs` -lctemplate_nothreads
db_test : db_test.cpp include/database.cpp include/database.h
	g++ -o db_test db_test.cpp include/database.cpp -std=c++11 -g `mysql_config --cflags --libs`

#example : demo.cpp example.tpl
#	g++ -o example demo.cpp -lctemplate_nothreads -lfcgi	

sign_up : sign_up.cpp dist/template/signup.tpl
	g++ -o sign_up sign_up.cpp include/database.cpp -std=c++11 `mysql_config --cflags --libs` -lctemplate_nothreads -lfcgi

post : post.cpp
	g++ -o post post.cpp -g -lfcgi -std=c++11

db_init : db_init.cpp include/database.cpp include/database.h
	g++ -o db_init db_init.cpp include/database.cpp -std=c++11 -g `mysql_config --cflags --libs`

cookie : cookie.cpp include/session.h include/session.cpp include/public.h include/public.cpp
	g++ -o cookie cookie.cpp include/session.cpp include/public.cpp -std=c++11 -lhiredis -lfcgi
