all : sign_in sign_up home db_test get_all_friend_team get_friends_by_team query_users get_notification send_friend_request get_notification_num add_new_team

add_new_team : add_new_team.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o add_new_team add_new_team.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs`

get_notification_num : get_notification_num.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o get_notification_num get_notification_num.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs`

get_notification : get_notification.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o get_notification get_notification.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs`

send_friend_request : send_friend_request.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o send_friend_request send_friend_request.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs`

query_users : query_users.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o query_users query_users.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs`

get_all_friend_team : get_all_friend_team.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o get_all_friend_team get_all_friend_team.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs`

get_friends_by_team : get_friends_by_team.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp
	g++ -o get_friends_by_team get_friends_by_team.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs`

home : home.cpp dist/template/home.tpl
	g++ -o home home.cpp -lctemplate_nothreads -lfcgi

sign_in : sign_in.cpp include/database.h include/database.cpp include/session.h include/session.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp dist/template/login.tpl
	g++ -o sign_in sign_in.cpp include/database.cpp include/session.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs` -lctemplate_nothreads

db_test : db_test.cpp include/database.cpp include/database.h
	g++ -o db_test db_test.cpp include/database.cpp -std=c++11 -g `mysql_config --cflags --libs`

#example : demo.cpp example.tpl
#	g++ -o example demo.cpp -lctemplate_nothreads -lfcgi	

sign_up : sign_up.cpp include/database.h include/database.cpp include/public.h include/public.cpp json/json.h json/jsoncpp.cpp dist/template/signup.tpl
	g++ -o sign_up sign_up.cpp include/database.cpp include/public.cpp json/jsoncpp.cpp -std=c++11 -lhiredis -lfcgi `mysql_config --cflags --libs` -lctemplate_nothreads

post : post.cpp
	g++ -o post post.cpp -g -lfcgi -std=c++11

db_init : db_init.cpp include/database.cpp include/database.h
	g++ -o db_init db_init.cpp include/database.cpp -std=c++11 -g `mysql_config --cflags --libs`

cookie : cookie.cpp include/session.h include/session.cpp include/public.h include/public.cpp
	g++ -o cookie cookie.cpp include/session.cpp include/public.cpp -std=c++11 -lhiredis -lfcgi
