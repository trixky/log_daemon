#include "utils.hpp"

// https://stackoverflow.com/questions/8343676/how-to-get-current-date-and-time

/* get_time gets and serializes the time in a suitable format */
std::string get_time()
{
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm *parts = std::localtime(&now_c);

    // Format the current time
    char current_time[22];
    sprintf(current_time, "[%02d/%02d/%04d-%02d:%02d:%02d]", parts->tm_mday, 1 + parts->tm_mon, 1900 + parts->tm_year, parts->tm_hour, parts->tm_min, parts->tm_sec);

    return current_time;
}

/* _exit exit the program properly */
void _exit(int status, bool unlock_mode, bool server_mode)
{
    g_reporter.info("Quitting.");

    if (unlock_mode)
    {
        // If in unlock mode
        // Unlock the lock file
        unlock();
    }
    if (server_mode)
    {
        // If in server mode
        // Close the server
        close_server();
    }

    exit(status);
}
