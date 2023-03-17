#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int sd;
	int portNumber;
    int rc = 0;
    char userInput[100];
    char bufferOut[17];
    char bufferRead[100];
    char serverIP[29];
	struct sockaddr_in server_address;
    struct sockaddr_in fromAddress;
	socklen_t fromLength = sizeof(fromAddress);

    if (argc < 3){
        printf ("usage is client <ipaddr> <portnumber>\n");
        exit(1);
    }
    // create the socket
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    portNumber = strtol(argv[2], NULL, 10);
    strcpy(serverIP, argv[1]);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portNumber);
    server_address.sin_addr.s_addr = inet_addr(serverIP);

    // Used Dave's sample client, changed sequence input to string input
    printf("Enter the string you'd like to send. ");
	
	
	//scans the user input and stores as variable "userInput"
    scanf("%100[^\n]", userInput);
	//line reads the input and stores the string length as the message size
    int messageSize = strlen(userInput); 
	//used htonl keyword so that netowrk is able to read byte size
    int convertedMessageSize = htonl(messageSize);

    // Sends the size of the message to the server, got this line from project1
    rc = sendto(sd, &convertedMessageSize, sizeof(convertedMessageSize), 0, (struct sockaddr*)&server_address, sizeof(server_address));
    if (rc < 0)
        perror("write");

	// Sets windowSize number to 0
    int windowSize = 0; 
	int bytesLeft = messageSize; // wil use this in for loop in nested loop




	//Need an if statement that determines whether one or two bytes is sending based on how many bytes are left.
	//Using bytesLeft and a for loop, to go through the string to see how many bytes are left to determine whether will use one if or the other statement

    // nested for loop in order to continuously read string and increment bytes sent
    for(int i =0, j = 1; i < bytesLeft; i += 2, j += 2){
        if(j >= bytesLeft){
            memset(bufferOut, 0, 17);
            sprintf (bufferOut, "%11d%4d%1c", windowSize, 1, userInput[i]); 
            rc = sendto(sd, bufferOut, 16, 0,(struct sockaddr *) &server_address, sizeof(server_address));

            memset (bufferRead, 0, 100);
            rc = recvfrom(sd, &bufferRead, 100, MSG_DONTWAIT, //From Dave announcement
                          (struct sockaddr *)&fromAddress, &fromLength);

			//This allows for the windowSize to keep sending if there is an ack from server
            if (rc > 0){ 
                windowSize += 2;
                continue;
            }
 
            i -= 2;
            j -= 2;
        }
		//// This is when the there is 2 bytes available that're allowed to send 
        else{
            memset(bufferOut, 0, 17); // buffer is set to 17 because of requirements 
            sprintf (bufferOut, "%11d%4d%1c%1c", windowSize, 2, userInput[i], userInput[j]); // Put header and data into send buffer, send to server
            rc = sendto(sd, bufferOut, 17, 0,(struct sockaddr *) &server_address, sizeof(server_address));

            memset (bufferRead, 0, 100);
            rc = recvfrom(sd, &bufferRead, 100, MSG_DONTWAIT,
                          (struct sockaddr *)&fromAddress, &fromLength);

            if (rc > 0){ 
                windowSize += 2;
                continue;
            }
            i -= 2;
            j -= 2;
        }

    }
	
	
	
  /*int sendStuff(int sd, struct sockaddr_in server_address){
  int rc = 0;                       
  char bufferOut[16];
  char bufferIn[1000];
  int sequenceNumber = 0;
  int randomNumber = 19;
  size_t length;
  char *bPtr = bufferIn; // don't ask. sometimes i love indirection...
 
  }
  
  memset(bufferOut, 0, 16); // clear the buffer
  // now put the data into the buffer
  sprintf (bufferOut, "%11d%4d", sequenceNumber,
   randomNumber);
  // hardcoding (don't do this) that the packet is 15 bytes in length
  rc = sendto(sd, bufferOut, 15, 0,
  (struct sockaddr *) &server_address, sizeof(server_address));
  
  if (rc < 0)
    perror ("sendto");
  return 0; 
}*/
	
	
	
	
	
}