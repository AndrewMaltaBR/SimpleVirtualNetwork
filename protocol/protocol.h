#ifndef PROTOCOL
#define PROTOCOL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Contants
#define LOCALIP "127.0.0.1"
#define BUFLEN 150  // Max length of buffer
#define ACKMSG "Ack"
#define TIMEOUT_MS 100
#define TIMEOUT_MAX 10
#define TRUE 1
#define FALSE 0
#define INF 999999

// Structs and types
typedef struct node{
  int ID;
  int cost;
  struct node *next;
}vertex;

typedef struct proc{
	int ID;
	char IP[15];
	int socket;
}proc;

typedef struct path {
	proc destination;
	int cost;
	proc next_proc;
}path;

typedef struct mainSettings{
	proc me;
	path *path_table;
	int len;
	struct sockaddr_in si_me;
	struct sockaddr_in si_other;
	int s;
	int recv_len;
	int waiting_ack;
}mainSettings;

typedef struct message{
	proc origin;
	proc destination;
	unsigned short ack;
	char text[BUFLEN];
} message;

// Initialization functions
mainSettings initializeProtocol(int ID);
void die(char *s);

// Adjacency list functions
vertex *addVertex(vertex *adjacency_list, int ID, int cost);
vertex **readVertexesFromFile(vertex **adjacency_list, int *len);
void showAdjacencyList(vertex **adjacency_list, int len);
void freeAdjacencyList(vertex **adjacency_list, int len);

// Dijkstra functions
vertex *createDijkstraTable(vertex **adjacency_list, int len, int ID);
void showDijkstraTable(vertex *dijkstra_table, int len);
void freeDijkstraTable(vertex *dijkstra_table);

// Main settings functions
mainSettings createMainSettings(vertex *dijkstra_table, int len);
void showMainSettings(mainSettings main_settings);

// Socket functions
void createSocket(mainSettings *main_settings);
void receiveMessage(mainSettings *main_settings, char *buf);
void sendMessage(mainSettings *main_settings, int destination_ID, char *buf, char *msg_text);
void fowardMessage(mainSettings *main_settings, int destination_ID, char *encoded_msg);

#endif
