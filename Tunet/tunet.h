/*
 * tunet.h
 *
 *  Created on: Dec 24, 2012
 *      Author: randyzhao
 */

#ifndef TUNET_H_
#define TUNET_H_
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
typedef struct tunet_connection_helper_s{
	int sockfd;
	int current_account_index;
	char** usernames;
	char** password_md5s;
	int user_count;
	struct sockaddr_in servaddr;
} tunet_connection_helper_t;

int tunet_connection_helper_t_init(tunet_connection_helper_t* helper);
/**
 * begin tunet process. login and wait for a period of time and then log out
 * and change an account to do the things again
 */
int tunet_begin();
/*
 * generate a http request header for login purpose. The generated header will be stored in the buffer.
 * When the header is largers than the buffer_size or other error occurs, it will return -1
 * otherwise it will return 0;
 *
 */
int tunet_generate_login_http_request(char* buffer, int buffer_size, char* username, char* password_md5);
int tunet_generate_logout_http_request(char* buffer, int buffer_size);
/**
 * login with a single account
 */
int tunet_login(tunet_connection_helper_t* helper);
/**
 * logout with a single account
 */
int tunet_logout(tunet_connection_helper_t* helper);
#endif /* TUNET_H_ */
