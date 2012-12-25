/*
 * tunet.h
 *
 *  Created on: Dec 24, 2012
 *      Author: randyzhao
 */

#ifndef TUNET_H_
#define TUNET_H_

/*
 * generate a http request header for login purpose. The generated header will be stored in the buffer.
 * When the header is largers than the buffer_size or other error occurs, it will return -1
 * otherwise it will return 0;
 *
 */
int generate_login_http_request(char* buffer, int buffer_size, char* username, char* password_md5);

#endif /* TUNET_H_ */
