/*
 * chat_client.h
 *
 *  Created on: 4 déc. 2018
 *      Author: 3603567
 */

#ifndef SRC_CHAT_CLIENT_H_
#define SRC_CHAT_CLIENT_H_

#include <vector>
#include "chat_common.h"
#include "pipe.h"
#include "Stack.h"

class chat_client {
	const char * identifiant;
	pr::Stack<message> * pile_server;
	pr::Stack<message> * pile_client;

public:
	chat_client(char * id_client, char * id_server);
	void start(char * args[]);
	~chat_client();
};

#endif /* SRC_CHAT_CLIENT_H_ */
