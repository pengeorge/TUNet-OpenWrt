/*
 * tunet.c
 *
 *  Created on: Dec 24, 2012
 *      Author: randyzhao
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

#define SERVER_IP ("166.111.8.120")
char* username1 = "zhao-hz09";
char* username2 = "zhao-hz09";
char* password_md5s1 = "2826d33b5997f841733593c3a153c929";
char* password_md5s2 = "2826d33b5997f841733593c3a153c929";
int user_count = 2;

int tunet_connection_helper_t_init(tunet_connection_helper_t* helper){
	helper->sockfd = 0;
	helper->current_account_index = 0;
	helper->user_count = user_count;
	helper->usernames = (char**)malloc(sizeof(char**) * helper->user_count);
	helper->password_md5s = (char**)malloc(sizeof(char**) * helper->user_count);
	helper->usernames[0] = username1;
	helper->usernames[1] = username2;
	helper->password_md5s[0] = password_md5s1;
	helper->password_md5s[1] = password_md5s2;
	memset(&helper->servaddr, 0, sizeof(helper->servaddr));
	helper->servaddr.sin_family = AF_INET;
	helper->servaddr.sin_port = htons(80);
	inet_pton(AF_INET, SERVER_IP, &helper->servaddr.sin_addr);
	return 0;
}

#define TUNET_LOGIN_POST_REQUEST_PATH 		("POST /cgi-bin/do_login HTTP/1.1\r\n")
#define TUNET_LOGOUT_POST_REQUEST_PATH 		("POST /cgi-bin/do_logout HTTP/1.1\r\n")
//A login http get fild looks like here
//the request header will be generated line by line in this function

//POST /cgi-bin/do_login HTTP/1.1
//Host: net.tsinghua.edu.cn
//Connection: keep-alive
//Content-Length: 80
//Origin: http://net.tsinghua.edu.cn
//User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97 Safari/537.11
//Content-Type: application/x-www-form-urlencoded
//Accept: */*
//Referer: http://net.tsinghua.edu.cn/wired/?url=http://www.renren.com
//Accept-Encoding: gzip,deflate,sdch
//Accept-Language: en-US,en;q=0.8
//Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3
//Cookie: __utma=69804565.720270301.1354198205.1354198205.1354240374.2; __utmz=69804565.1354240374.2.2.utmcsr=google|utmccn=(organic)|utmcmd=organic|utmctr=(not%20provided); tunet=zhao-hz09%0A183040
//username=zhao-hz09&password=2826d33b5997f841733593c3a153c929&drop=0&type=1&n=100
int tunet_generate_login_http_request(char* buffer, int buffer_size,
		char* username, char* password_md5) {
	//TODO:
	http_header_t* header = (http_header_t*) malloc(sizeof(http_header_t));
	if (http_header_t_init(header)) {
		return -1;
	}
	http_header_set_request_path(header, TUNET_LOGIN_POST_REQUEST_PATH);
	//set http fields
	http_header_set_request_type(header, HTTP_REQUEST_TYPE_POST);
	http_header_add_request_field(header, "Host", "net.tsinghua.edu.cn");
	http_header_add_request_field(header, "Connection", "keep-alive");
	http_header_add_request_field(header, "Content-Length", "80");
	http_header_add_request_field(header, "Origin",
			"http://net.tsinghua.edu.cn");
	http_header_add_request_field(header, "User-Agent", "Hongze's code");
	http_header_add_request_field(header, "Accept", "*/*");
	http_header_add_request_field(header, "Referer",
			"http://net.tsinghua.edu.cn/wired/?url=http://www.renren.com");
	http_header_add_request_field(header, "Accept-Encoding",
			"gzip,deflate,sdch");
	http_header_add_request_field(header, "Accept-Language", "en-US,en;q=0.8");
	http_header_add_request_field(header, "Accept-Charset",
			"ISO-8859-1,utf-8;q=0.7,*;q=0.3");
	http_header_add_post_field(header, "username", username);
	http_header_add_post_field(header, "password", password_md5);
	http_header_add_post_field(header, "drop", "0");
	http_header_add_post_field(header, "type", "1");
	http_header_add_post_field(header, "n", "100");

	//output the header
	http_header_to_text(header, buffer, buffer_size);
	return 0;
}
//POST /cgi-bin/do_logout HTTP/1.1
//Host: net.tsinghua.edu.cn
//Connection: keep-alive
//Content-Length: 0
//Origin: http://net.tsinghua.edu.cn
//User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97 Safari/537.11
//Content-Type: application/x-www-form-urlencoded
//Accept: */*
//Referer: http://net.tsinghua.edu.cn/wired/succeed.html?418016282062403,zhao-hz09,9354250480,0,1289
//Accept-Encoding: gzip,deflate,sdch
//Accept-Language: en-US,en;q=0.8
//Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3
//Cookie: tunet=zhao-hz09%0A183040
int tunet_generate_logout_http_request(char* buffer, int buffer_size){
	http_header_t* header = (http_header_t*) malloc(sizeof(http_header_t));
	if (http_header_t_init(header)) {
		return -1;
	}
	http_header_set_request_path(header, TUNET_LOGOUT_POST_REQUEST_PATH);
	//set http fields
	http_header_set_request_type(header, HTTP_REQUEST_TYPE_POST);
	http_header_add_request_field(header, "Host", "net.tsinghua.edu.cn");
	http_header_add_request_field(header, "Connection", "keep-alive");
	http_header_add_request_field(header, "Content-Length", "0");
	http_header_add_request_field(header, "Origin",
			"http://net.tsinghua.edu.cn");
	http_header_add_request_field(header, "User-Agent", "Hongze's code");
	http_header_add_request_field(header, "Accept", "*/*");
	http_header_add_request_field(header, "Referer",
			"http://net.tsinghua.edu.cn/wired/?url=http://www.renren.com");
	http_header_add_request_field(header, "Accept-Encoding",
			"gzip,deflate,sdch");
	http_header_add_request_field(header, "Accept-Language", "en-US,en;q=0.8");
	http_header_add_request_field(header, "Accept-Charset",
			"ISO-8859-1,utf-8;q=0.7,*;q=0.3");

	//output the header
	http_header_to_text(header, buffer, buffer_size);
	return 0;
}
int tunet_login(tunet_connection_helper_t* helper) {
	char* username = helper->usernames[helper->current_account_index];
	char* password_md5 = helper->password_md5s[helper->current_account_index];
	printf("[tunet_login]: login with username %s\n", username);
	char buffer[MAX_HTTP_REQUEST_BUFFER];
	char read_buffer[MAX_HTTP_REQUEST_BUFFER];
	int sockfd;
	int bytes_read;
	memset(buffer, 0, sizeof(buffer));
	memset(read_buffer, 0, sizeof(read_buffer));
	if (tunet_generate_login_http_request(buffer, MAX_HTTP_REQUEST_BUFFER,
			username, password_md5)) {
		printf("[tunet_login]: generate http request error\n");
		return -1;
	}
	printf("request header is %s\n", buffer);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		printf("[tunet_login]: create socket error\n");
		return -1;
	}
	if (connect(sockfd, (struct sockaddr *) &helper->servaddr,
			sizeof(struct sockaddr))) {
		printf("[tunet_login]: connect to server error\n");
		return -1;
	}
	if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
		printf("[tunet_login]: send login request error\n");
		return -1;
	}
	struct timeval tv;
	tv.tv_sec = 5;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	bytes_read = recv(sockfd, read_buffer, MAX_HTTP_REQUEST_BUFFER, 0);
	if (bytes_read > 0) {
		printf("read bytes: %s\n", read_buffer);
	}
	memset(read_buffer, 0, sizeof(read_buffer));

	printf("do not receive more data\n");
	close(sockfd);
	printf("[tunet_login]: login ok!\n");
	return 0;
}


int tunet_logout(tunet_connection_helper_t* helper){
	printf("[tunet_logout]: logout user %s\n", helper->usernames[helper->current_account_index]);
	char buffer[MAX_HTTP_REQUEST_BUFFER];
	char read_buffer[MAX_HTTP_REQUEST_BUFFER];
	int sockfd;
	int bytes_read;
	memset(buffer, 0, sizeof(buffer));
	memset(read_buffer, 0, sizeof(read_buffer));
	if (tunet_generate_logout_http_request(buffer, MAX_HTTP_REQUEST_BUFFER)) {
		printf("[tunet_logout]: generate http request error\n");
		return -1;
	}
	printf("request header is %s\n", buffer);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		printf("[tunet_logout]: create socket error\n");
		return -1;
	}
	if (connect(sockfd, (struct sockaddr *) &helper->servaddr,
			sizeof(struct sockaddr))) {
		printf("[tunet_logout]: connect to server error\n");
		return -1;
	}
	if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
		printf("[tunet_logout]: send logout request error\n");
		return -1;
	}
	struct timeval tv;
	tv.tv_sec = 5;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	bytes_read = recv(sockfd, read_buffer, MAX_HTTP_REQUEST_BUFFER, 0);
	if (bytes_read > 0) {
		printf("read bytes: %s\n", read_buffer);
	}
	memset(read_buffer, 0, sizeof(read_buffer));

	printf("do not receive more data\n");
	close(sockfd);
	printf("[tunet_logout]: logout ok!\n");
	return 0;
}

#define CONNECTION_TIME (30)
#define LOGOUT_WAIT_TIME (25)
int tunet_begin(){
	tunet_connection_helper_t* helper =
			(tunet_connection_helper_t*)malloc(sizeof(tunet_connection_helper_t));
	tunet_connection_helper_t_init(helper);
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
