/*
 * http_header.h
 *
 *  Created on: Dec 25, 2012
 *      Author: hongze
 */

#ifndef HTTP_HEADER_H_
#define HTTP_HEADER_H_

#define HTTP_REQUEST_TYPE_ERROR		(0)
#define HTTP_REQUEST_TYPE_GET		(100)
#define HTTP_REQUEST_TYPE_POST		(200)

typedef struct http_header_s {
	char** 			request_field_name;
	char** 			request_field_value;
	char**			post_field_name;
	char**			post_field_value;
	int				request_field_count;
	int				post_field_count;
	int				http_request_type;
} http_header_t;

/*
 * init http_header struct. This function should always be called before
 * using http_header_t
 */
int http_header_t_init(http_header_t* header);
int http_header_add_request_field(http_header_t* header, char* field_name, char* field_value);
int http_header_set_request_type(http_header_t* header, int request_type);
int http_header_to_text(http_header_t* header, char* output, int output_size);

int http_header_test();
#endif /* HTTP_HEADER_H_ */
