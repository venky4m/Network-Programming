/**
 * Author : Venkatesh Marripelly
 * File : server.c
 * 
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h> //struct sockaddr_in
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <arpa/inet.h>
#define PORT 12221

void print_address(struct sockaddr_in addr)
{
    printf("{\n\t");
    printf("addr.sin_family :%d\n",addr.sin_family);
    printf("\taddr.sin_port :%d\n",addr.sin_port);
    printf("\taddr.sin_addr.s_addr :%d\n",addr.sin_addr.s_addr);
    printf("}\n");
}

int main(int argc,char *argv[])
{   
    int main_socket_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_addr_int = 0;
    socklen_t len_of_sever_addr = sizeof(server_addr);
    socklen_t len_of_client_addr = 0;

    main_socket_fd = socket(AF_INET,SOCK_STREAM,0);
        
        //prepare the address of server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"192.168.0.108",&server_addr_int);
    server_addr.sin_addr.s_addr = INADDR_ANY;
        
    print_address(server_addr);
        
    //bind --> this function is to binding our main_socket to function on
    //specified server_addr
    if(bind(main_socket_fd,(struct sockaddr *)&server_addr,len_of_sever_addr) == -1)
    {
        perror("bind error\n");
        exit(1);
    }

    //the purpose of listen call is just to set the queue length
    //queue length is number of max of connection that can be placed in queue
    //so it is a non blocking system call

    if(listen(main_socket_fd,5) == -1)
    {
        perror("listen error\n");
        exit(1);
    }
    //connection came after this listen() are all be placed in queue
    //before listen() , any connection request will be dropped
    printf("listen done..\n");
    //accept() call take the one connection request from queue and give the address of that client
    //if no connection in already in queue ,then accept method block untill a new connection come
    //returns a new file descriptor referring to that socket
    /*
         int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        */
    int accept_fd = 0;
    while((accept_fd = accept(main_socket_fd,(struct sockaddr *)&client_addr,(socklen_t *)&len_of_client_addr))!=-1)
    {
        printf("A client connected with -> accept_fd = %d",accept_fd);
        print_address(client_addr);
        printf("\ncommunication started with client..\n\n");
        char buffer_in[512];
        char buffer_out[512];
        memset(buffer_in,0,sizeof(buffer_in));
        memset(buffer_out,0,sizeof(buffer_out));
        
        while( 1 )
        {
            //memcpy(buffer_out,"communicating");
            int len = read(accept_fd,buffer_in,512);
            if(len<=0)
            {
                printf("recv error\n..");
                exit(0);
            }
            if(strcmp(buffer_in,"exit") == 0)
            {
                printf("Bye Bye");
                exit(0);
            }
            printf("client : %s\n",buffer_in);
            printf("server : ");
            memset(buffer_in,0,sizeof(buffer_in));
            memset(buffer_out,0,sizeof(buffer_out));
            fflush(stdin);
            scanf("%[^\n]",buffer_out);
            //printf("%s",buffer_out);
            if((len = send(accept_fd,buffer_out,512,0))<0)
            {
                printf("seding error\n");
                exit(1);
            }           
        }
    }
    return 0;
}
