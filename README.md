# log_daemon

A daemon that reads the messages it receives through tcp connections and writes them to a log file.

- Only one daemon instance can run at a time using a file lock system.
- Only 3 clients can connect simultaneously to the daemon.
- The daemon listen to the port 4242.

## Usage

```
make
sudo ./build/log_daemon
ps aux | grep log_daemon
nc localhost 4242
test
quit

ps aux | grep log_daemon
cat /var/log/log_daemon/log_daemon.log
```

> The program will have to run only with the root rights.