#include "protocol.h"

void createSocket(mainSettings *main_settings){
	// Create a UDP socket
	if ((main_settings->s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");
	 
	// Zero out the structure
	memset((char *) &main_settings->si_me, 0, sizeof(main_settings->si_me));
	 
	main_settings->si_me.sin_family = AF_INET;
	main_settings->si_me.sin_port = htons(main_settings->me.socket);
	main_settings->si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	 
	// Bind socket to port
	if(bind(main_settings->s, (struct sockaddr*) &main_settings->si_me, sizeof(main_settings->si_me) ) == -1)
		die("bind");
}

char *encodeMessage(message msg, char *encoded_msg){
	sprintf(encoded_msg, "%d %s %d %d %s %d %hu %s", msg.origin.ID, msg.origin.IP, msg.origin.socket, msg.destination.ID, msg.destination.IP, msg.destination.socket, msg.ack, msg.text);
	return encoded_msg;
}

message decodeMessage(message msg, char *encoded_msg){
	sscanf(encoded_msg, "%d %s %d %d %s %d %hu %s", &msg.origin.ID, msg.origin.IP, &msg.origin.socket, &msg.destination.ID, msg.destination.IP, &msg.destination.socket, &msg.ack, msg.text);
	return msg;
}

void receiveMessage(mainSettings *main_settings, char *buf){
	int slen = sizeof(main_settings->si_other);

	fflush(stdout);
	//receive a reply and print it
	//clear the buffer by filling null, it might have previously received data
	memset(buf,'\0', BUFLEN);

	//try to receive some data, this is a blocking call
	if ((main_settings->recv_len = recvfrom(main_settings->s, buf, BUFLEN, 0, (struct sockaddr *) &main_settings->si_other, (socklen_t *)&slen)) == -1)
			die("recvfrom()");
		
	// Decode the message
	message msg;
	msg = decodeMessage(msg, buf);

	if(msg.destination.ID == main_settings->me.ID){
		// Test if it's an ack message
		if(!msg.ack){
			printf("\nReceived packet from (%d, %s, %d)", msg.origin.ID, msg.origin.IP, msg.origin.socket);
			printf("\n   Data: %s\n" , msg.text);

			// Now reply the client with ack
			proc aux;
			aux = msg.destination;
			msg.destination = msg.origin;
			msg.origin = aux;
			msg.ack = TRUE;
			buf = encodeMessage(msg, buf);
			if(sendto(main_settings->s, buf, BUFLEN, 0, (struct sockaddr*) &main_settings->si_other, slen) == -1)
				die("sendto()");
		}
		else if(main_settings->waiting_ack)
			main_settings->waiting_ack = FALSE;
	}
	else{
		printf("\nForwarding message:");
		printf("\n   From (%d, %s, %d) to (%d, %s, %d)", msg.origin.ID, msg.origin.IP, msg.origin.socket, msg.destination.ID, msg.destination.IP, msg.destination.socket);
		fowardMessage(main_settings, msg.destination.ID, buf);
	}
}

void sendMessage(mainSettings *main_settings, int destination_ID, char *buf, char *msg_text){
	int timeouts = 0;
	int slen = sizeof(main_settings->si_other);

	memset((char *) &main_settings->si_other, 0, sizeof(main_settings->si_other));

	path destination;
	for(int i=0;i<main_settings->len;i++)
		if(main_settings->path_table[i].destination.ID == destination_ID)
			destination = main_settings->path_table[i];

	main_settings->si_other.sin_family = AF_INET;
	main_settings->si_other.sin_port = htons(destination.next_proc.socket);

	if (inet_aton(destination.next_proc.IP, &main_settings->si_other.sin_addr) == 0) {
		fprintf(stderr, "inet_aton()");
		exit(1);
	}
	// Encode the message
	message msg;
	msg.origin = main_settings->me;
	msg.destination = destination.destination;
	msg.ack = FALSE;
	strcpy(msg.text, msg_text);
	char *encoded_msg;
	encoded_msg = encodeMessage(msg, encoded_msg);

	printf("\nSending message: ");
	do{
		// Send the message
		main_settings->waiting_ack = TRUE;
		if(sendto(main_settings->s, encoded_msg, strlen(encoded_msg), 0, (struct sockaddr *) &main_settings->si_other, slen) < 0)
			die("sendto()");

		int cont = 1;
		while(cont++ != TIMEOUT_MS && main_settings->waiting_ack)
			if(!main_settings->waiting_ack)
				break;
			else
				usleep(1000);

		if(main_settings->waiting_ack){
			timeouts++;
			printf("\n   Timeout reached, sending again!");
		}
	}while(timeouts < TIMEOUT_MAX && main_settings->waiting_ack);

	if(!main_settings->waiting_ack)
		printf("\n   Ack confirmed!\n\n");
	else{
		main_settings->waiting_ack = FALSE;
		printf("\n   Send aborted!\n\n");
	}
}

void fowardMessage(mainSettings *main_settings, int destination_ID, char *encoded_msg){
	int slen = sizeof(main_settings->si_other);

	memset((char *) &main_settings->si_other, 0, sizeof(main_settings->si_other));

	path destination;
	for(int i=0;i<main_settings->len;i++)
		if(main_settings->path_table[i].destination.ID == destination_ID)
			destination = main_settings->path_table[i];

	printf("\n   Sending to (%d, %s, %d)\n", destination.next_proc.ID, destination.next_proc.IP, destination.next_proc.socket);

	main_settings->si_other.sin_family = AF_INET;
	main_settings->si_other.sin_port = htons(destination.next_proc.socket);

	if (inet_aton(destination.next_proc.IP, &main_settings->si_other.sin_addr) == 0) {
		fprintf(stderr, "inet_aton()");
		exit(1);
	}

	// Send the message
	if (sendto(main_settings->s, encoded_msg, strlen(encoded_msg) , 0 , (struct sockaddr *) &main_settings->si_other, slen)==-1)
		die("sendto()");
}
