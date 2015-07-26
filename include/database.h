#include <stdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <mysql/mysql.h>
#include <ctime>
#include <vector>
using namespace std;

class Database {
  
  private:
	static Database * instance = null;
	MYSQL *connection;
	Database(); 
	void finish_with_error();

  public:
	~Database();
	static Database* getInstance();
	bool dbCreateTable(string &);
	bool dbInsert(unordered_map<string,string> & data,string & table);
	bool dbQuery(string &,vector<vector<string> > &);
	bool dbQuery(string &);
}
