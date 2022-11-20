#ifndef SERVER_HPP

#define SERVER_HPP

#include <arpa/inet.h>
#include <vector>
#include <syslog.h>
#include <string.h>
#include "tintin_reporter.hpp"
#include "utils.hpp"

#define PORT 4242
#define BACK_LOG 10
#define MAX_CONNECTIONS 3
#define REQUEST_BUFF_SIZE 1024

int accept_client(struct sockaddr_in *client, int sock_s);
void handle_client(int sock_s, std::vector<int> &client_socks, fd_set &fd_list);
int handle_request(std::vector<int> &client_socks, fd_set &fd_list);
void create_server(void);
void close_server(void);
void start_server(void);

extern int g_server_fd;

#endif
