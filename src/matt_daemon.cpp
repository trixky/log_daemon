#include <iostream>
#include <signal.h>
#include <stdlib.h>

#include "tintin_reporter.hpp"
#include "locker.hpp"
#include "server.hpp"

void _exit(void)
{
    unlock();
}

void handle_signale(int sig)
{
    g_reporter.log("Received shutdown signal");
}

void register_signals(void)
{
    signal(SIGINT, handle_signale);
    signal(SIGQUIT, handle_signale);
    signal(SIGTERM, handle_signale);
}

void daemonize(void)
{
    g_reporter.log("Entering Daemon mode.");

    int pid = fork();
    if (pid < 0)
        _exit(EXIT_FAILURE);
    if (pid > 0)
        _exit(EXIT_SUCCESS);
    if (setsid() < 0)
        _exit(EXIT_FAILURE);
    umask(0);
    fclose(stdout);
    fclose(stderr);
    fclose(stdin);

    g_reporter.log(std::string("Daemon Started. PID " + (std::to_string(getpid()))));
}

int main()
{
    lock();
    g_reporter.log("Started.");
    start_server();
    daemonize();

    return 0;
}