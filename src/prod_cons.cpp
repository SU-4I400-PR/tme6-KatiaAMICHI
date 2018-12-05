#include <iostream>
#include <vector>
#include <cassert>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pipe.h"
#include "Stack.h"


using namespace std;
using namespace pr;

const size_t N = 10;
const size_t M = 10;

void producteur (Stack<char> * stack) {
	std::cout << "producteur" << std::endl;
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	std::cout << "consomateur" << std::endl;
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

int exercice2()
{
	int fd = shm_open("x", O_CREAT|O_RDWR|O_EXCL, 0666);
	ftruncate(fd, sizeof(Stack<char>));
	void* addr = mmap(NULL, sizeof(Stack<char>), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	Stack<char> * s = new(addr) Stack<char>();


	for(size_t i = 0; i < N; ++i)
	{
		pid_t pp = fork();
		if (pp == 0)
		{
			producteur(s);
			return 0;
		}
	}

	for(size_t i = 0; i < M; ++i)
	{
		pid_t pc = fork();
		if (pc == 0)
		{
			consomateur(s);
			return 0;
		}
	}

	for(size_t i = 0; i < N; ++i)
		wait(0);
	for(size_t i = 0; i < M; ++i)
		wait(0);

	wait(0);
	wait(0);

	delete s;
	return 0;
}

int main (int argc, char* argv[])
{
	exercice1(argc, argv);
	//exercice2();
}

