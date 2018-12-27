#include"../include/http.h"

/*Receive and responsed to http request*/
void httpreq(int connfd, char* ROOT)
{
	struct stat buf;
	char mesg[MAXLINE], *reqhdr[3], data_to_send[BYTES], path[MAXLINE];
	int rcvd, fd, bytes_read;

	memset( (void*)mesg, (int)'\0', MAXLINE );

	rcvd=recv(connfd, mesg, MAXLINE, 0);

	if (rcvd<0)    /*receive error*/
		fprintf(stderr,"SERVER: Error calling recv()\n");
	else if (rcvd==0)    /*receive socket closed*/
		fprintf(stderr,"SERVER: Client disconnected upexpectedly.\n");
	else    /*request message received*/
	{
		reqhdr[0] = strtok (mesg, " \t\n");
		if ( strcasecmp(reqhdr[0], "GET\0")==0 || strcasecmp(reqhdr[0], "HEAD\0")==0 )
		{
			reqhdr[1] = strtok (NULL, " \t");
			reqhdr[2] = strtok (NULL, " \t\n");

			strcpy(path, ROOT);
			strcpy(&path[strlen(ROOT)], reqhdr[1]);
			
			if ( (stat(path, &buf))==0 )    /*File found*/
			{
				if (!(S_ISREG(buf.st_mode)) || !(S_IRUSR & buf.st_mode))	/*Not have permission to access*/
				{
		    		clienterror(connfd, path, "403", "Forbidden",
						"The server couldn't read the file");
				}
				else
				{
					fd=open(path, O_RDONLY);
					send(connfd, "HTTP/1.0 200 OK\n\n", 17, 0);
					while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
					{
						write(connfd, data_to_send, bytes_read);
					}
				}
			}
			else    /*File not found*/
			{
	    		clienterror(connfd, path, "404", "Not Found",
					"The server couldn't find the file");
			}
		}
		else
		{
	    	clienterror(connfd, reqhdr[0], "501", "Not Implemented",
				"The server wasn't implement this method");
		}
	}
}

void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXLINE];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    write(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    write(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    write(fd, buf, strlen(buf));
    write(fd, body, strlen(body));
}
