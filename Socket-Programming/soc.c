#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <regex>

int socket_connect(char *host, in_port_t port);
void gethostandloc(char* url, char* host, char* loc);
 
#define BUFFER_SIZE 1024*1024 

int main(int argc, char *argv[]){
    int fd;
    char buffer[BUFFER_SIZE];
    FILE *fp;
    fp=fopen("output.html","w");
    if(fp==NULL)
    {
        printf("File Not Found");
    }
    if(argc < 3){
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(1); 
    }
    // char request[] = "GET / HTTP/1.1\r\n\r\nrandoms";
    //char request[] = "GET /index.html HTTP/1.0\r\nHost: 192.168.111.175\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.84 Safari/537.36\r\n\r\n";
    // char request[] = "GET /index.html?gfe_rd=cr&ei=k4q2V_aiOYjC8geu8ouIDQ HTTP/1.0\r\n\r\n";

    char host[256];
    char loc[256];
    bzero(loc,256);
    bzero(host,256);
    gethostandloc(argv[1],host,loc);
    char request[1000];
    bzero(request,1000);
    sprintf(request, "GET %s HTTP/1.0\r\nHost:%s \r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.84 Safari/537.36\r\n\r\n", loc,host);
    
    printf("%s%s\n", request,argv[2]);
    fd = socket_connect(host, atoi(argv[2])); 
    //send(fd,request, strlen(request),0);
    write(fd, request, strlen(request)); // write(fd, char[]*, len);  
    // write(fd, request, strlen(request)); // write(fd, char[]*, len);  
    bzero(buffer, BUFFER_SIZE);
    
    if(read(fd, buffer, BUFFER_SIZE - 1) != 0){
        printf("GOT A RESPONSE\n");
        fprintf(stderr, "%s", buffer);
        fprintf(fp, "%s\n", buffer);
        //fputs(buffer, fp);
        close(fd);
        bzero(buffer, BUFFER_SIZE); // clears buffer
    }
	
/*

	regex e ( "Content-Type.*?(?=^\\s*\n?\r?$)" );
	regex_search (s,m,e)

*/
    shutdown(fd, SHUT_RDWR); 
    //close(fd); 
    fclose(fp);
    system("google-chrome output.html");

    return 0;
}

int socket_connect(char *host, in_port_t port){
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;     

    if((hp = gethostbyname(host)) == NULL){
        herror("gethostbyname");
        exit(1);
    }
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

    if(sock == -1){
        perror("setsockopt");
        exit(1);
    }
    
    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
        perror("connect");
        exit(1);

    }
    return sock;
}

void gethostandloc(char* url, char* host, char* loc)
{
    int i=0,check = 0,li = 0,hi = 0;
    while(url[i])
    {
      //printf("%c\n", url[i]);
      if(url[i] == ':')
      {
        check = 1;
        i +=3 ;
        hi=0;
        li=0;
      }
      if(url[i] == '/')
      {
        check = 2;
      }
      if(check != 2)
      {
        host[hi++] = url[i];
      }
      if(check == 2)
      {
        loc[li++] = url[i];
      }
      i++;
    }
}
