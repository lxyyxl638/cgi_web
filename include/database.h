#include <stdlib>
#include <cstdio>
#include <cstring>
#include <mysql/mysql.h>

using namespace std;

class Database {
  
  private:
	static Database * instance = null;
	MYSQL *connection;
	Database(); 
	void finish_with_error() {
		fprintf(stderr, "%s\n",mysql_error(connection));
	}

  public:
	~Database();
	static Database* getInstance();
	bool dbcreateTable(string &);
	bool dbinsert(string[],string[]);
	bool dbdelete()

}
