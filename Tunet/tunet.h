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

#define MAX_USER_COUNT (8)
#define MAX_NAME_LENGTH (64)
#define MD5_LENGTH (32)

// Default server ip and port
#define SERVER_IP ("166.111.204.120")
#define SERVER_PORT (80)

typedef enum{NORMAL} Mode;
typedef struct tunet_connection_helper_s{
	int sockfd;
	int current_account_index;
	char usernames[MAX_USER_COUNT][MAX_NAME_LENGTH];
	char password_md5s[MAX_USER_COUNT][MD5_LENGTH + 9]; // plus the length of "{MD5_HEX}"
	int user_count;
	struct sockaddr_in servaddr;
    Mode mode;
} tunet_connection_helper_t;

int tunet_connection_helper_t_init(tunet_connection_helper_t* helper);
/**
 * begin tunet process. login and wait for a period of time and then log out
 * and change an account to do the things again
 */
int tunet_begin();

int do_login();
int do_logout();

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
