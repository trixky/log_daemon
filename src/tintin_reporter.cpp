#include "tintin_reporter.hpp"
#include "utils.hpp"

Tintin_reporter g_reporter;

Tintin_reporter::Tintin_reporter(void)
{
    this->file = -1;
    // Create the default log file if it doesn't exist
    mkdir(DEFAULT_PATH_DIRECTORY, 0755);

    // Set the (default) path of the log file
    this->setpath(DEFAULT_PATH);
}

Tintin_reporter::Tintin_reporter(const char *path)
{
    this->file = -1;

    // Set the path of the log file
    this->setpath(path);
}

void Tintin_reporter::setpath(const char *path)
{
    // Close the potential opened current log file
    close(this->file);

    // Open/Create the log file
    this->file = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);

    if (this->file == -1)
    {
        // If an error occured from the log file
        fprintf(stderr, "can't open log file [%s]\n", path);
        exit(EXIT_FAILURE);
    }
}

void Tintin_reporter::print(std::string type, std::string msg)
{
    if (this->file < 0)
        // If the log file is not set
        fprintf(stderr, "log file path not set\n");
    else
    {
        // Get the current time
        std::string log = get_time();

        // Concat the mode
        log += "[ ";
        log += type;
        log += " ] - log_daemon: ";

        // Concat the message to log
        log += msg;
        log += "\n";

        // Write the log in the log file
        if (write(this->file, log.c_str(), log.length()) == -1)
        {
            // If an error occured from the log file when writing in
            fprintf(stderr, "failed to write in the log file\n");
            exit(EXIT_FAILURE);
        }
    }
}

void Tintin_reporter::info(std::string msg)
{
    this->print("INFO", msg);
}

void Tintin_reporter::log(std::string msg)
{
    this->print("LOG", msg);
}

Tintin_reporter::~Tintin_reporter(void)
{
    // Close the file descriptor
    close(this->file);
}
