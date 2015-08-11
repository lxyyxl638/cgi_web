#include "database.h"
#include <iostream>
using namespace std;
Database * Database::instance = 0;

Database::Database() {
	

	fd = open("/usr/local/nginx/cgibin/log/mysql_log", O_WRONLY|O_CREAT|O_APPEND,0666);  
	
	if (fd == -1) {  
	}  

	connection = mysql_init(NULL);
	

	if (connection == NULL) {
		finish_with_error();
	}
	if (!mysql_real_connect(connection, "localhost","root","123456","web_chat",0,NULL,0)) {
		finish_with_error();
		mysql_close(connection);			
	}
	mysql_query(connection,"SET character_set_results=utf8");
	mysql_query(connection,"SET character_set_client=utf8");
	mysql_query(connection,"SET character_set_connection=utf8");
	t = time(NULL);
	
}

Database::~Database() {

	mysql_close(connection);
	close(fd);	
}	


void Database::finish_with_error() {
		
	char buf[512];
	memset(buf,0,sizeof(buf));
	snprintf(buf,sizeof(buf),"%s %s\n",mysql_error(connection),ctime(&t));
	write(fd,buf,strlen(buf));
}

void Database::addLog(string str) {

	char buf[512];
	memset(buf,0,sizeof(buf));
	snprintf(buf,sizeof(buf),"%s %s\n",str.c_str(),ctime(&t));
	write(fd,buf,strlen(buf));
}

Database* Database::getInstance() {

	if (0 == instance) {
		instance = new Database;
	}
	return instance; 
}

bool Database::dbInsert(unordered_map<string,string> & data,string & table) {

	if (data.empty()) return true;
	
	string query = "INSERT INTO " + table + "(";
	string val_str = ") VALUES (";
	
	for (auto & p : data) {
		query = query + p.first + ",";
		val_str = val_str + "\""+ p.second + "\"" + ",";
	}

	query.erase(query.length()-1, query.length()-1);
	val_str.erase(val_str.length()-1,val_str.length()-1);

	query = query + val_str + ")";

	addLog(query);

	if (mysql_query(connection,query.c_str())) {
		finish_with_error();
		return false;
	} else {
		return true;
	}
	return true;
}

bool Database::dbCreateTable(string & query) {
	
	if (query.empty()) return true;
	
	query = "CREATE TABLE " + query;
	
	addLog(query);
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		  return false;
	  }
	 return true;
}

int Database::dbQuery(string & query) {
	
	if (query.empty()) return true;
	
	addLog(query);
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		  return 0;
	  }
	 
	
	 MYSQL_RES *res = mysql_store_result(connection);
	 mysql_free_result(res);

	 return mysql_affected_rows(connection);
}

bool Database::dbQuery(string & query,vector<vector<string> > & result) {
	
	if (query.empty()) return true;
	result.clear();
	
	addLog(query);
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		  return false;
	  } else {
	  	
		  MYSQL_RES *res = mysql_store_result(connection);
		  int num_fields = mysql_num_fields(res);
		  
   		  MYSQL_ROW row;
		  int num = 0;
		  while ((row = mysql_fetch_row(res))) {
			  ++num;
			  vector<string> tmp;
			  
		      for(int i = 0; i < num_fields; ++i) { 
		          tmp.push_back(row[i] ? row[i] : "NULL"); 
		      } 
			  result.push_back(tmp);
		  }
		mysql_free_result(res);
	  }
	 return true;
}


//int Database::dbAggreQuery(string & query) {
/*
  Add on 2015/7/30
*/	
bool Database::dbQuery(string & query,vector<unordered_map<string,string> > & result) {


	if (query.empty()) return true;
	result.clear();
	
	addLog(query);
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		return false;
	  } else {
	  	  unordered_map<string,string> tmp;
		  MYSQL_RES *res = mysql_store_result(connection);
		  int num_fields = mysql_num_fields(res);
		  MYSQL_ROW row;
		  MYSQL_FIELD *fields = mysql_fetch_fields(res);

		  while ((row = mysql_fetch_row(res))) {
			  
		      for(int i = 0; i < num_fields; ++i) { 
		          tmp[fields[i].name] = (row[i] ? row[i] : "NULL"); 
		      } 
		      result.push_back(tmp);
		  }
		  mysql_free_result(res);
	  }
	 return true;
}

