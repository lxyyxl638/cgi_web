#include <string>
#include <iostream>
#include <ctime>
#include "include/database.h"
using namespace std;

int main() {
	
	FILE * file = fopen("/usr/local/nginx/cgibin/log","r+");
	if (file == 0) cout << "WTF" << endl;
	
	fprintf(stderr,"12\n");
	
	Database* db = Database::getInstance();
	string table = "test";
	fprintf(stderr,"13\n");
	string query_1 = "test(id INT,name varchar(5))";
	
	db->dbCreateTable(query_1);
	
	fprintf(stderr,"19\n");
	unordered_map<string,string> query_2;
	query_2["id"] = "1";
	query_2["name"] = "haha";
	
	db->dbInsert(query_2,table);
	
	fprintf(file,"20\n");
	query_2["id"] = "2";
	query_2["name"] = "hh";
	
	db->dbInsert(query_2,table);
	
	string query_3 = "SELECT * FROM test";
	vector<vector<string> > result;

	fprintf(stderr,"35\n");
	db->dbQuery(query_3,result);
	
	for (int i = 0;i < result.size();++i) {
		for (int j = 0;j < result[i].size();++j)
			cout << result[i][j];
		cout << endl;
	}
	
	return 0;
}
