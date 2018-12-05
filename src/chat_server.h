/*
 * chat_server.h
 *
 *  Created on: 5 déc. 2018
 *      Author: 3603567
 */

#ifndef SRC_CHAT_SERVER_H_
#define SRC_CHAT_SERVER_H_
#define NB_CLIENTS 20

#include <vector>
#include "chat_common.h"
#include "pipe.h"
#include "Stack.h"

using namespace std;

class chat_server {
	const char * identifiant;
	pr::Stack<message> * s;

	vector<char *> id_clients;

public:
	chat_server();
	chat_server(const char * id);
//	char * chat_server::getId() const;
	virtual ~chat_server();
	void start(message * m);
	void semserver();
};

#endif /* SRC_CHAT_SERVER_H_ */
