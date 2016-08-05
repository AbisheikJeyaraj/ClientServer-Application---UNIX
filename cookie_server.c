/*
 * Name : Fortune Cookie Server
 * Authors : Venkat Nivas Nagarajan(#14480452), Sangeetha Selvaraj(#1448045), Abisheik Jayaraja Perumal (#1464440)
 * Functionality :
		The server uses TCP sockets to connect with the client. On getting a 5 tuple socked descriptor,  the server recieves the number of cookies to be sent to the client, sends the fortune messages, closes the connection and then accepts new connection. This server works iteratively.
*/

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include    <strings.h>
#include 	<string.h>

#define SERV_TCP_PORT 6000

char *pname;

	/*
		 process_request function is used to randomly send fortune messages to the client.
		 1.Generates random number using srand(time_t t).
		 2.Sends the number of cookies requested by the client randomly.
		 Arguments   : socket descriptor (integer)
		 Return type : int
	 */
	int process_request(int newsockfd){
		int n,loop=1,random_number;
		time_t t;
		char line[10000];
		char cookies[50][200] = 
			{	"Today it's up to you to create the peacefulness you long for.\n", 
				"A friend asks only for your time not your money.\n", 
				"If you refuse to accept anything but the best, you very often get it.\n",
				"A smile is your passport into the hearts of others.\n", 
				"A good way to keep healthy is to eat more Chinese food.\n", 
				"Your high-minded principles spell success.\n",
				"Hard work pays off in the future, laziness pays off now.\n", 
				"Change can hurt, but it leads a path to something better.\n", 
				"Enjoy the good luck a companion brings you.\n",
				"People are naturally attracted to you.\n", 
				"Hidden in a valley beside an open stream - This will be the type of place where you will find your dream.\n", 
				"A chance meeting opens new doors to success and friendship.\n",
				"You learn from your mistakes... You will learn a lot today.\n", 
				"If you have something good in your life, don't let it go!\n", 
				"What ever you're goal is in life, embrace it visualize it, and for it will be yours.\n",	
				"Your shoes will make you happy today.\n",
				"You cannot love life until you live the life you love.\n", 
				"Be on the lookout for coming events; They cast their shadows beforehand.\n", 
				"Land is always on the mind of a flying bird.\n",
				"The man or woman you desire feels the same about you.\n", 
				"Meeting adversity well is the source of your strength.\n", 
				"A dream you have will come true.\n", 
				"Our deeds determine us, as much as we determine our deeds.\n",
				"Never give up. You're not a failure if you don't give up.\n",	
				"You will become great if you believe in yourself.\n",	
				"There is no greater pleasure than seeing your loved ones prosper.\n", 
				"You will marry your lover.\n",
				"A very attractive person has a message for you.\n",	
				"You already know the answer to the questions lingering inside your head.\n", 
				"It is now, and in this world, that we must live.\n", 
				"You must try, or hate yourself for not trying.\n",
				"You can make your own happiness.\n", 
				"The greatest risk is not taking one.\n", 
				"The love of your life is stepping into your planet this summer.\n", 
				"Love can last a lifetime, if you want it to.\n", 
				"Adversity is the parent of virtue.\n",
				"Serious trouble will bypass you.\n", 
				"A short stranger will soon enter your life with blessings to share.\n", 
				"Now is the time to try something new.\n", 
				"Wealth awaits you very soon.\n", 
				"If you feel you are right, stand firmly by your convictions.\n",
				"If winter comes, can spring be far behind?\n", 
				"Keep your eye out for someone special.\n", 
				"You are very talented in many ways.\n", 
				"A stranger, is a friend you have not spoken to yet.\n", 
				"A new voyage will fill your life with untold memories.\n",
				"You will travel to many exotic places in your lifetime.\n", 
				"Your ability for accomplishment will follow with success.\n", 
				"Nothing astonishes men so much as common sense and plain dealing.\n",
				"Its amazing how much good you can do if you dont care who gets the credit.\n"
			};
		
		bzero(line,sizeof(line));
		n = read(newsockfd,line, 512);
	
		//Closes the connection if the client requests to quit on connecting.
		if (line[0] == 'q') 
			return 0;
		else{
			//Generates a random funtion using time.
			srand((unsigned) time(&t));
			printf("\nClient Requested %d cookies\n", atoi(line));
			printf("\nProcessing and sending %d cookies\n",atoi(line));
			
			//Sends the randomly generated cookie messages requested by the client using
			//the socket descriptor.
			while(loop <=atoi(line)){
				random_number = rand()% 50;
				loop++;	
				write(newsockfd,cookies[random_number],strlen(cookies[random_number]));
			}
		}
	}


	/*
		 main function is used to connect to the client.
		 1.Opens a TCP Socket.
		 2.Binds the address.
		 3.Wait for a client connection.
		 4.
			a) Call process_request function to process client request.
			b) Repeat Step 3 after processing client request.
		 NOTE: This is an iterative server.
		 Arguments : command line arguments (not used)
		 Return type : int
	*/
	int main (int argc, char *argv[]){
		
		int	sockfd, newsockfd, clilen, childpid;
		struct sockaddr_in	cli_addr, serv_addr;
		pname = argv[0];
		
		/* Open a TCP socket*/
		if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			printf("server: can't open stream socket");
			return 1;
		}
		else{
			printf("\nFortune Cookie Server up and running.\n");
		}
	
		//Empty the server address structure.
		bzero((char *) &serv_addr, sizeof(serv_addr));
		
		//Assign the IP address of the server that is available.
		serv_addr.sin_family      = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port      = htons(SERV_TCP_PORT);

		/* Bind our local address to the socket.*/
		if (bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			printf("server: can't bind local address");
			return 1;
		}
	
		/*Listen to the socket with maximum queue length.*/
		listen(sockfd, 5);
   
		for ( ; ; ) {
			
			//Wait for a client connection.
			//Accept and form a 5 tuple socket descriptor on getting a new client request.

			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
			
			//Throw an error if any issue in accepting a TCP connection from the client.
			if (newsockfd < 0) {
				printf("server: accept error");
				return 1;
			}
			else{
					//Call the process_request to interact with the client using the new socket descriptor.
					process_request(newsockfd);
			}
		
			//Close Socket Descriptor on processing the request.
			close(newsockfd);
			//Continue looping to wait for a client.
		}
	}

	