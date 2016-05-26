/***************************************************//**
 * @file    main.cpp
 * @date    February 2015
 * @author  Ocean Optics, Inc.
 *
 * The main program for ocean-daemon. This sets up the request handler as a daemon,
 * forking processes detaching any terminal and closing standard streams as necessary.
 *
 *
 * LICENSE:
 *
 * Dev Kit Copyright (C) 2015, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#include "Daemon.h"
#include "OceanHandlerConfiguration.h"
#include "OceanHandler.h"
#include <boost/bind.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/thread.hpp>
#include <syslog.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[]) {

    try {

        boost::asio::io_service io_service;
        // Initialise the server before becoming a daemon. If the process is
        // started from a shell, this means any errors will be reported back to the
        // user.
        sleep(5);

        // Register signal handlers so that the daemon may be shut down. You may
        // also want to register for other signals, such as SIGHUP to trigger a
        // re-read of a configuration file.
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
        signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));

        // Inform the io_service that we are about to become a daemon. The
        // io_service cleans up any internal resources, such as threads, that may
        // interfere with forking.
        io_service.notify_fork(boost::asio::io_service::fork_prepare);

        // Fork the process and have the parent exit. If the process was started
        // from a shell, this returns control to the user. Forking a new process is
        // also a prerequisite for the subsequent call to setsid().
        if (pid_t pid = fork()) {
            if (pid > 0) {
                // We're in the parent process and need to exit.
                //
                // When the exit() function is used, the program terminates without
                // invoking local variables' destructors. Only global variables are
                // destroyed. As the io_service object is a local variable, this means
                // we do not have to call:
                //
                //   io_service.notify_fork(boost::asio::io_service::fork_parent);
                //
                // However, this line should be added before each call to exit() if
                // using a global io_service object. An additional call:
                //
                //   io_service.notify_fork(boost::asio::io_service::fork_prepare);
                //
                // should also precede the second fork().
                exit(0);
            }
            else {
                syslog(LOG_ERR | LOG_USER, "First fork failed: %m");
                return 1;
            }
        }

        // Make the process a new session leader. This detaches it from the
        // terminal.
        setsid();

        // A process inherits its working directory from its parent. This could be
        // on a mounted filesystem, which means that the running daemon would
        // prevent this filesystem from being unmounted. Changing to the root
        // directory avoids this problem.
        chdir("/");

        // The file mode creation mask is also inherited from the parent process.
        // We don't want to restrict the permissions on files created by the
        // daemon, so the mask is cleared.
        umask(0);

        // A second fork ensures the process cannot acquire a controlling terminal.
        if (pid_t pid = fork()) {
            if (pid > 0) {
                exit(0);
            }
            else {
                syslog(LOG_ERR | LOG_USER, "Second fork failed: %m");
                return 1;
            }
        }

        // Close the standard streams. This decouples the daemon from the terminal
        // that started it.
        close(0);
        close(1);
        close(2);

        // We don't want the daemon to have any standard input.
        if (open("/dev/null", O_RDONLY) < 0) {
            syslog(LOG_ERR | LOG_USER, "Unable to open /dev/null: %m");
            return 1;
        }

        // Send standard output to a log file.
        const char* output = "/tmp/spectrad.daemon.out";
        const int flags = O_WRONLY | O_CREAT | O_APPEND;
        const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        if (open(output, flags, mode) < 0) {
            syslog(LOG_ERR | LOG_USER, "Unable to open output file %s: %m", output);
            return 1;
        }

        // Also send standard error to the same log file.
        if (dup(1) < 0) {
            syslog(LOG_ERR | LOG_USER, "Unable to dup output descriptor: %m");
            return 1;
        }

        // Inform the io_service that we have finished becoming a daemon. The
        // io_service uses this opportunity to create any internal file descriptors
        // that need to be private to the new process.
        io_service.notify_fork(boost::asio::io_service::fork_child);

        // The io_service can now be used normally.
        OceanHandlerConfiguration config;
        OceanHandler oceanHandler(io_service, config);
        Daemon daemon(io_service, config, oceanHandler);

        syslog(LOG_INFO | LOG_USER, "Daemon started");
        daemon.Run();
        io_service.run();

        syslog(LOG_INFO | LOG_USER, "Daemon stopped");
    }
    catch (std::exception& e) {
        syslog(LOG_ERR | LOG_USER, "Exception: %s", e.what());
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}