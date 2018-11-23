#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cassert>

#include "pipe.h"
#include "Stack.h"



using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}


int exo2()
{
	Stack<char> * s = new Stack<char>();

	pid_t pp = fork();
	if (pp==0) {
		producteur(s);
		return 0;
	}

	pid_t pc = fork();
	if (pc==0) {
		consomateur(s);
		return 0;
	}

	wait(0);
	wait(0);

	delete s;
	return 0;
}

int main (int argc, char* argv[])
{
	int fds[2];
		pipe(fds);
		execv("/bin/pwd", { NULL });
		perror("ee");
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

