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
	string query_users = "users (\
   user_id int primary key auto_increment not null,\
   username varchar(255) not null,\
   nickname varchar(255) not null,\
   password varchar(255) not null,\
   birthday	 varchar(255) not null,\
   sex int not null default 0,\
   avatar varchar(255) not null,\
   signature varchar(255) not null,\
   state int not null,\
   ip varchar(255) not null,\
   register_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
   last_login_time timestamp not null,\
   back_1 varchar(255) not null,\
   back_2 varchar(255) not null\
) default charset utf8";
   
	db->dbCreateTable(query_users);
	
	string query_friendship = "friendship (\
   id int primary key auto_increment not null,\
   user_id int not null,\
   friend_id int not null,\
   team_id int not null,\
   add_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
   ) default charset utf8";
	   
	db->dbCreateTable(query_friendship);
	
	string query_messages = "p2p_messages (\
   message_id int primary key auto_increment not null,\
   send_id int not null,\
   rece_id int not null,\
   message text not null,\
   send_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
   state int not null default 0\
	 ) default charset utf8";  
   db->dbCreateTable(query_messages);
   
   string query_teams = "teams (\
	   team_id int primary key auto_increment not null,\
   	   user_id int not null,\
	   team_name varchar(255) not null,\
	   created_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
	   team_type int not null,\
	   remark varchar(255) not null\
	   ) default charset utf8";
	db->dbCreateTable(query_teams);
	
	string query_groups = "groups(\
group_id int primary key auto_increment not null,\
group_name varchar(255) not null,\
group_intro varchar(255) not null,\
group_news varchar(255) not null,\
created_user_id int not null,\
created_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
) default charset utf8"; 
	db->dbCreateTable(query_groups);
		
	string query_user_group = "user_group (\
id int primary key auto_increment not null,\
group_id int not null,\
user_id int not null,\
created_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
) default charset utf8";
	db->dbCreateTable(query_user_group);
	
	string query_groups_messages = "group_messages (\
message_id int primary key auto_increment not null,\
group_id int not null,\
message text not null,\
send_id int not null,\
send_time timestamp not null DEFAULT CURRENT_TIMESTAMP\
) default charset utf8";
	db->dbCreateTable(query_groups_messages);
		
	string query_notification = "notification (\
no_id int primary key auto_increment not null,\
send_id int not null,\
rece_id int not null,\
type int not null default 0,\
created_time timestamp not null DEFAULT CURRENT_TIMESTAMP,\
additional_message varchar(255) not null,\
state int not null default 0,\
handling_time timestamp not null \
) default charset utf8";
		
	db->dbCreateTable(query_notification);
	
	return 0;
}
