#include <iostream>
#include <signal.h>
#include <stdlib.h>

#include "tintin_reporter.hpp"
#include "locker.hpp"
#include "server.hpp"

void handle_signale(int sig)
{
    g_reporter.info("Received shutdown signal");
    close_server();
    exit(EXIT_SUCCESS);
}

void register_signals(void)
{
    signal(SIGINT, handle_signale);
    signal(SIGQUIT, handle_signale);
    signal(SIGTERM, handle_signale);
}

void daemonize(void)
{
    g_reporter.info("Entering Daemon mode.");

    int pid = fork();
    if (pid < 0)
        _exit(EXIT_FAILURE, false, false);
    if (pid > 0)
        _exit(EXIT_SUCCESS, false, false);
    if (setsid() < 0)
        _exit(EXIT_FAILURE, false, false);
    umask(0);
    fclose(stdout);
    fclose(stderr);
    fclose(stdin);

    g_reporter.info(std::string("Daemon Started. PID " + (std::to_string(getpid()))));
}

int main()
{
    lock();
    g_reporter.info("Started.");
    register_signals();
    daemonize();
    create_server();
    start_server();

    return EXIT_SUCCESS;
}