/*
 * Name : Encrypt/Decrypt Server
 * Authors : Venkat Nivas Nagarajan(#1448042), Sangeetha Selvaraj(#1448045), Abisheik Jayaraja Perumal (#1464440)
 * Functionality :
    The server uses TCP sockets to connect with the client. On getting a 5 tuple socked descriptor,  the server creates a process to serve the client. Encrypts/Decrypts messages or closes the connection based on user input. Closes the newsocket descriptor in the parent. Listens for new clients in the old socket descriptor. The encrypt/decrypt server works concurrently.
*/

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include    <strings.h>
#include 	<string.h>

#define SERV_TCP_PORT 6000

char	*pname;

	/*
	 str function is used to Encrypt the messages.
	 
	 1.reads each character in the message
	 2.converts them and adds 3 to their corresponding ASCII value
	 3.saves in the buffer to be sent to the client.
	 
	 Arguments : pointer to the string from the client(character pointer), length of the string (integer)
	 Return type : void
	*/
	void str(char *line, int length)
	{
		//prints the string in the server window.
		printf("\nEncrypting..\n");
		int i;
		/*
		loop through every character in the message, add three to its ascii value
		and save them in the array to be sent to the client.
		*/
		for(i=0; i< length; i++){
			line[i] = (char)(((int)line[i])+3);
			//printf("%c\n",line[i]);
		}
	}

	/*
	 str function is used to Decrypt the messages.
	 
	 1.reads each character in the message
	 2.converts them and subracts 3 to their corresponding ASCII value
	 3.saves in the buffer to be sent to the client.
	 
	 Arguments : pointer to the string from the client(character pointer), length of the string (integer)
	 Return type : void
	*/
	void dstr(char *line, int length)
	{
		//prints the string in the server window.
		printf("\nDecrypting..\n");
		int i;
		/*
		 loop through every character in the message, subtract three from its ascii value
		 and save them in the array to be sent to the client.
		 */
		for(i=0; i< length; i++){
			line[i] = (char)(((int)line[i])-3);
			
		}
	}

	/*
	 process_request function is used to receive/send messages from/to the client.
	 
	 1.Reads the messages from the client.
	 2.Calls the Encrypt/Decrypt function based on user input.
	 3.Sends the Encrypted/Decrypted and waits for input from the client.
	 4.
		a) Closes the connection if the user requests to quit.
		b) Does Step 3 if the user requests to Encrypt/Decrypt.
	 
	 Arguments : socket descriptor (integer)
	 Return type : int
	 */
	int process_request(int newsockfd)
	{
		int n,loop=1;
		char line[512],inopt;

		//loops until the user requests to close the connection.
		for ( ; ; ) {
			bzero(line,sizeof(line));
			n = read(newsockfd,line, 512);
			//Checks if the user wants to Encrypt.
			if (line[0] == 'a'){
				//prints the string in the server window.
				printf("\nReceived message for encryption from Client.");
				//Calls the str function to encrypt the user message.
				str(&line[1],strlen(line)-1);
				//prints the string in the server window.
				printf("\nSending Encrypted Message\n");
				//writes the encrypt message in the buffer to send through the TCP scoket.
				write(newsockfd,&line[1],strlen(line));
			}
			//Checks if the user wants to Decrypt.
			else if(line[0] =='b'){
				printf("\nReceived message for decryption from Client.");
				//Calls the dstr function to decrypt the user message.
				dstr(&line[1],strlen(line)-1);
				printf("\nSending Decrypted Message..\n");
				//writes the decrypt message in the buffer to send through the TCP scoket.
				write(newsockfd,&line[1],strlen(line));
			}
			else{
				//Terminates the function when the user requests to quit.
				return 0;
			}
		}
	}

	/*
		 main function is used to connect to the client.
		 
		 1.Opens a TCP Socket.
		 2.Binds the address.
		 3.Wait for a client connection.
		 4.
			a) Creates a child process on receiving a client connection.
			b) Closes the old scoket in the child.
			c) Calls the process_request function to process clients request.
			d) Closes the new socket in the parent process. 
			e) The parent process listens for new connection.
			f) Do from 4-a through 4-e if new client connection. Else, 4-e.
			(NOTE: On getting a client connection, execution of child process or parent process depends on the kernel)
		 
		 NOTE: This is a concurrent server.
		 
		 Arguments : command line arguments (not used)
		 Return type : int
	*/
	int main (int argc, char *argv[])
	{
		int	sockfd, newsockfd, clilen, childpid;
		struct sockaddr_in	cli_addr, serv_addr;
		pname = argv[0];
	
		/* Open a TCP socket */

		if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			printf("server: can't open stream socket");
			return 1;
		}
		//Empty the server address structure.
		bzero((char *) &serv_addr, sizeof(serv_addr));
    
		//Assign the IP address of the server that is available.
		serv_addr.sin_family      = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port      = htons(SERV_TCP_PORT);

		/* Bind the local address to the socket.*/
		if (bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			printf("server: can't bind local address");
			return 1;
		}
		else{
			printf("\nEncrypt/Decrypt Server Up and Running....\n");
		}
		
		/*Listen to the socket with maximum queue length.*/
		listen(sockfd, 5);

		for ( ; ; ) {
			
			//Wait for a client connection.
			//Accept and form a 5 tuple socket descriptor on getting a new client request.
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
			
			//Throw an error if any issue in accepting a TCP connection from the client.
			if (newsockfd < 0) 	{
				printf("server: accept error");
				return 1;
			}
			else{
				//create a process for concurrent processing.
				if ( (childpid = fork() ) < 0){
					printf("\nFork Error\n");
				}
				else if (childpid == 0){
					//Close the old socket for the child process.
					close(sockfd);
					//Call the process_request to interact with the client using the new socket descriptor.
					process_request(newsockfd);
					return 0;
				}
				//Close the new socket descriptor in the parent process.
				close(newsockfd);
				//Continue looping to wait for a client.
			}
		}
	}

	









