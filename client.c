/**
 * Name: Client 
 * Authors: Sangeetha Selvaraj(#1448045), Abisheik Jayaraja Perumal(#1464440), Venkat Nivas Nagarajan (#1448042)
 * Functionality: 
   The client program on execution will provide the options to connect to two different server or quit from the application.
   Client uses TCP connection protocol to connect with the servers.
   The two servers are Fortune cookie server and Encrypt\Decrypt server. 
   Once the client choose the server, client should also provide the IP address of the servers.
   1. Fortune Cookier Server: On choosing option a, client should provide the number of cookies needed.
   2. Encrypt\Decrypt server: On choosing option b, client will be provided with three options.
		a) encrypt: Client should provide the message which needs to be encrypted.
		b) decrypt: Client should provide the message which needs to be decrypted.
		c) quit: Client can clost the connection with encrypt/decrypt server.
   3. Quit: Client can quit from the application.
**/
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include    <strings.h>
#include    <string.h>
#define SERV_TCP_PORT 6000
char	*pname;

	/*
		str_cli function is to send and receive the encrypt/decrypt message to/from the server and display it to the user
		1. Clears the sendline buffer and copies the message from user to sendline buffer
		2. Read the message from recvline buffer and clears the buffer
		3. Prints the message received to the console and throws error if any problem in reading or writing in the buffer
		
		Arguments: File Pointer(pointer), Socket descriptor(integer), Selected menu option(pointer)
		Return type: int
	*/

	int str_cli(fp, sockfd,opt)
	int	sockfd;
	FILE *fp;
	char *opt;
	{
		long	n,n_read;
		char	ch[2],sendline[512],recvline[513],input[512];
	
		//Empty the input buffer
		bzero(input,sizeof(input));
		
		//Conacatenate the first character with the user selected option from the menu
		strcat(input,opt);
		
		//Empty the buffer used for encrypt/decrypt message from the user 
		bzero(sendline,sizeof(sendline));
		
		//Empty the buffer used for output from the server 
		bzero(recvline,sizeof(recvline));
		
		//Clear the input stream
		fgets(ch,2, fp);
		
		// Display message for client
		printf("\nEnter the Message:\n");
	
		//Gets the user input message for encrypt/decrypt
		fgets(sendline, 512, stdin);
		//concatenates the option chosen from the menu and the message from the user
		strcat(input,sendline);
		//Length of the input is assigned to variable n 
		n = strlen(input);
		// If the messgae is not written completely in the buffer, error is displayed and returns to the main menu
		if (write(sockfd, input, n) != n){
			printf("str_cli: writen error on socket");
		    return 1;
		}
		//Output of read function is assigned to n_read
		n_read = read(sockfd,recvline,512);
		
		//If the message is not read completely from the buffer, error is displayed
		if (n_read < 0) {
			printf("If");
			printf("readline error");
		}
		//Else If the message is read completely from the buffer, corresponding message is displayed
		else{
			if(opt[0]=='a'){
					printf("\nEncrypted Message is:");
			}
			else{
				printf("\nDecrypted Message is:");
			}
			//Display the encrypt/decrypt message from the server in the command prompt
			fputs(recvline,stdout);
			
			printf("\n");
			return 0;
		}
	}

	/*
		str_exit function is to exit from encrypt/decrypt server when user chose the option is 'c'
		
		1. Empty the sendline buffer and copies the character 'q' in sendline.
		2. Displays error if any error in writing to the buffer.
		
		Arguments: Socket descriptor(integer)
		Return type: int
	*/
	int str_exit(sockfd)
	int	sockfd;
	{
		char	sendline[512];
		// Empty the buffer used for user input message
		bzero(sendline,sizeof(sendline));
		//copies the character 'q' in the sendline buffer
		strcpy(sendline,"q");
		// Displays the closing message to the user
		printf("\nClosing connection with Encrypt/Decrypt Server\n");
		//Display error message if the message is not written completely in the server
		if (write(sockfd, sendline, 1) != 1){
			printf("str_cli: writen1 error on socket");
		    return 1;
		}
		return 0;
	}


	/*
		encrypt_client function is to display with Encrypt/Decrypt/Quit menus.
		1. 
			a) Calls the str_cli function to send messages for encryption or decryption.
			b) Quit the connection if the user requests.
		
		Arguments: Socket descriptor(integer)
		Return type: int
	*/
	void encrypt_client(int sockfd)
	{
		char opt[2],ch[2];
		// loop runs until the user choose the option to quit
		do{
			//Display the options to the user
			printf("\nWhat do you want to do ?");
			printf("\na.Encrypt \nb.Decrypt \nc.Quit\n");
			//Receive the input from user console
			fgets(opt,2, stdin);
			//If the option is a or b , str_cli function is called
			if ((opt[0] =='a') || (opt[0] =='b') ) {
					str_cli(stdin, sockfd,opt);	/* do it all */
			}
			//Else, if the option is c, str_exit function is called
			else if(opt[0] == 'c'){
				str_exit(sockfd);
			}
			else {
				//Else, if the option is any other than the provided option, error message is displayed
				printf("Please enter a valid entry");
			}
		}
		while(opt[0] != 'c');
		close(sockfd);
	}

	/*
		fortune_cookie function is to interact with the fortune cookie server.
		
		1. Send the client message to the server using the socket.
		2. Display the message from the server.
		3. Quit the connection .
		
		Arguments: Socket descriptor(integer)
		Return type: int
		
	 */
	int fortune_cookie(sockfd)
	int	sockfd;
	{
		long	n,n_read;
		char	ch[2],sendline[512],recvline[10000];
		int loop=0;
		//Empty the buffer used for input message from the user 
		bzero(sendline,sizeof(sendline));
		
		//Empty the buffer used for fortune cookies from the server
		bzero(recvline,sizeof(recvline));
		
		printf("How many cookies do you need?\n");
		
		//Read the input from the console
		fgets(sendline, 512, stdin);
		
		//Length of the input message is assigned to n
		n = strlen(sendline);
    
		// If the messgae is not written completely in the buffer, error is displayed and returns to the main menu
		if (write(sockfd, sendline, n) != n){
			printf("str_cli: writen error on socket");
			return 1;
		}
    
		//Loops according to number of cookies requested
		while(loop < atoi(sendline)){
			// Displays message on completion of reading all the fortune cookie message from the server
			if(loop==0){
				printf("\nFortune Cookies for the day:\n");
			}
			//Output of read fucntion is assigned to n_read
			n_read = read(sockfd,recvline,10000);
			//If the message is not read completely from the buffer, error is displayed
			if (n_read < 0) 
				printf("readline error");
			//If the message is read completely from the buffer, corresponding message is displayed
			else{
				recvline[n_read]= 0;
				fputs(recvline,stdout);
			}
			//Empty the buffer used for the output message from the server
			bzero(recvline,sizeof(recvline));
			//Increment the variable loop
			loop++;
		}
		//Empty the buffer used for the input message from the user
		bzero(sendline,sizeof(sendline));
		//Display message about disconnecting from cookie server
		printf("\nClosing connection with Fortune Cookie Server\n");
		//copies the character 'q' in the sendline buffer
		strcpy(sendline,"q");
		//If the message is not written completely into the buffer, error is displayed
		if (write(sockfd, sendline, n) != n)
		{
			printf("str_cli: writen1 error on socket");
			return 1;
		}
	}


	/*
		TCP connection to the server.
		
		Arguments: Server Name (Character)
		Return type: int
		1.Opens a TCP Socket.
		2.Binds the address.
		3.Connect to a server
		4.Call the function to interact with the appropriate server.
		
	 */
	int server_connect(char call_server)
	{
		int  sockfd;
		struct sockaddr_in	serv_addr;
		char opt[2],cl[2];
		char ip_addr[35];
		
		//Display message to get server IP address from the user
		
		printf("Enter Server IP Address:");
		scanf("%s",ip_addr);
	
		//Given IP address is assigned to variables in struct data type "serv_addr"  

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family 		= AF_INET;
		serv_addr.sin_addr.s_addr	= inet_addr(ip_addr);  
		serv_addr.sin_port 	= htons(SERV_TCP_PORT);
		
		//Open a TCP socket  
		if ((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
			printf("client: can't open stream socket");
			return 1;
		}	
		
		// Connect to the server using connect() method
		if (connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
		   printf("client: can't connect to server");
		   return 1;
		}
		
		fgets(cl,2, stdin);
		
		if(call_server == 'a'){
			fortune_cookie(sockfd);
		}	
		else if (call_server == 'b'){
			encrypt_client(sockfd);
		}
		else{
			printf("Not a valid Entry");
		}
		return 0;
	}


	/*
		 main function is used to Display the menus to the clients and call server connection function.
		 
		 1. Displays the menu to the client.
		 2. Call server_connect using the server name as argument.
		 3. Repeat Step 1 or quit.
		 
		 Arguments : command line arguments (not used)
		 Return type : int
	*/
	int main(int argc, char *argv[])
	{	
		char input[2],clear[2];
		pname = argv[0];
		//Loops until the user choose the option as 'C' from the menu
		do{
			//Display the message to the server
			printf("\nWhich server do you want to connect ?");
			printf("\na.Fortune Cookie Server \nb.Encrypt/DecryptServer \nc.Quit\n");
			// To get the input from the console
			fgets(input,2, stdin);
			// If option is 'a' or 'b', server_connect funtion is called
			if ((input[0] =='a') || (input[0] =='b') ) {
					server_connect(input[0]);
					if(input[0] =='b')
					fgets(clear,2, stdin);
					/* do it all */
			}
			//Else if the option is 'c', the funtion returns 0 and exit from the main program 
			else if(input[0] == 'c'){
				return 0;
			}
			// Else if the option is other than the option provided, error message is displayed
			else {
					printf("Please enter a valid entry");
			}
		}while(input[0] != 'c');
		return 0;
	}
