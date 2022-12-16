#ifndef TINTIN_REPORTER_HPP

#define TINTIN_REPORTER_HPP

#include <iostream>
#include <sys/stat.h>

#define DEFAULT_PATH_DIRECTORY "/var/log/matt_daemon"
#define DEFAULT_PATH "/var/log/matt_daemon/matt_daemon.log"

class Tintin_reporter
{
private:
    int file;
    void print(std::string type, std::string msg);

public:
    Tintin_reporter(void);
    Tintin_reporter(const char *path, const char *path_directory);
    ~Tintin_reporter(void);

    void setpath(const char *path, const char *path_directory);
    void log(std::string msg);
    void info(std::string msg);
    void error(std::string msg);
};

extern Tintin_reporter g_reporter;

#endif
