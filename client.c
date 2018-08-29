#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
void *recvmg(void *sock)
{
    int their_sock = *((int *)sock);
    char msg[500];
    int len;
    while((len = recv(their_sock,msg,500,0)) > 0)               // receive msg from their_sock with 500 length
    {
        msg[len] = '\0';
        // put NULL into the msg string
        fputs(msg,stdout);                  // print msg out display
        memset(msg,'\0',sizeof(msg));         // fill sizeof(msg) byte of the memory area pointed to by msg with NULL
    }
}
int main(int argc, char *argv[])
{
    struct sockaddr_in their_addr;
    int sockfd;
    int their_sock;
    int their_addr_size;            // define size of address client
    int portno;                     // define port
    pthread_t sendt,recvt;
    char msg[500];
    char username[100];
    char res[600];
    int len;

    if(argc > 3)
    {
        printf("too many arguments");
        exit(1);
    }
    portno = atoi(argv[2]);                           // convert a string poited by pointer argv to int
    strcpy(username,argv[1]);                         // copying the string pointed by argv pointer to usename destination

    // create a socket
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    memset(their_addr.sin_zero,'\0',sizeof(their_addr.sin_zero));       // fill sizeof(their_addr.sin_zero) byte NULL into ther_addr.sin_zero memory
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(portno);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sockfd,(struct sockaddr *)&their_addr,sizeof(their_addr)) < 0)      // request connect to server
    {
        perror("connection not established");
        close (sockfd);
        exit(1);
    }
    pthread_create(&recvt, NULL, recvmg, &sockfd);    // create a new thread
    printf("Start chatting:\n");
    while(fgets(msg,500,stdin) > 0)                  // get string from keyboard with size 500 and return msg string on success
    {
        strcpy(res,username);                       // copying the string pointed by username to res destination
        strcat(res,":");                            // // append the  ":" string  in the res destination string
        strcat(res,msg);                            // append the msg string in the res destination string
        len = write(sockfd,res,strlen(res));        // strlen(res) tra ve chieu dai cua chuoi res
        if(len < 0)
        {
            perror("message not sent");
            exit(1);
        }
        memset(msg,'\0',sizeof(msg));
        memset(res,'\0',sizeof(res));
    }
   pthread_join(recvt, NULL);                      // waiting for thread recvt exit and get the result into void **NULL
    close(sockfd);
}
