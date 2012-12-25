/*
 * tunet.c
 *
 *  Created on: Dec 24, 2012
 *      Author: randyzhao
 */


#include "tunet.h"

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

int generate_http_request(char* buffer, int buffer_size, char* username, char* password_md5){
	//TODO:
	return 0;
}
