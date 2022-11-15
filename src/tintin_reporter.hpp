#ifndef TINTIN_REPORTER_HPP

#define TINTIN_REPORTER_HPP

#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_PATH_DIRECTORY "/var/log/matt_daemon"
#define DEFAULT_PATH "/var/log/matt_daemon/matt_daemon.log"

class Tintin_reporter
{
private:
    int file;

public:
    Tintin_reporter(void);
    Tintin_reporter(const char *path);
    ~Tintin_reporter(void);

    void setpath(const char *path);
    void log(std::string log);
};

extern Tintin_reporter g_reporter;

#endif
