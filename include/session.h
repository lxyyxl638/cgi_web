#ifndef SESSION_H
#define SESSION_H
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <mysql/mysql.h>
#include <ctime>
#include <vector>
#include <hiredis/hiredis.h>

using namespace std;

class Session {  
  private:
	static Session* instance;
	Session(); 
	void addLog();
	FILE * log_file;
	time_t t;
	redisContext *connection;
	redisReply *reply;
	string session_id;
	
  public:
	~Session();
	static Session* getInstance();
	
	void sessionInit();
	string getCookie();
	bool checkSession();
	string getValue(string key);
	bool setValue(string key,string value);
	bool setValue(string key,int value);
	bool getAllValue(unordered_map<string,string> & result);
	bool setOnline(int user_id);
	bool getOnline(int user_id);
	void destroySession();
};
#endif
