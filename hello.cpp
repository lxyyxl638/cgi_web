#include <string>
#include <iostream>
#include <ctime>
#include <database.h>
using namespace std;

int main() {
	
	Database* db = Database::getInstance();
	
	string query_1 = "test(id INT,name varchar(5))";
	
	db->dbCreateTable(query_1);
	
	unordered_map<string,string> query_2;
	query_2["id"] = "1";
	query_2["name"] = "haha";
	
	db->dbInsert(query_2);
	
	query_2["id"] = "2";
	query_2["name"] = "hh";
	
	db->dbInsert(query_2);
	
	string query_3 = "SELECT * FROM test";
	vector<vector<string> > result;
	db->dbQuery(query_3,result);
	
	for (int i = 0;i < result.size();++i) {
		for (int j = 0;j < result[i].size();++j)
			cout << result[i][j];
		cout << endl;
	}
	return 0;
}