#include "database.h"

Database * Database::instance = 0;

Database::Database() {
	
	connection = mysql_init(NULL);
	
		
	if (connection == NULL) {
		finish_with_error();
	}
	fprintf(stderr,"init");
	if (!mysql_real_connect(connection, "localhost","root","123456",NULL,0,NULL,0)) {
		finish_with_error();
		mysql_close(connection);			
	}
	fprintf(stderr,"constructor");
}

Database::~Database() {

	mysql_close(connection);	
}	


void Database::finish_with_error() {
	
//	FILE *logs = fopen("usr/local/nginx/cgibin/logs","r+");
	
	time_t t;
	t = time(NULL);
	char * time = ctime(&t);
	
	fprintf(stderr, "%s %s\n",time,mysql_error(connection));
	// fprintf(stderr, "%s\n",mysql_error(connection));
	//fclose(logs);
}

Database* Database::getInstance() {

	if (0 == instance) {
		fprintf(stderr,"getInstance");
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
		val_str = val_str + p.second + ",";
	}
	
	query[query.length() - 1] = '\0';
	val_str[val_str.length() - 1] = '\0';
	query = query + val_str + ")";
	
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
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		  return false;
	  }
	 return true;
}

bool Database::dbQuery(string & query) {
	
	if (query.empty()) return true;
	
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		  return false;
	  }
	 return true;
}

bool Database::dbQuery(string & query,vector<vector<string> > & result) {
	
	if (query.empty()) return true;
	result.clear();
	
	if (mysql_query(connection, query.c_str())) {      
	      finish_with_error();
		  return false;
	  } else {
	  	
		  MYSQL_RES *res = mysql_store_result(connection);
		  int num_fields = mysql_num_fields(res);
		  MYSQL_ROW row;
		  while ((row = mysql_fetch_row(res))) {
			  
			  vector<string> tmp;
			  
		      for(int i = 0; i < num_fields; ++i) { 
		          tmp.push_back(row[i] ? row[i] : "NULL"); 
		      } 
			  result.push_back(tmp);
		  }
	  }
	 return true;
}
