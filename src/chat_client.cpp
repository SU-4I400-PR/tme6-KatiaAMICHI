/*
 * chat_client.cpp
 *
 *  Created on: 4 déc. 2018
 *      Author: 3603567
 */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipe.h"
#include "Stack.h"
#include "chat_server.h"
#include "chat_client.h"
#include "chat_common.h"
#include <thread>
using namespace std;
using namespace pr;


void work_echo(Stack<message> * ps, int fds [], const char* path){
	std::cout << "consomateur" << std::endl;
	char * c[ps->getSize()+1];
	*c[0] = (char)ps->getSize();
	int i=1;
	while (true) {
		c[i] = ps->pop().content;
		cout << c << flush;
		i++;
	}

	close(fds[0]);
	dup2(fds[1], STDOUT_FILENO);
	execv(path , c);
	perror("exec 1");
}

void work_ecrit(Stack<message> * pc, int fds [], const char* path, int size){
	std::cout << "producteur" << std::endl;
	char * st[size];
	int i = 0;
	char c;
	while (cin.get(c)) {
		message m ;
		m.type = 2;
		m.content[0] = c;
		pc->push(m);
		st[i] = &c;
		i++;
	}

	dup2(fds[0], STDIN_FILENO);
	close(fds[1]);
	execv(path, st);
	perror("exec 2");
}



chat_client::chat_client(char * id_client, char * id_server) {
	int fd_server = shm_open(id_server, O_CREAT|O_RDWR|O_EXCL, 0666);
	ftruncate(fd_server, sizeof(Stack<message>));
	void* addr_server = mmap(NULL, sizeof(Stack<message>), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0);
	this->pile_server = new(addr_server) Stack<message>();

	this->identifiant = id_client;
	int fd_client = shm_open(id_client, O_CREAT|O_RDWR|O_EXCL, 0666);
	ftruncate(fd_client, sizeof(Stack<message>));
	void* addr_client = mmap(NULL, sizeof(Stack<message>), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);
	this->pile_client = new(addr_client) Stack<message>();

	signal(SIGINT,[](int sig){
		this->~chat_client(); //
	});

}


void chat_client::start(char * args[]){
	int fds[2];
	pipe(fds);
	thread t1(work_echo,pile_server,fds,args[0]);
	thread t2(work_ecrit, pile_client,fds,"/bin/cat",pile_server->getSize());
}

chat_client::~chat_client(){
	pile_client->~Stack();
	pile_server->~Stack();
}


