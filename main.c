#include <pthread.h>
#include "protocol/initialization.c"
#include "protocol/adjacencyList.c"
#include "protocol/dijkstra.c"
#include "protocol/mainSettings.c"
#include "protocol/socket.c"

// Threads
void *receiver(void *data);
void *sender(void *data);
pthread_t Receiver, Sender;

// Network settings
mainSettings main_settings;

int main(void) {
	int ID;

	printf("Enter the vertex ID: ");
	scanf("%d", &ID);

	main_settings = initializeProtocol(ID);
	printf("\n\n");

	// Create threads
	pthread_create(&Receiver, NULL, receiver, NULL);
	pthread_create(&Sender, NULL, sender, NULL);

	// Join threads
	pthread_join(Receiver, NULL);
	pthread_join(Sender, NULL);

	return 0;
}

void *receiver(void *data){
    char buf[BUFLEN];

	while(1){
		receiveMessage(&main_settings, buf);
	}
}

void *sender(void *data){
	char buf[BUFLEN];
	int destination_ID;

	while(1){
		char message[BUFLEN];
		
		printf("Enter destination ID: ");
		scanf("%d", &destination_ID);

		printf("Enter message       : ");
		scanf("%s", message);
	 
	sendMessage(&main_settings, destination_ID, buf, message);
	}
}
