#include<database.h> 

Database::Database() {
	
	connection = mysql_init(NULL);
	
	if (con == NULL) {
		finish_with_error();
		exit(1);
	}

	if (!mysql_real_connection(connection, "localhost","root"," ",NULL,0,NULL,0)) {
		finish_with_error();
		mysql_close(connection);
		exit(1);			
	}
}

Database::~Database() {

	mysql_close(connection);	

}


Database::finish_with_error() {
	
	FILE *logs = fopen("./logs","w+");
	
	time_t t;
	t = time(NULL);
	char * time = ctime(&t);
	
	fprintf(logs, "%s %s\n",time,mysql_error(connection));
	// fprintf(stderr, "%s\n",mysql_error(connection));
}
static Database* Database::getInstance() {

	if (!instance) {
		instance = new Database();
	}
	return instance; 
}

bool Database::dbInsert(unordered_map<string,string> & data,string & table) {

	if (data.empty()) return true;
	
	string query = "INSERT INTO " + table + "(";
	string val_str = ") VALUES (";
	
	for (auto & p : data) {
		query = query + p.first() + ",";
		val_str = val_str + p.second() + ",";
	}
	
	query[query.length() - 1] = '\0';
	val_str[val_str.length() - 1] = '\0';
	query = query + val_str + ")";
	
	if (mysql_query(connection,query)) {
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
	if (mysql_query(con, query)) {      
	      finish_with_error(con);
		  return false;
	  }
	 return true;
}

bool Database::dbQuery(string & query) {
	
	if (query.empty()) return true;
	
	if (mysql_query(con, query)) {      
	      finish_with_error(con);
		  return false;
	  }
	 return true;
}

bool Database::dbQuery(string & query,vector<vector<string> > & result) {
	
	if (query.empty()) return true;
	result.clear();
	
	if (mysql_query(con, query)) {      
	      finish_with_error(con);
		  return false;
	  } else {
	  	
		  MYSQL_RES *res = mysql_store_result(con);
		  int num_fields = mysql_num_fields(res);
		  MYSQL_ROW row;
		  while ((row = mysql_fetch_row(result))) {
			  
			  vector<string> tmp;
			  
		      for(int i = 0; i < num_fields; ++i) { 
		          tmp.push_back(row[i] ? row[i] : "NULL"); 
		      } 
			  result.push_back(tmp);
		  }
	  }
	 return true;
}