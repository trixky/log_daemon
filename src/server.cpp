#include "server.hpp"

int g_server_fd;

/* accept_client try to accept a new client.  */
int accept_client(struct sockaddr_in *client, int sock_s)
{
    size_t len;
    int client_socket;

    len = sizeof(struct sockaddr_in);
    if ((client_socket = accept(sock_s, (struct sockaddr *)client, (socklen_t *)&len)) < 0)
        return -1;
    return client_socket;
}

/* refuse_client refuses a client.  */
void refuse_client(struct sockaddr_in *client, int sock_s)
{
    size_t len;
    int client_socket;

    len = sizeof(struct sockaddr_in);
    if ((client_socket = accept(sock_s, (struct sockaddr *)client, (socklen_t *)&len)) < 0)
        return;
    // Close the connection of the client if he has been accepted
    close(client_socket);
}

/* handle_client handle new clients arriving.  */
void handle_client(int sock_s, std::vector<int> &client_socks, fd_set &fd_list)
{
    int client_socket;
    struct sockaddr_in client;

    if (client_socks.size() >= MAX_CONNECTIONS)
    {
        // If already too many client are simultaneously connected
        g_reporter.info(std::string("Client refused because: too many client connected. (max: " + (std::to_string(MAX_CONNECTIONS))) + ")");
        // Refuse the incoming client
        refuse_client(&client, sock_s);
    }
    else if ((client_socket = accept_client(&client, sock_s)) < 0)
        // Else if an error occured in the client accepting
        g_reporter.info(std::string("Client not accepted: " + (std::string(strerror(errno)))));
    else
    {
        // Else the client was accepted
        g_reporter.info(std::string("Client accepted."));
        client_socks.push_back(client_socket);
    }
}

/* handle_request handle requests.  */
void handle_request(std::vector<int> &client_socks, fd_set &fd_list)
{
    int offset;
    char buf[BUFSIZ];
    std::string msg;

    for (unsigned i = 0; i < client_socks.size(); i++)
    {
        // For each known client
        int client = client_socks[i];
        if (FD_ISSET(client, &fd_list))
        {
            // If the client is still connected
            bzero(buf, sizeof(buf));
            offset = 0;
            for (;;)
            {
                // While his message is not terminated
                if (recv(client, buf + offset, 1, 0) <= 0)
                {
                    // If the message is empty
                    // Considere the client leave
                    g_reporter.info(std::string("Client leave."));
                    // Close the connection of the leaving client
                    close(client_socks[i]);
                    // Remove the client from known ones
                    client_socks.erase(client_socks.begin() + i);
                    break;
                }
                if (buf[offset] == '\n')
                {
                    // Handle line breaks
                    buf[offset] = '\0';
                    break;
                }
                offset++;
                if (offset >= BUFSIZ - 1)
                {
                    // Append the current part of the message from the buffer in global message
                    msg.append(buf);
                    // Clean the buffer for the next round
                    bzero(buf, sizeof(buf));
                    offset = 0;
                }
            }
            // Append the last part of the message from the last buffer round
            msg.append(buf);
            if (msg.length() > 0)
            {
                // If the message lenght is not null
                if (msg.compare("quit") == 0)
                {
                    // If the message is the "quit" command
                    // Quit the program/server
                    close_server();
                    g_reporter.info("Request quit.");
                    _exit(EXIT_SUCCESS, true, true);
                }
                else
                {
                    // Else save/print the message with the reporter
                    g_reporter.log(std::string(msg));
                }
            }
            offset = 0;
        }
    }
}

/* create_server create a new global server.  */
void create_server(void)
{
    g_reporter.info("Creating server.");

    struct sockaddr_in server;

    // Init the server fle descriptor
    if ((g_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        g_reporter.info("Failed to create the main socket\n");
        _exit(EXIT_FAILURE, true, false);
    }

    int opt = 1;

    // Set the server file descriptor options
    if (setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        close_server();
        g_reporter.info("Failed to set options to the main socket\n");
        _exit(EXIT_FAILURE, true, true);
    }

    // Set the bind options
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the server file descriptor
    if (bind(g_server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        close_server();
        g_reporter.info("Failed to bind the main socket\n");
        _exit(EXIT_FAILURE, true, true);
    }

    // Listen new connection using the server file descriptor
    if (listen(g_server_fd, BACK_LOG))
    {
        close_server();
        g_reporter.info("Failed to listen the main socket\n");
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
        // Clean the list of registered file descriptor
        FD_ZERO(&fd_list);
        // Add the server file descriptor
        FD_SET(g_server_fd, &fd_list);
        // Add each known client if there not corrupted
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
            _exit(EXIT_FAILURE, true, true);
            return;
        }
        if (FD_ISSET(g_server_fd, &fd_list))
            // If the client is not known
            // Handle the new client arriving
            handle_client(g_server_fd, client_socks, fd_list);
        // Handle clients requests
        handle_request(client_socks, fd_list);
    }
}
