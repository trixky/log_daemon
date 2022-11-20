#include "server.hpp"

int g_server_fd;

/* accept_client accept or not a new client.  */
int accept_client(struct sockaddr_in *client, int sock_s)
{
    size_t len;
    int client_socket;

    len = sizeof(struct sockaddr_in);
    if ((client_socket = accept(sock_s, (struct sockaddr *)client, (socklen_t *)&len)) < 0)
        return -1;
    return client_socket;
}

/* handle_client handle new clients arriving.  */
void handle_client(int sock_s, std::vector<int> &client_socks, fd_set &fd_list)
{
    int client_socket;
    struct sockaddr_in client;

    if ((client_socket = accept_client(&client, sock_s)) < 0)
        g_reporter.info(std::string("Client not accepted: " + (std::string(strerror(errno)))));
    else
        client_socks.push_back(client_socket);
}

/* handle_request handle requests.  */
int handle_request(std::vector<int> &client_socks, fd_set &fd_list)
{
    int offset;
    char buf[BUFSIZ];
    std::string msg;

    for (unsigned i = 0; i < client_socks.size(); i++)
    {
        int client = client_socks[i];
        if (FD_ISSET(client, &fd_list))
        {
            bzero(buf, sizeof(buf));
            offset = 0;
            for (;;)
            {
                if (recv(client, buf + offset, 1, 0) <= 0)
                {
                    close(client_socks[i]);
                    client_socks.erase(std::next(client_socks.begin() + i));
                    break;
                }
                if (buf[offset] == '\n')
                {
                    buf[offset] = '\0';
                    break;
                }
                offset++;
                if (offset >= BUFSIZ - 1)
                {
                    msg.append(buf);
                    bzero(buf, sizeof(buf));
                    offset = 0;
                }
            }
            msg.append(buf);
            if (msg.length() > 0)
            {
                if (msg.compare("quit") == 0)
                {
                    close_server();
                    g_reporter.info("Request quit.");
                    _exit(EXIT_SUCCESS, true, true);
                }
                else
                {
                    g_reporter.log(std::string(msg));
                }
            }
            offset = 0;
        }
    }
    return (0);
}

/* create_server create a new global server.  */
void create_server(void)
{
    g_reporter.info("Creating server.");

    struct sockaddr_in server;

    if ((g_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        g_reporter.info("Failed to create the main socket\n");
        fprintf(stderr, "Failed to create the main socket\n");
        _exit(EXIT_FAILURE, true, false);
    }

    int opt = 1;

    if (setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        close_server();
        g_reporter.info("Failed to set options to the main socket\n");
        fprintf(stderr, "Failed to set options to the main socket\n");
        _exit(EXIT_FAILURE, true, true);
    }

    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(g_server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        close_server();
        g_reporter.info("Failed to bind the main socket\n");
        fprintf(stderr, "Failed to bind the main socket\n");
        _exit(EXIT_FAILURE, true, true);
    }

    if (listen(g_server_fd, BACK_LOG))
    {
        close_server();
        g_reporter.info("Failed to listen the main socket\n");
        fprintf(stderr, "Failed to listen the main socket\n");
        _exit(EXIT_FAILURE, true, true);
    }

    g_reporter.info("Server created.");
}

/* create_server close the global server.  */
void close_server(void)
{
    close(g_server_fd);
}

/* start_server start the global server.  */
void start_server()
{
    fd_set fd_list;
    int max_sock;
    std::vector<int> client_socks;

    while (1)
    {
        max_sock = g_server_fd;
        FD_ZERO(&fd_list);
        FD_SET(g_server_fd, &fd_list);
        for (int client : client_socks)
        {
            if (client > 0)
                FD_SET(client, &fd_list);
            if (client > max_sock)
                max_sock = client;
        }
        if (select(max_sock + 1, &fd_list, NULL, NULL, NULL) < 0)
        {
            close_server();
            const std::string select_error_msg = std::string("Failed to select: " + (std::string(strerror(errno))));
            g_reporter.info(select_error_msg);
            fprintf(stderr, select_error_msg.c_str());
            _exit(EXIT_FAILURE, true, true);
            return;
        }
        if (FD_ISSET(g_server_fd, &fd_list))
            handle_client(g_server_fd, client_socks, fd_list);
        if (handle_request(client_socks, fd_list))
            return;
    }
}