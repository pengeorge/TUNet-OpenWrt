/*
 * main.c
 *
 *  Created on: Dec 24, 2012
 *      Author: randyzhao
 */
#include <stdio.h>
#include <stdlib.h>

#include "http_header.h"
#include "tunet.h"
#include "md5.h"


#define SUSUO b6b2a596d787b1c647cf96ca04c3fc85

#define MAX_LINE_LENGTH (4096)
#define MAX_USERNAME (10)
#define MAX_PASSWORD (64)
#define MD5_LENGTH (128)

int parse_input(char* path);
int main(int argc, char *argv[]){
/*
	if (argc < 2){
		printf("usage: randy_tunet <configure_path>\n");
		return 0;
	}
	if (!parse_input(argv[1])){
		printf("[main]: parse_input failed\n");
		return 0;
	}*/

	//http_header_test();
	tunet_connection_helper_t* helper =
			(tunet_connection_helper_t*)malloc(sizeof(tunet_connection_helper_t));
	//tunet_connection_helper_t_init(helper);
	//tunet_login(helper);
	//tunet_logout(helper);
	tunet_begin();
	return 0;
}

/*
 * a input file should looks like
 *
 *    2  #user account
 *    username1
 *    password1
 *    username2
 *    password2
 *
 */
int parse_input(char* path){
/*	char buf[MAX_LINE_LENGTH];
	memset(buf, 0, sizeof(buf));
	FILE* f = fopen(path);
	int bytes_read;

	getline(buf, MAX_LINE_LENGTH, f);//read user account
	user_count = atoi(buf);
	username = malloc(sizeof(char*) * user_count);
	password_md5 = malloc(sizeof(char*) * user_count);
	for (int i = 0; i < user_count; i++){
		username[i] = malloc(sizeof(char) * MAX_USERNAME);
		password_md5 = malloc(sizeof(char) * MD5_LENGTH);

		//read username
		memset(buf, 0, sizeof(buf));
		bytes_read = getline(buf, MAX_LINE_LENGTH, f);
		strncpy(username[i], buf, bytes_read);

		//read and encode password
		memset(buf, 0, sizeof(buf));
		bytes_read = getline(buf, MAX_LINE_LENGTH, f);
		if (md5_encode(buf, bytes_read, password_md5)){
			printf("[parse_input]: cannot encode password %s to md5\n", buf);
			return -1;
		}
	}*/
	return 0;

}
