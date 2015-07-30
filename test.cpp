#include <string>
#include <iostream>
#include <ctime>
#include "include/database.h"
using namespace std;

int main() {
	
	//使用之前调用生成一个db类
	Database* db = Database::getInstance();
	
	string query_3 ("select * from users where username = 'yy' and password = '13123' ");


	int rows = db->dbQuery(query_3);
	cout <<rows<<endl;
	
	return 0;
}
