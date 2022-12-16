#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "tintin_reporter.hpp"
#include "locker.hpp"
#include "server.hpp"

/* handle_shutdown_signale handles shutdown signals */
void handle_shutdown_signale(int sig)
{
    g_reporter.info("Received shutdown signal.");
    g_reporter.info("Quitting.");
    close_server();
    exit(EXIT_SUCCESS);
}

/* register_shutdown_signals registers shutdown signals */
void register_shutdown_signals(void)
{
    signal(SIGINT, handle_shutdown_signale);
    signal(SIGQUIT, handle_shutdown_signale);
    signal(SIGTERM, handle_shutdown_signale);
}

/* daemonize daemonizes the program */
void daemonize(void)
{
    g_reporter.info("Entering Daemon mode.");

    // Fork (daemonize) the program
    // By killing the parent
    int pid = fork();
    if (pid < 0)
        _exit(EXIT_FAILURE, false, false);
    if (pid > 0)
        _exit(EXIT_SUCCESS, false, false);
    if (setsid() < 0)
        _exit(EXIT_FAILURE, false, false);

    umask(0);

    // Hide outputs
    fclose(stdout);
    fclose(stderr);
    fclose(stdin);

    g_reporter.info(std::string("Daemon Started. (pid " + (std::to_string(getpid()))) + ")");
}

int main()
{
    lock();
    g_reporter.info("Started.");
    register_shutdown_signals();
    daemonize();
    create_server();
    start_server();

    return EXIT_SUCCESS;
}
