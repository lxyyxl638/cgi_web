#include <string>
#include <iostream>
#include <ctime>
#include "include/database.h"
using namespace std;

int main() {
	
	//使用之前调用生成一个db类
	Database* db = Database::getInstance();
	//选定数据表
	string table = "test";
	
	//创建表的语句
	string query_1 = "test(id INT,name varchar(5))";
	db->dbCreateTable(query_1);
	
	//插入语句，先声明一个unordered_map<string,string>存储插入数据
	unordered_map<string,string> query_2;
	query_2["id"] = "1";
	query_2["name"] = "插入语句";
	
	db->dbInsert(query_2,table);
	
	query_2["id"] = "2";
	query_2["name"] = "hh";
	
	db->dbInsert(query_2,table);
	//查询语句，要先声明一个二维vector存储结果
	string query_3 = "SELECT name FROM test";
	vector<vector<string> > result;

	db->dbQuery(query_3,result);
	
	for (int i = 0;i < result.size();++i) {
		for (int j = 0;j < result[i].size();++j)
			cout << result[i][j];
		cout << endl;
	}
	
	string query_4 = "SELECT SUM(id),name from test GROUP BY name";
	db->dbQuery(query_4,result);
	
	for (int i = 0;i < result.size();++i) {
	    for (int j = 0;j < result[i].size();++j)
		  cout << result[i][j];
	     cout << endl;
	}
	
	string query_5 = "SELECT id,name from test";
	vector<unordered_map<string,string> > result_map;
	db->dbQuery(query_5,result_map);
	cout << "This API is " << endl;
	for (int i = 0;i < result_map.size();++i) {
	      cout << result_map[i]["name"] << " ";
	      cout << result_map[i]["id"] << endl;
	}
	
	string query_6 = "select * from test";
	cout << "num is" << db->dbQuery(query_6) << endl;
	return 0;
}
