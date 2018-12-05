#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>
#include <iostream>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];

	size_t sz;

	sem_t sem_prod;
	sem_t sem_cons;
	sem_t sem_mtx;

public :
	int getSize(){
		return sz;
	}
	Stack () : sz(0) {
		std::cout << "Strack" << std::endl;
		memset(tab,0,sizeof tab) ;

		int status = sem_init(&sem_prod, 1, STACKSIZE);
		if (status < 0) {
			perror("sem_prod : ");
			exit(-1);
		}

		status =  sem_init(&sem_cons, 1, 0);
		if (status < 0) {
			perror("sem_cons : ");
			exit(-1);
		}

		status =  sem_init(&sem_mtx, 1, 1);
		if (status < 0) {
			perror("sem_mtx : ");
			exit(-1);
		}
	}
	~Stack() {
		sem_destroy(&sem_prod);
		sem_destroy(&sem_mtx);
		sem_destroy(&sem_cons);
	}

	T pop () {
		std::cout << "pop" << std::endl;
		// bloquer si vide
		sem_wait(&sem_cons);
		std::cout << "[pop] Popping sem_cons" << std::endl;
		sem_wait(&sem_mtx);
		std::cout << "[pop] Popping sem_mtx" << std::endl;

		T toret = tab[--sz];

		sem_post(&sem_mtx);
		sem_post(&sem_prod);

		std::cout << "Popped" << std::endl;
		return toret;
	}

	void push(T elt) {
		std::cout << "push" << std::endl;
		//bloquer si plein
		sem_wait(&sem_prod);
		std::cout << "[push] Popping sem_prod" << std::endl;
		sem_wait(&sem_mtx);
		std::cout << "[push] Popping sem_mtx" << std::endl;

		tab[sz++] = elt;

		sem_post(&sem_mtx);
		sem_post(&sem_cons);
		std::cout << "Pushed" << std::endl;
	}
};

}
