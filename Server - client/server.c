#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h> //struct sockaddr_in
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <arpa/inet.h>
#define PORT 13331


void print_address(struct sockaddr_in addr)
{
    printf("{\n\t");
    printf(".sin_family :%d\n",addr.sin_family);
    printf("\t.sin_port :%d\n",addr.sin_port);
    printf("\t.sin_addr.s_addr :%d\n",addr.sin_addr.s_addr);
    printf("}\n");
}
int main()
{
    int main_socket_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_addr_int = 0;
    socklen_t len_of_server_addr = sizeof(server_addr);
    socklen_t len_of_client_addr = 0;
    if((main_socket_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
    {
        printf("socket error..\n");
        exit(0);
    }
    server_addr.sin_port = htons(12221);
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&server_addr_int);
    server_addr.sin_addr.s_addr = server_addr_int;
    print_address(server_addr);

    int con_fd = 0;
    con_fd = connect(main_socket_fd,(struct sockaddr *)&server_addr,len_of_server_addr);
    
        printf("%d",con_fd);
        print_address(client_addr);
        printf("\ncommunication started with server..\n\n");
        char buffer_in[512];
        char buffer_out[512];
        memset(buffer_in,0,sizeof(buffer_in));
        memset(buffer_out,0,sizeof(buffer_out));
        while( 1 )
        {
            printf("client : ");
            int len = 0;
            memset(buffer_in,0,sizeof(buffer_in));
            memset(buffer_out,0,sizeof(buffer_out));
            fflush(stdin);
            scanf("%[^\n]",buffer_out);
            if((len = send(main_socket_fd,buffer_out,512,0))<0)
            {
                printf("error while sending...\n");
                exit(1);
            }
            if((len = read(main_socket_fd,buffer_in,512))<0)
            {
                printf("error while reading..\n");
                exit(1);
            }
            if(strcmp(buffer_out,"exit") == 0)
            {
                printf("Bye Bye");
                exit(0);
            }
            printf("server : %s\n",buffer_in);
        }
}
