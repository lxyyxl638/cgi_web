#include "session.h"
#include "public.h"
using namespace std;

Session* Session::instance = NULL;

void Session::addLog() {
	
	char buf[512];
	memset(buf,0,sizeof(buf));
	snprintf(buf,sizeof(buf),"%s ",reply->str);
	snprintf(buf,sizeof(buf),"%lld %s\n",reply->integer,ctime(&t));
	write(fd,buf,strlen(buf));
	   
	if (reply->type = REDIS_REPLY_ERROR) {
		const char *hostname = "127.0.0.1";
		int port = 6379;
		struct timeval timeout = { 1, 500000 }; // 1.5 seconds
		connection = redisConnectWithTimeout(hostname, port, timeout);
		if (connection == NULL || connection->err) {
		      if (connection) {
		          redisFree(connection);
		       } else {
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
	
	fd = open("/usr/local/nginx/cgibin/log/redis_logs",O_WRONLY|O_CREAT|O_APPEND,0666);
	t = time(NULL);
	
	connection = redisConnectWithTimeout(hostname, port, timeout);
	if (connection == NULL || connection->err) {
	      if (connection) {
	          redisFree(connection);
	       } else {
	       }
	       exit(1);
	 }
}

Session::~Session() {
	redisFree(connection);
	close(fd);
}

Session* Session::getInstance() {
	
	if (instance == NULL) {
		instance = new Session;
	}
	
	return instance;
}

void Session::sessionInit() {
	
	session_id = getEnvir("HTTP_COOKIE");
}
string Session::getCookie() {

		string cookie ="";
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
	
	//重新设置Cookie过期时间或者建立Cookie
	string order = "HSET session:"+cookie+" exist yes";
	write(fd,order.c_str(),strlen(order.c_str()));
	reply = (redisReply *) redisCommand(connection,order.c_str());
	addLog();
	freeReplyObject(reply);
	
	order = "EXPIRE session:"+cookie+" 3600";
	write(fd,order.c_str(),strlen(order.c_str()));
	reply = (redisReply *) redisCommand(connection,order.c_str());
	addLog();
	freeReplyObject(reply);
	//我们的cookie只存储了session_id，因此可以认为两者在值上相等，其实应该加密
	session_id = cookie;
	return cookie;
}


bool Session::checkSession() {
	
	string cookie = getEnvir("HTTP_COOKIE");
	string order;
	if (!cookie.empty()) {
		order = "HLEN session:"+cookie;
		write(fd,order.c_str(),strlen(order.c_str()));
		reply = (redisReply *) redisCommand(connection,order.c_str());
		addLog();
		if (reply->integer > 0) {
			freeReplyObject(reply);

			instance->setOnline(atoi(instance->getValue("user_id").c_str()));
			

			order = "EXPIRE session:"+cookie+" 3600";
			write(fd,order.c_str(),strlen(order.c_str()));
			reply = (redisReply *) redisCommand(connection,order.c_str());
			addLog();
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
	
	string cookie = getEnvir("HTTP_COOKIE");
	string order = "HGET session:"+cookie+" "+key;
	write(fd,order.c_str(),strlen(order.c_str()));
	reply = (redisReply *) redisCommand(connection,order.c_str());
	addLog();
	string result = reply->str;
	freeReplyObject(reply);
	
	return result;
}


bool Session::setValue(string key,string value) {
	
	string cookie = session_id;
	string order = "HSET session:"+cookie+" "+key+" "+value;
	write(fd,order.c_str(),strlen(order.c_str()));
	reply = (redisReply *) redisCommand(connection,order.c_str());
	addLog();
	freeReplyObject(reply);
	
	return true;
}

bool Session::setValue(string key,int value) {
	
	string cookie = getEnvir("HTTP_COOKIE");
	
	reply = (redisReply *) redisCommand(connection,"HSET session:%s %s %d",cookie.c_str(),key.c_str(),value);
	addLog();
	freeReplyObject(reply);
	
	return true;
}

bool Session::getAllValue(unordered_map<string,string> & result) {
	
	result.clear();

	string cookie = getEnvir("HTTP_COOKIE");
	string order="HGETALL session:" + cookie;
	write(fd,order.c_str(),strlen(order.c_str()));
	reply = (redisReply *) redisCommand(connection,order.c_str());
	addLog();
	//if (reply->type == REDIS_REPLY_ARRAY) {
		for (unsigned int i = 0;i < reply->elements;i+=2) {
			result[reply->element[i]->str] = reply->element[i+1]->str;
		}
	//}
	freeReplyObject(reply);
	return true;
}


bool Session::setOnline(int user_id) {

	string order;

	reply = (redisReply *) redisCommand(connection,"SET online:%d 1",user_id);
	addLog();
	freeReplyObject(reply);

	reply = (redisReply *) redisCommand(connection,"EXPIRE online:%d 3600",user_id);
	addLog();
	freeReplyObject(reply);
	
	return true;
}

bool Session::getOnline(int user_id) {

	bool result;
	reply = (redisReply *) redisCommand(connection,"EXISTS online:%d",user_id);
	addLog();
	if (reply->integer == 0) result = false;
	else result = true;

	freeReplyObject(reply);
	return result;
}

void Session::destroySession() {
		

	string cookie = getEnvir("HTTP_COOKIE");
	int user_id = atoi(instance->getValue("user_id").c_str());
	
	string order = "DEL session:" + cookie;
	reply = (redisReply *) redisCommand(connection,order.c_str());
	addLog();
	write(fd,order.c_str(),strlen(order.c_str()));
	freeReplyObject(reply);
	

	reply = (redisReply *) redisCommand(connection,"DEL online:%d",user_id);
	addLog();
	freeReplyObject(reply);
	
	return;
}
