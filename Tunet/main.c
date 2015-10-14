/*
 * main.c
 *
 *  Created on: Dec 24, 2012
 *      Author: randyzhao
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_header.h"
#include "tunet.h"
#include "md5.h"

#define MAX_BUFFER_LENGTH (128)
#define MAX_LINE_LENGTH (4096)
#define MAX_USERNAME (10)
#define MAX_PASSWORD (64)

#define DEFAULT_CONFIG "/etc/tunet/tunet.conf"
#define USAGE \
    ("Usage: tunet [options] <login|logout|info|status>\n" \
     "       -c: the configuration file, default: " DEFAULT_CONFIG "\n" \
     "       -uUSERNAME: set username as USERNAME\n" \
     "       -pPASSWORD: set password as PASSWORD\n\n" \
     "       config file example:\n" \
     "         host_ip = 166.111.204.120 # optional\n" \
     "         port = 80 # optional\n" \
     "         username = myname15\n" \
     "         password = 123456\n" )

struct Opts {
    char *config_file;
    char *username;
    char *password;
};

static int parse_options(int argc, char *argv[], struct Opts *opts);
static int parse_config_file(struct Opts *opts, tunet_connection_helper_t *helper);

int main(int argc, char *argv[]){
    struct Opts opts;
    memset(&opts, 0, sizeof(opts));
    if (parse_options(argc, argv, &opts)) {
        printf(USAGE);
        return -1;
    }
	tunet_connection_helper_t* helper =
			(tunet_connection_helper_t*)malloc(sizeof(tunet_connection_helper_t));
	tunet_connection_helper_t_init(helper, opts.username, opts.password);
    if (parse_config_file(&opts, helper)) {
        return -1;
    }

    char *action = argv[argc - 1];
    if (tunet_do(helper, action)) {
        printf(USAGE);
    }
	return 0;
}

int parse_options(int argc, char *argv[], struct Opts *opts) {
    opts->config_file = NULL;
    opts->username = NULL;
    opts->password = NULL;
    if (argc < 2) return -1;
    char *option = NULL;
    int i;
    for (i = 1; i < argc - 1; i++) {
        int len = strlen(argv[i]);
        if (argv[i][0] == '-') {
            if (len < 2) {
                return -1;
            }
            option = argv[i] + 1;
        }
        if (argv[i][0] != '-' || len >= 2) {
            switch (*option) {
                case 'c':
                    if (option[1] != '\0') {
                        printf("[tunet] Command line auguments error: unknown option '%s'\n", option);
                        return -1;
                    }
                    opts->config_file = argv[i];
                    break;
                case 'u':
                    opts->username = option + 1;
                    break;
                case 'p':
                    opts->password = option + 1;
                    break;                        
                default:
                    printf("[tunet] Command line auguments error: unknown option '%s'\n", option);
                    return -1;
            }
        }
    }
    if (!opts->config_file && !(opts->username && opts->password)) {
        opts->config_file = DEFAULT_CONFIG;
    }
    if (opts->config_file) {
        printf("[tunet] Using configuration file %s\n", opts->config_file);
    }
    return 0;
}

void trim_left(char **pstr) {
    char *p = *pstr;
    while (*p != '\0' && (*p == ' ' || *p == '\t')) p++;
    *pstr = p;
}

static void trim(char **pstr) {
    trim_left(pstr);
    char *p = *pstr + strlen(*pstr) - 1;
    while (p > *pstr && (*p == ' ' || *p == '\t')) p--;
    p[1] = '\0';
}

/*
 * a configure file should looks like
 *
 *    host_ip = 166.111.204.120
 *    port = 80
 *    username = user1_name
 *    password = user1_password
 *    username = user2_name
 *    password = user2_password
 *    ...
 *
 * (server_ip and server_port are optional, 8 users at most)
 *
 */
static int parse_config_file(struct Opts *opts, tunet_connection_helper_t *helper) {
    int host_converted = 0;
    if (opts->config_file) {
        FILE *fp = fopen(opts->config_file, "r");
        if (fp == NULL) {
            fprintf(stderr, "[tunet] Error reading configuration file: %s\n", opts->config_file);
            return -1;
        }
        char buf[MAX_LINE_LENGTH];
        char *line = buf;
        int waiting_for_password = 0;
        char no_passwd_hint[] = "Configuration error: no password for user %s\n";
        while (fgets(line, MAX_LINE_LENGTH, fp)) {
            int len = strlen(line) - 1;
            line[len] = '\0';
            // trim
            trim_left(&line);
            // Skip comments
            if (line[0] == '#') continue;
            // tokenize
            char *sep = strstr(line, "=");
            if (sep == NULL) {
                printf("Configuration error: unrecognized line: %s\n", line);
                return -1;
            }
            char *key = line;
            *sep = '\0';
            char *val = sep + 1;
            trim(&key);
            trim(&val);
            if (strcmp(key, "host_ip") == 0) {
                if (inet_pton(AF_INET, val, &helper->servaddr.sin_addr) != 1) {
                    printf("ERROR cannot convert server address: %s, try default: %s\n", val, SERVER_IP);
                }
            } else if (strcmp(key, "port") == 0) {
                helper->servaddr.sin_port = htons(atoi(val));
            } else if (strcmp(key, "username") == 0) {
                if (helper->user_count >= MAX_USER_COUNT) {
                    printf("WARNING no more users supported. Ignore user %s\n", val);
                    continue;
                }
                if (waiting_for_password) {
                    printf(no_passwd_hint, helper->usernames[helper->user_count]);
                    return 1;
                }
                strcpy(helper->usernames[helper->user_count], val);
                waiting_for_password = 1;
            } else if (strcmp(key, "password") == 0) {
                if (helper->user_count >= MAX_USER_COUNT) {
                    continue;
                }
                encode_password(helper->password_md5s[helper->user_count], val);
                helper->user_count++;
            } else if (strcmp(key, "mode") == 0) {
                if (strcmp(val, "normal") == 0) {
                    helper->mode = NORMAL;
                } else {
                    printf("WARNING unsupported working mode \"%s\", reset as \"normal\"\n", val);
                }
            } else {
                printf("WARNING unknown key: %s\n", key);
            }
        }
        fclose(fp);
    }
    if (!host_converted) {
        if (inet_pton(AF_INET, SERVER_IP, &helper->servaddr.sin_addr) != 1) {
            printf("ERROR cannot convert server address: %s.\n", SERVER_IP);
            return -1;
        }
    }
    return 0;
}

