#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <iostream>
#include <hiredis/hiredis.h>
using namespace std;

int main(int argc, char **argv) {
    unsigned int j;
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";
    int port = (argc > 2) ? atoi(argv[2]) : 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING server */
    reply = (redisReply*) redisCommand(c,"PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key */
    reply = (redisReply*) redisCommand(c,"SET %s %s", "foo", "hello world");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key using binary safe API */
    reply = (redisReply*) redisCommand(c,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5);
    printf("SET (binary API): %s\n", reply->str);
    freeReplyObject(reply);

    /* Try a GET and two INCR */
    reply = (redisReply*) redisCommand(c,"GET foo");
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = (redisReply*) redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);
    /* again ... */
    reply = (redisReply*) redisCommand(c,"HSET key a yes");
    printf("HSET key a 1: %lld\n", reply->integer);
    freeReplyObject(reply);

    reply = (redisReply*) redisCommand(c,"HGET key a");
    printf("HGET key a: %lld\n", reply->integer);
    printf("HGET key str a: %s\n", reply->str);
    freeReplyObject(reply);
    

    reply = (redisReply*) redisCommand(c,"HDEL key a");
    printf("HDEL key a: %lld\n", reply->integer);
    printf("HDET key str a: %s\n", reply->str);
    freeReplyObject(reply);

    reply = (redisReply*) redisCommand(c,"HGET key a");

    printf("HGET key a: %lld\n", reply->integer);
    printf("HGET key str a: %s\n", reply->str);
    freeReplyObject(reply);
    /* Create a list of numbers, from 0 to 9 */
    /*
    string str="abc";
    reply = (redisReply*) redisCommand(c,"DEL mist:%s",str.c_str());
    printf("DEL mylist: %lld\n",reply->integer);
    freeReplyObject(reply);
    for (j = 0; j < 10; j++) {
        char buf[64];

        snprintf(buf,64,"%d",j);
        reply = (redisReply*) redisCommand(c,"LPUSH mylist element-%s", buf);
        freeReplyObject(reply);
    }

*/
    /* Disconnects and frees the context */
    redisFree(c);

    return 0;
}
