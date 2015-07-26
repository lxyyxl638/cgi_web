#include<database.h> 

Database::Database() {
	
	connection = mysql_init(NULL);
	
	if (con == NULL) {
		fprintf(stderr, "%s\n", mysql_error(connection));
		exit(1);
	}

	if (!mysql_real_connection(connection, "localhost","root"," ",NULL,0,NULL,0)) {
		fprintf(stderr, "%s\n", mysql_error(connection));
		mysql_close(connection);
		exit(1);			
	}
}

Database::~Database() {

	mysql_close(connection);	

}


static Database* Database::getInstance() {

	if (!instance) {
		instance = new Database();
	}
	return instance; 
}

bool Database::dbinsert(string field[],string value[], string & table,int num) {

	string query = "INSERT INTO " + table + "(";
	for (int i = 0;i < num - 1;++i)
		query = query + field[i] + ",";
	
	query = query + field[num - 1] + ") VALUES("; 
	
	for (int i = 0;i < num - 1;++i)
		query = query + value[i] + ",";
	
	query = query + value[num - 1] + ")";

	if (mysql_query(connection,query)) {
		finish_with_error();
		return false;
	} else {
		return true;
	}
	return true;
}
