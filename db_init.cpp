#include <string>
#include <iostream>
#include <ctime>
#include "include/database.h"
using namespace std;

int main() {
	
	//使用之前调用生成一个db类
	Database* db = Database::getInstance();	
	cout << "haha" << endl;
	//创建表的语句
	string query_users = "users(\
user_id int primary key auto_increment not null,\
username varchar(255) not null,\
nickname varchar(255) not null,\
password varchar(255) not null,\
birthday varchar(255) not null default \"\",\
sex int not null default 0,\
avatar varchar(255) not null default \"\",\
signature varchar(255) not null default \"\",\
state int not null default 0,\
ip varchar(255) not null default \"\",\
register_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
last_login_time timestamp not null default 0,\
back_1 varchar(255) not null default \"\",\
back_2 varchar(255) not null default \"\"\
) default charset utf8";
   
	db->dbCreateTable(query_users);
//	cout << query_users << endl;	
	string query_friendship = "friendship (\
id int primary key auto_increment not null,\
user_id int not null default 0,\
friend_id int not null default 0,\
team_id int not null default 0,\
add_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
) default charset utf8";
	   
	db->dbCreateTable(query_friendship);
	
	string query_messages = "p2p_messages (\
message_id int primary key auto_increment not null,\
send_id int not null default 0,\
rece_id int not null default 0,\
message text not null,\
send_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
state int not null default 0\
) default charset utf8";  
   db->dbCreateTable(query_messages);
   
   string query_teams = "teams (\
team_id int primary key auto_increment not null,\
user_id int not null default 0,\
team_name varchar(255) not null default \"\",\
created_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
team_type int not null default 0,\
remark varchar(255) not null default \"\"\
) default charset utf8";
	db->dbCreateTable(query_teams);
	
	string query_groups = "groups(\
group_id int primary key auto_increment not null,\
group_name varchar(255) not null default \"\",\
group_intro varchar(255) not null default \"\",\
group_news varchar(255) not null default \"\",\
created_user_id int not null default 0,\
created_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
) default charset utf8"; 
	db->dbCreateTable(query_groups);
		
	string query_user_group = "user_group (\
id int primary key auto_increment not null,\
group_id int not null default 0,\
user_id int not null default 0,\
created_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
) default charset utf8";
	db->dbCreateTable(query_user_group);
	
	string query_groups_messages = "group_messages (\
message_id int primary key auto_increment not null,\
group_id int not null default 0,\
message text not null,\
send_id int not null default 0,\
send_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
) default charset utf8";
	db->dbCreateTable(query_groups_messages);
		
	string query_notification = "notification (\
no_id int primary key auto_increment not null,\
send_id int not null default 0,\
rece_id int not null default 0,\
type int not null default 0,\
created_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
additional_message varchar(255) not null default \"\",\
state int not null default 0,\
handling_time timestamp not null default 0 \
) default charset utf8";
		
	db->dbCreateTable(query_notification);
	
	return 0;
}
