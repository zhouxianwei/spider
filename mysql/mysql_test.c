#include <stdio.h>
#include <mysql.h>

void main(){
    MYSQL mysql, *sock;
    mysql_init(&mysql);
    sock = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "spider", 3306, NULL, 0);

    mysql_close(sock);
}
