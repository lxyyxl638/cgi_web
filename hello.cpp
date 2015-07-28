#include <string>
#include <iostream>
#include <ctime>
#include "include/database.h"
using namespace std;

int main() {
	
	Database* db = Database::getInstance();
	string table = "test";
	string query_1 = "test(id INT,name varchar(5))";
	
	db->dbCreateTable(query_1);
	
	unordered_map<string,string> query_2;
	query_2["id"] = "1";
	query_2["name"] = "haha";
	
	db->dbInsert(query_2,table);
	
	query_2["id"] = "2";
	query_2["name"] = "hh";
	
	db->dbInsert(query_2,table);
	
	string query_3 = "SELECT name FROM test";
	vector<vector<string> > result;

	db->dbQuery(query_3,result);
	
	for (int i = 0;i < result.size();++i) {
		for (int j = 0;j < result[i].size();++j)
			cout << result[i][j];
		cout << endl;
	}
	
	cout << "Aggre" << endl;
	string query_4 = "SELECT SUM(id),name from test GROUP BY name";
	db->dbQuery(query_4,result);
	
	for (int i = 0;i < result.size();++i) {
	    for (int j = 0;j < result[i].size();++j)
		  cout << result[i][j];
	     cout << endl;
	}

	return 0;
}
