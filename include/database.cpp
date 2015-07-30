#include "database.h"
#include <iostream>
using namespace std;
Database * Database::instance = 0;

Database::Database() {
	
	connection = mysql_init(NULL);
			
	if (connection == NULL) {
		finish_with_error();
	}
	if (!mysql_real_connect(connection, "localhost","root","7553629","web_chat",0,NULL,0)) {
		finish_with_error();
		mysql_close(connection);			
	}
	log_file = fopen("/usr/local/nginx/cgibin/logs","a");
	if (log_file == 0) fprintf(stderr,"WTF");
	t = time(NULL);
	
}

Database::~Database() {

	mysql_close(connection);
	fclose(log_file);	
}	


void Database::finish_with_error() {
		
	//fprintf(log_file,"%s %s\n",time,mysql_error(connection));	
	fprintf(log_file,"%s %s\n",mysql_error(connection),ctime(&t));	
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

	fprintf(log_file,"%s %s",query.c_str(),ctime(&t));

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
	fprintf(log_file,"%s %s",query.c_str(),ctime(&t));
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		  return false;
	  }
	 return true;
}

int Database::dbQuery(string & query) {
	
	if (query.empty()) return true;
	

	fprintf(log_file,"%s %s",query.c_str(),ctime(&t));
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
	

	fprintf(log_file,"%s %s",query.c_str(),ctime(&t));
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
	
	fprintf(log_file,"%s %s",query.c_str(),ctime(&t));
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
	  }
	 return true;
}

