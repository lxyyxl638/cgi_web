hello : hello.cpp include/database.cpp include/database.h
	g++ -o hello hello.cpp include/database.cpp -std=c++11 `mysql_config --cflags --libs`
	
