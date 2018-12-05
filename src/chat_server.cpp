/*
 * chat_server.cpp
 *
 *  Created on: 5 déc. 2018
 *      Author: 3603567
 */
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
#include "chat_server.h"

using namespace std;
using namespace pr;


vector<message> consomateur(Stack<message> * s){
	std::cout << "consomateur" << std::endl;
	vector<message> vec;
	vec.reserve(s->getSize());

	while (true) {
		message c = s->pop();
		vec.emplace_back(c);
	}
	return vec;
}


chat_server::chat_server(const char * id) {
	identifiant = id;
	int fd = shm_open(identifiant, O_CREAT|O_RDWR|O_EXCL, 0666);
	ftruncate(fd, sizeof(Stack<message>));
	void* addr = mmap(NULL, sizeof(Stack<message>), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	this->s = new(addr) Stack<message>();


	vector<message> requetes;
	requetes = consomateur(s);
}

//char * chat_server::getId()const{
//	return identifiant;
//}

void chat_server::start(message * m){
	int index = 0;
	if(m->type == 1){
		id_clients.emplace_back(m->content);
	}else if(m->type == 3){
		for(auto it = id_clients.begin(); it != id_clients.end(); it++){
			if(m->content == *it){
				index++;
				break;
			}
		}
		id_clients.erase(id_clients.begin(), id_clients.begin()+index);
	}else if(m->type == 2){
		for(auto it = id_clients.begin(); it != id_clients.end(); it++){
			int fd_client = shm_open(*it, O_CREAT|O_RDWR|O_EXCL, 0666);
			ftruncate(fd_client, sizeof(Stack<message>));
			void* addr_client = mmap(NULL, sizeof(Stack<message>), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0);
			Stack<message> * pile_client = new(addr_client) Stack<message>();

			pile_client->push(*m);
		}
	}

}

chat_server::~chat_server() {
	// TODO Auto-generated destructor stub
}

