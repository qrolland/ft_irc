#include <iostream>
#include <csignal>

bool stopServer = false;

void signalHandler(int signum)
{
	if (signum == SIGINT)
		stopServer = true;
}