/*
 * http_header.c
 *
 *  Created on: Dec 25, 2012
 *      Author: hongze
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "http_header.h"

#define MAX_HTTP_HEADER_FIELD_COUNT			(200)
#define MAX_HTTP_HEADER_FIELD_LENGTH		(300)

#define POST_METHOD ("POST /cgi-bin/do_login HTTP/1.1\r\n")
int http_header_t_init(http_header_t* header) {
	memset(header, 0, sizeof(header));
	header->request_field_count = 0;
	header->post_field_count = 0;
	header->request_path = NULL;
	header->request_field_name = malloc(
			sizeof(char*) * MAX_HTTP_HEADER_FIELD_COUNT);
	header->request_field_value = malloc(
			sizeof(char*) * MAX_HTTP_HEADER_FIELD_COUNT);
	header->post_field_name = malloc(
			sizeof(char*) * MAX_HTTP_HEADER_FIELD_COUNT);
	header->post_field_value = malloc(
			sizeof(char*) * MAX_HTTP_HEADER_FIELD_COUNT);
	header->http_request_type = HTTP_REQUEST_TYPE_ERROR;
	return 0;
}

int http_header_set_request_path(http_header_t* header, char* request_path){
	if (!header){
		return -1;
	}
	header->request_path = strndup(request_path, MAX_HTTP_HEADER_FIELD_LENGTH);
	return 0;
}

int http_header_add_request_field(http_header_t* header, char* field_name,
		char* field_value) {
	if (header->request_field_count >= MAX_HTTP_HEADER_FIELD_COUNT) {
		return -1;
	}
	header->request_field_name[header->request_field_count] = strndup(
			field_name, MAX_HTTP_HEADER_FIELD_LENGTH);
	header->request_field_value[header->request_field_count] = strndup(
			field_value, MAX_HTTP_HEADER_FIELD_LENGTH);
	header->request_field_count++;
	return 0;
}
int http_header_set_request_type(http_header_t* header, int request_type) {
	header->http_request_type = request_type;
	return 0;
}

int http_header_add_post_field(http_header_t* header, char* field_name, char* field_value) {
	if (header->post_field_count >= MAX_HTTP_HEADER_FIELD_COUNT) {
		return -1;
	}
	header->post_field_name[header->post_field_count] = strndup(field_name,
			MAX_HTTP_HEADER_FIELD_LENGTH);
	header->post_field_value[header->post_field_count] = strndup(field_value,
			MAX_HTTP_HEADER_FIELD_LENGTH);
	header->post_field_count++;
	return 0;
}
int http_header_to_text(http_header_t* header, char* output, int output_size) {
	memset(output, 0, sizeof(output));
	int offset = 0;
	int i;
/*	switch (header->http_request_type){
	case HTTP_REQUEST_TYPE_POST:
		strncpy(output + offset, POST_METHOD, sizeof(POST_METHOD));
		offset += sizeof(POST_METHOD) -1;
		break;
	default:
		return -1;
	}*/
	strncpy(output + offset, header->request_path, strlen(header->request_path));
	offset += strlen(header->request_path);
	for (i = 0; i < header->request_field_count; i++) {
		int name_length = strnlen(header->request_field_name[i],
				MAX_HTTP_HEADER_FIELD_LENGTH);
		int value_length = strnlen(header->request_field_value[i],
				MAX_HTTP_HEADER_FIELD_LENGTH);
		strncpy(output + offset, header->request_field_name[i],
				MAX_HTTP_HEADER_FIELD_LENGTH);
		offset += name_length;
		strncpy(output + offset, ": ", MAX_HTTP_HEADER_FIELD_LENGTH);
		offset += 2;
		strncpy(output + offset, (const char*)header->request_field_value[i],
				MAX_HTTP_HEADER_FIELD_LENGTH);
		offset += value_length;
		strncpy(output + offset, "\r\n", MAX_HTTP_HEADER_FIELD_LENGTH);
		offset += 2;
		if (offset >= output_size) {
			return -1;
		}
	}

	//append POST data here
	if (HTTP_REQUEST_TYPE_POST == header->http_request_type) {
		strncpy(output + offset, "\r\n", MAX_HTTP_HEADER_FIELD_LENGTH);
		offset += 2;
		for (i = 0; i < header->post_field_count; i++) {
			if (i != 0) {
				strncpy(output + offset, "&", MAX_HTTP_HEADER_FIELD_LENGTH);
				offset += 1;
			}
			int name_length = strnlen(header->post_field_name[i],
					MAX_HTTP_HEADER_FIELD_LENGTH);
			int value_length = strnlen(header->post_field_value[i],
					MAX_HTTP_HEADER_FIELD_LENGTH);
			strncpy(output + offset, header->post_field_name[i],
					MAX_HTTP_HEADER_FIELD_LENGTH);
			offset += name_length;
			strncpy(output + offset, "=", MAX_HTTP_HEADER_FIELD_LENGTH);
			offset += 1;
			strncpy(output + offset, header->post_field_value[i],
					MAX_HTTP_HEADER_FIELD_LENGTH);
			offset += value_length;
			if (offset >= output_size) {
				return -1;
			}
		}
		strncpy(output + offset, "\r", MAX_HTTP_HEADER_FIELD_LENGTH);
		offset += 1;
	}
	return 0;
}

int http_header_test() {
	printf("begin test http_header\n");
	http_header_t* header =
			(http_header_t*) malloc(sizeof(http_header_t));
	if (http_header_t_init(header)) {
		return -1;
	}
	http_header_set_request_type(header, HTTP_REQUEST_TYPE_POST);
	http_header_add_request_field(header, "test", "sgsgsg");
	http_header_add_request_field(header, "test2", "sss");
	http_header_add_post_field(header, "post1", "aaa");
	http_header_add_post_field(header, "post2", "ssssqqq");
	char buf[1000];
	memset(buf, 0, sizeof(buf));
	http_header_to_text(header, buf, 1000);
	printf("header content is :\n%s\n", buf);
	free(header);
	return 0;
}
