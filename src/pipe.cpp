#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cassert>
#include <cstring>

#include "pipe.h"

namespace pr
{

void display_args(char** argv)
{
	for(char** str = argv; *str != NULL; ++str)
		std::cout << *str << ' ';
	std::cout << std::endl;
}

int exo1(int argc, char* argv[])
{

	int fds[2];
	pipe(fds);

	char** str;
	for(str = argv + 1; *str != NULL && strcmp(*str, "|") != 0; ++str) {}
	// assert : il y a bien un pipe
	assert(*str != NULL);

	assert(*(argv + 1) != NULL);
	assert(*(str + 1) != NULL);

	pid_t pid1, pid2;
	if ((pid1 = fork()) == 0)
	{
		// fils : première commande
		dup2(fds[1], STDOUT_FILENO);
		close(fds[0]);

		*str = NULL; // NULL sur pipe

		execv(*(argv + 1), argv + 2);
		perror("exec 1");
		return 1;
	}
	if ((pid2 = fork()) == 0)
	{
		// fils2 : deuxième commande
		dup2(fds[0], STDIN_FILENO);
		close(fds[1]);

		execv(*(str + 1), str + 2);
		perror("exec 2");
		return 1;
	}
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, 0, 0);
	waitpid(pid2, 0, 0);
	return 0;
}

}
