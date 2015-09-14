/***************************************************//**
 * @file    heliospectraSTSdaemon_eventLoop.h
 * @date    January 2015
 * @author  Kirk Clendinning, Heliospectra
 *
 * Notes:
 *
 * periodic functionality for the daemon
 *
 */

#ifndef HELIOSPECTRA_STS_DAEMON_EVENTLOOP_H
#define HELIOSPECTRA_STS_DAEMON_EVENTLOOP_H


#include <netdb.h> // needed for struct addrinfo

#define noError 0  // Linux omissions
#define success 0

#define MAX_MESSAGE_LENGTH 32
#define NOFLAGS 0
#define POLL_WAIT_FOREVER -1

#define TRUE 1
#define FALSE 0

int EventLoop(volatile int*, int);

#endif