#ifndef TINTIN_REPORTER_HPP

#define TINTIN_REPORTER_HPP

#include <iostream>
#include <sys/stat.h>

#define DEFAULT_PATH_DIRECTORY "/var/log/log_daemon"
#define DEFAULT_PATH "/var/log/log_daemon/log_daemon.log"

class Tintin_reporter
{
private:
    int file;
    void print(std::string type, std::string msg);

public:
    Tintin_reporter(void);
    Tintin_reporter(const char *path);
    ~Tintin_reporter(void);

    void setpath(const char *path);
    void log(std::string msg);
    void info(std::string msg);
};

extern Tintin_reporter g_reporter;

#endif
