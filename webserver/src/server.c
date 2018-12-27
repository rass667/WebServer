#include<getopt.h>

#include"../include/http.h"

#define LISTENQ	8196   /*  Backlog for listen()   */

int main(int argc, char *argv[])
{
	int list_s;
	int conn_s;
	char port[6];
	int portnr;
	socklen_t addrlen;
	struct sockaddr_in servaddr;
	struct sockaddr_in clieaddr;
	char *ROOT;
	char option;

	/*Default Values ROOT = /lab2/www and PORT=8080*/
	chdir("../www");
	ROOT = getcwd(NULL,MAXLINE);
	portnr=8080;

	/*Parsing the command line arguments*/
	while ((option = getopt (argc, argv, "p:h")) != -1)
		switch (option)
		{
			case 'h':
				printf("\t-h \t\tPrint help text.\n");
				printf("\t-p port \tListen to port number port.\n");
				exit(1);
			case 'p':
				strcpy(port,optarg);
				portnr = atoi(port);
				break;
			case '?':
				fprintf(stderr, "Usage: %s [-h] [-p [argument]]\n", argv[0]);
				exit(1);
			default:
				exit(1);
		}

	/*  Create the listening socket  */
    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "SERVER: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(portnr);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */
    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
	fprintf(stderr, "SERVER: Error calling bind()\n");
	exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
	fprintf(stderr, "SERVER: Error calling listen()\n");
	exit(EXIT_FAILURE);
    }

    printf("Server started.\n");
    printf("Port: %d\n", portnr);
    printf("Root: %s\n", ROOT);

    while (1)
	{
		addrlen = sizeof(clieaddr);
		if ( (conn_s = accept(list_s, (struct sockaddr *)&clieaddr, &addrlen) ) < 0 ) {
		    fprintf(stderr, "SERVER: Error calling accept()\n");
		    exit(EXIT_FAILURE);
		}
		else
		{
			if ( fork()==0 )
			{
				close(list_s);
				httpreq(conn_s, ROOT);
				exit(0);
			}
			else
			{
				close(conn_s);
			}
		}
	}

	return 0;
}
