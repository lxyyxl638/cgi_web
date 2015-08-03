#include "session.h"
#include "public.h"
using namespace std;

Session* Session::instance = NULL;

void Session::addLog() {
	
	fprintf(log_file,"%s %s\n",reply->str,ctime(&t));
	if (reply->type = REDIS_REPLY_ERROR) {
		const char *hostname = "127.0.0.1";
		int port = 6379;
		struct timeval timeout = { 1, 500000 }; // 1.5 seconds
		connection = redisConnectWithTimeout(hostname, port, timeout);
		if (connection == NULL || connection->err) {
		      if (connection) {
		          fprintf(log_file,"Connection error: %s\n", connection->errstr);
		          redisFree(connection);
		       } else {
		          fprintf(log_file,"Connection error: can't allocate redis context\n");
		       }
		       exit(1);
		 }
	}
}

Session::Session() {
	
	session_id = "";
	const char *hostname = "127.0.0.1";
	int port = 6379;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	
	log_file = fopen("/usr/local/nginx/cgibin/log/redis_logs","a");
	t = time(NULL);
	
	connection = redisConnectWithTimeout(hostname, port, timeout);
	if (connection == NULL || connection->err) {
	      if (connection) {
	          fprintf(log_file,"Connection error: %s\n", connection->errstr);
	          redisFree(connection);
	       } else {
	          fprintf(log_file,"Connection error: can't allocate redis context\n");
	       }
	       exit(1);
	 }
}

Session::~Session() {
	redisFree(connection);
	fclose(log_file);
}

Session* Session::getInstance() {
	
	if (instance == NULL) {
		instance = new Session;
	}
	
	return instance;
}

void Session::sessionInit() {
	session_id = "";
}
string Session::getCookie() {
	
	string cookie = getEnvir("HTTP_COOKIE");
	cookie = (cookie.empty()? session_id : cookie); 
	if (cookie.empty()) {
		string http_user_agent = getEnvir("HTTP_USER_AGENT");
		string remote_host = getEnvir("REMOTE_HOST");
		string remote_addr = getEnvir("REMOTE_ADDR");
		string cookie_time = getCurrentTime();
		string prefix = "name=";
		cookie = prefix + http_user_agent + remote_host + remote_addr + cookie_time;
		int n;
		while ((n=cookie.find(';')) != string::npos) {
			cookie.erase(n,1);
		}
		while ((n=cookie.find(' ')) != string::npos) {
			cookie.erase(n,1);
		}
	}
	
	//重新设置Cookie过期时间或者建立Cookie
	reply = (redisReply *) redisCommand(connection,"HSET session:%s exist %s",cookie.c_str(),"yes");
	addLog();
	freeReplyObject(reply);
	
	reply = (redisReply *) redisCommand(connection,"EXPIRE session:%s %d",cookie.c_str(),3600);
	addLog();
	freeReplyObject(reply);
	//我们的cookie只存储了session_id，因此可以认为两者在值上相等，其实应该加密
	session_id = cookie;
	return cookie;
}


bool Session::checkSession() {
	
	string cookie = getEnvir("HTTP_COOKIE");
	
	if (!cookie.empty()) {
		reply = (redisReply *) redisCommand(connection,"HLEN session:%s",cookie.c_str());
		addLog();
		if (reply->integer > 0) {
			freeReplyObject(reply);
			return true;
		} else {
			freeReplyObject(reply);
			return false;
		}
	}
	return false;
}

string Session::getValue(string key) {
	
	string cookie = getCookie();
	
	reply = (redisReply *) redisCommand(connection,"HGET session:%s %s",cookie.c_str(),key.c_str());
	addLog();
	string result = reply->str;
	freeReplyObject(reply);
	
	return result;
}


bool Session::setValue(string key,string value) {
	
	string cookie = getCookie();
	
	reply = (redisReply *) redisCommand(connection,"HSET session:%s %s %s",cookie.c_str(),key.c_str(),value.c_str());
	addLog();
	freeReplyObject(reply);
	
	return true;
}

bool Session::setValue(string key,int value) {
	
	string cookie = getCookie();
	
	reply = (redisReply *) redisCommand(connection,"HSET session:%s %s %d",cookie.c_str(),key.c_str(),value);
	addLog();
	freeReplyObject(reply);
	
	return true;
}

bool Session::getAllValue(unordered_map<string,string> & result) {
	
	result.clear();
	string cookie = getCookie();
	
	reply = (redisReply *) redisCommand(connection,"HGETALL session:%s",cookie.c_str());
	addLog();
	//if (reply->type == REDIS_REPLY_ARRAY) {
		for (unsigned int i = 0;i < reply->elements;i+=2) {
			result[reply->element[i]->str] = reply->element[i+1]->str;
		}
	//}
	freeReplyObject(reply);
	return true;
}
void Session::destroySession() {
	
	string cookie = getCookie();
	reply = (redisReply *) redisCommand(connection,"DEL session:%s",cookie.c_str());
	addLog();
	freeReplyObject(reply);
	return;
}
