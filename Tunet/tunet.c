/*
 * tunet.c
 *
 *  Created on: Dec 24, 2012
 *      Author: randyzhao
 *  Forked on: Oct 13, 2015
 *      Author: pengeorge
 */

#include "tunet.h"
#include "http_header.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_HTTP_REQUEST_BUFFER (2000)

int tunet_connection_helper_t_init(tunet_connection_helper_t* helper){
	helper->sockfd = 0;
	helper->current_account_index = 0;
    helper->user_count = 0;
	memset(&helper->servaddr, 0, sizeof(helper->servaddr));
	helper->servaddr.sin_family = AF_INET;
	helper->servaddr.sin_port = htons(SERVER_PORT);
	return 0;
}

#define TUNET_LOGIN_POST_REQUEST_PATH 		("POST /do_login.php HTTP/1.1\r\n")
#define TUNET_USER_INFO_GET_REQUEST_PATH 		("GET /cgi-bin/rad_user_info HTTP/1.1\r\n")
int tunet_generate_login_post_common_header(http_header_t *header) {
	if (http_header_t_init(header)) {
		return -1;
	}
	http_header_set_request_path(header, TUNET_LOGIN_POST_REQUEST_PATH);
	//set http fields
	http_header_set_request_type(header, HTTP_REQUEST_TYPE_POST);
	http_header_add_request_field(header, "Host", "net.tsinghua.edu.cn");
	http_header_add_request_field(header, "Connection", "keep-alive");
	http_header_add_request_field(header, "Accept", "*/*");
	//http_header_add_request_field(header, "Pragma", "no-cache");
	//http_header_add_request_field(header, "Cache-Control", "no-cache");
	http_header_add_request_field(header, "Origin", "http://net.tsinghua.edu.cn");
	//http_header_add_request_field(header, "X-Requested-With", "XMLHttpRequest");
	http_header_add_request_field(header, "User-Agent", "Linux");
	http_header_add_request_field(header, "Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	http_header_add_request_field(header, "Referer", "http://net.tsinghua.edu.cn/wired/");
	http_header_add_request_field(header, "Accept-Encoding", "gzip,deflate");
	http_header_add_request_field(header, "Accept-Language", "en-US,en;q=0.8,zh-CN;q=0.6");
	//http_header_add_request_field(header, "Accept-Charset",	"ISO-8859-1,utf-8;q=0.7,*;q=0.3");
}


//A login http get fild looks like here (Oct 13, 2015)
//the request header will be generated line by line in this function

//POST /do_login.php HTTP/1.1
//Host: net.tsinghua.edu.cn
//Connection: keep-alive
//Content-Length: 90
//Origin: http://net.tsinghua.edu.cn
//User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97 Safari/537.11
//Content-Type: application/x-www-form-urlencoded
//Accept: */*
//Referer: http://net.tsinghua.edu.cn/wired/
//Accept-Encoding: gzip,deflate,sdch
//Accept-Language: en-US,en;q=0.8
//Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3
//Cookie: thuwebcookie=XXXXXXXXXXX
//HTML Form URL Encoded:
//action=login&username=zhao-hz09&password={MD5_HEX}2826d33b5997f841733593c3a153c929&ac_id=1
int tunet_generate_login_http_request(char* buffer, int buffer_size,
		char* username, char* password_md5) {
	http_header_t* header = (http_header_t*) malloc(sizeof(http_header_t));
    tunet_generate_login_post_common_header(header);
	http_header_add_request_field(header, "Content-Length", "90");
	http_header_add_post_field(header, "action", "login");
	http_header_add_post_field(header, "username", username);
	http_header_add_post_field(header, "password", password_md5);
	http_header_add_post_field(header, "ac_id", "1");

	//output the header
	http_header_to_text(header, buffer, buffer_size);
	return 0;
}
//POST /do_login HTTP/1.1
//Host: net.tsinghua.edu.cn
//Connection: keep-alive
//Content-Length: 13
//Origin: http://net.tsinghua.edu.cn
//User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97 Safari/537.11
//Content-Type: application/x-www-form-urlencoded
//Accept: */*
//Referer: http://net.tsinghua.edu.cn/wired/succeed.html?online
//Accept-Encoding: gzip,deflate,sdch
//Accept-Language: en-US,en;q=0.8
//Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3
//Cookie: thuwebcookie=XXXXXXXX
//HTML Form URL Encoded:
//action=logout
int tunet_generate_logout_http_request(char* buffer, int buffer_size){
	http_header_t* header = (http_header_t*) malloc(sizeof(http_header_t));
    tunet_generate_login_post_common_header(header);
	http_header_add_request_field(header, "Content-Length", "13");
	http_header_add_post_field(header, "action", "logout");

	//output the header
	http_header_to_text(header, buffer, buffer_size);
	return 0;
}

int tunet_generate_status_http_request(char* buffer, int buffer_size){
	http_header_t* header = (http_header_t*) malloc(sizeof(http_header_t));
    tunet_generate_login_post_common_header(header);
	http_header_add_request_field(header, "Content-Length", "19");
	http_header_add_post_field(header, "action", "check_online");

	//output the header
	http_header_to_text(header, buffer, buffer_size);
	return 0;
}

int tunet_generate_info_http_request(char* buffer, int buffer_size){
	http_header_t* header = (http_header_t*) malloc(sizeof(http_header_t));
	if (http_header_t_init(header)) {
		return -1;
	}
	http_header_set_request_path(header, TUNET_USER_INFO_GET_REQUEST_PATH);
	//set http fields
	http_header_set_request_type(header, HTTP_REQUEST_TYPE_GET);
	http_header_add_request_field(header, "Host", "166.111.204.120");
	http_header_add_request_field(header, "Connection", "keep-alive");
	http_header_add_request_field(header, "Pragma", "no-cache");
	http_header_add_request_field(header, "Cache-Control", "no-cache");
	http_header_add_request_field(header, "Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	http_header_add_request_field(header, "Upgrade-Insecure-Requests", "1");
	http_header_add_request_field(header, "User-Agent", "Linux");
	http_header_add_request_field(header, "Accept-Encoding", "gzip,deflate,sdch");
	http_header_add_request_field(header, "Accept-Language", "en-US,en;q=0.8,zh-CN;q=0.6");
	http_header_add_request_field(header, "Accept-Charset",	"GBK,utf-8;q=0.7,*;q=0.3");

	//output the header
	http_header_to_text(header, buffer, buffer_size);
	return 0;
}

int tunet_do_request(struct sockaddr_in *p_servaddr, char *header_buffer, char **p_read_buffer, char *action) {
	int sockfd;
	int bytes_read;
    char *read_buffer = *p_read_buffer;
	memset(read_buffer, 0, sizeof(read_buffer));
#ifdef DEBUG
	printf("Request header:\n---\n%s\n---\n\n", header_buffer);
#endif
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		printf("[tunet_%s: create socket error\n", action);
		return -1;
	}
	if (connect(sockfd, (struct sockaddr *) p_servaddr,
			sizeof(struct sockaddr))) {
		printf("[tunet_%s]: connect to server error\n", action);
		return -1;
	}
	if (send(sockfd, header_buffer, strlen(header_buffer), 0) == -1) {
		printf("[tunet_%s]: send %s request error\n", action, action);
		return -1;
	}
	struct timeval tv;
	tv.tv_sec = 5;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	bytes_read = recv(sockfd, read_buffer, MAX_HTTP_REQUEST_BUFFER, 0);
	close(sockfd);
	if (bytes_read < 0) {
        perror("recv error: ");
        return bytes_read;
    }
#ifdef DEBUG
    printf("Response:\n---\n%s\n---\n\n", read_buffer);
#endif
    // Get code
    char code[4];
    strncpy(code, read_buffer + 9, 3);
    code[3] = 0;
    if (strcmp(code, "200")) {
        printf("[tunet_%s]: failed. Response code: %s\n", action, code);
        return 0;
    }
#ifdef DEBUG
  	printf("[tunet_%s]: %s ok!\n", action, action);
#endif
    // Get body text
    char *ptext = strstr(read_buffer, "\r\n\r\n");
    if (ptext == NULL) {
        printf("[tunet_%s]: no info returned.\n", action);
        return 0;
    }
    ptext += 4;
#ifdef DEBUG
    printf("Response body:\n---\n%s\n---\n\n", ptext);
#endif
    *p_read_buffer = ptext;
	return strlen(ptext);
}

int tunet_status(tunet_connection_helper_t* helper){
	printf("[tunet_status]: check status\n");
	char header_buffer[MAX_HTTP_REQUEST_BUFFER];
	char read_buffer[MAX_HTTP_REQUEST_BUFFER];
	memset(header_buffer, 0, sizeof(header_buffer));
	if (tunet_generate_status_http_request(header_buffer, MAX_HTTP_REQUEST_BUFFER)) {
		printf("[tunet_status]: generate http request error\n");
		return -1;
	}
    char* text = read_buffer;
    int res = tunet_do_request(&helper->servaddr, header_buffer, &text, "status");
    if (res <= 0) return -1;
    printf("[tunet_status]: %s\n", text);
    if (strstr(text, "not_online")) { //not online
        return 0;
    } else if (strstr(text, "online")) { // online
        return 1;
    } else {
        return -1;
    }
}

int do_tunet_login(tunet_connection_helper_t* helper) {
	char* username = helper->usernames[helper->current_account_index];
	char* password_md5 = helper->password_md5s[helper->current_account_index];
	printf("[tunet_login]: login with username %s\n", username);
	char header_buffer[MAX_HTTP_REQUEST_BUFFER];
	char read_buffer[MAX_HTTP_REQUEST_BUFFER];
	memset(header_buffer, 0, sizeof(header_buffer));
	if (tunet_generate_login_http_request(header_buffer, MAX_HTTP_REQUEST_BUFFER,
			username, password_md5)) {
		printf("[tunet_login]: generate http request error\n");
		return -1;
	}
    char* text = read_buffer;
    int res = tunet_do_request(&helper->servaddr, header_buffer, &text, "login");
    if (res <= 0) return -1;
    if (strstr(text, "Login is successful")) { // success
        do_tunet_getinfo(helper);
    }
    printf("[tunet_login]: %s\n", text);
    return 0;        
}

int tunet_login(tunet_connection_helper_t* helper) {
    if (tunet_status(helper)) {
        return 1;
    }
    return do_tunet_login(helper);
}

int do_tunet_logout(tunet_connection_helper_t* helper){
	printf("[tunet_logout]: logout user %s\n", helper->usernames[helper->current_account_index]);
	char header_buffer[MAX_HTTP_REQUEST_BUFFER];
	char read_buffer[MAX_HTTP_REQUEST_BUFFER];
	memset(header_buffer, 0, sizeof(header_buffer));
	if (tunet_generate_logout_http_request(header_buffer, MAX_HTTP_REQUEST_BUFFER)) {
		printf("[tunet_logout]: generate http request error\n");
		return -1;
	}
    char* text = read_buffer;
    int res = tunet_do_request(&helper->servaddr, header_buffer, &text, "logout");
    if (res <= 0) return -1;
    if (strstr(text, "Logout is successful")) { // success

    }
    printf("[tunet_logout]: %s\n", text);
    return 0;        
}

int tunet_logout(tunet_connection_helper_t* helper) {
    if (!tunet_status(helper)) {
        return 1;
    }
    return do_tunet_logout(helper);
}

int do_tunet_getinfo(tunet_connection_helper_t* helper){
	printf("[tunet_info]: get login info\n");
	char header_buffer[MAX_HTTP_REQUEST_BUFFER];
	char read_buffer[MAX_HTTP_REQUEST_BUFFER];
	memset(header_buffer, 0, sizeof(header_buffer));
	if (tunet_generate_info_http_request(header_buffer, MAX_HTTP_REQUEST_BUFFER)) {
		printf("[tunet_info]: generate http request error\n");
		return -1;
	}
    char* text = read_buffer;
    int res = tunet_do_request(&helper->servaddr, header_buffer, &text, "info");
    if (res <= 0) return -1;
    char *tmp_text = strstr(text, "\r\n");
    if (tmp_text) {
        text = tmp_text + 2;
    } else {
        //printf("[tunet_info]: probably unknown response body:---\n%s\n---\n", text);
    }
    char *p;
    printf("[username] %s\n", strtok(text, ","));
    int id = 0;
    int loginTime;
    while (p = strtok(NULL, ",")) {
        id++;
        switch (id) {
            case 1:
                loginTime = atoi(p);
                break;
            case 2:
                printf("[time elapsed] %d seconds\n", atoi(p) - loginTime);
                break;
            case 6:
                printf("[traffic] %.2f GB\n", atof(p) / 1024 / 1024 / 1024);
                break;
            case 8:
                printf("[ip] %s\n", p);
                break;
            case 10: // empty string is skipped
                printf("[balance] %.2f yuan\n", atof(p));
                break;
        }
    }
    return 0; 
}

int tunet_info(tunet_connection_helper_t* helper) {
    if (!tunet_status(helper)) {
        return 1;
    }
    return do_tunet_getinfo(helper);
}

int tunet_do(tunet_connection_helper_t *helper, char *action){
    if (strcmp(action, "login") == 0) {
        tunet_login(helper);
    } else if (strcmp(action, "logout") == 0) {
        tunet_logout(helper);
    } else if (strcmp(action, "status") == 0) {
        tunet_status(helper);
    } else if (strcmp(action, "info") == 0) {
        tunet_info(helper);
    } else {
        return 1;
    }
	return 0;
}

// Multiple users, not used.
#define CONNECTION_TIME (30)
#define LOGOUT_WAIT_TIME (25)
int tunet_begin(tunet_connection_helper_t* helper) {
	for (;;){
		tunet_login(helper);
		printf("[tunet_begin]: sleep for %d seconds\n]", CONNECTION_TIME);
		sleep(CONNECTION_TIME);
		tunet_logout(helper);
		helper->current_account_index = (helper->current_account_index + 1) % helper->user_count;
		printf("[tunet_begin]: sleep for %d seconds\n]", LOGOUT_WAIT_TIME);
		sleep(LOGOUT_WAIT_TIME);
	}
	return 0;
}

