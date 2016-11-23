#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/time.h>
#include <netinet/tcp.h>

#define REQ_SZ 1000
#define BUFFER_SIZE 1024*5000
#define CACHE_SZ 5

void error(char *msg)
{
	perror(msg);
	exit(0);
}

// response, length, output(100 chars)
void getmimetype(char *r, int len, char mimet[100])
{
	int i = 0;
	char *temp = "Content-Type:";
	bzero(mimet, 100);
	int found = 0;
	while(1)
	{	
		found = 0;
		if(r[i] == 'C')
		{
			found = 1;
			for(int j = 0; j < strlen(temp); j++)
			{
				if(r[i] == temp[j])
				{
					i++;
				}
				else
				{
					found = 0;
					break;
				}
			}
		}
		if(found == 1)
		{	
			int ind = 0;
			i++;
			while(r[i] != ';')
			{
				mimet[ind] = r[i];
				i++;
				ind++;
			}

			return;
		}
		if(r[i] != 'C')
			i++;
		if(i >= len)
			break;
	}
	return;	
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
    printf("Socket connected successfully\n");
    return sock;
}

char *extractfilename(char *url, int len)
{
	if(len > 4)
	{
		int offset = 0;

		if(url[0] == 'w' && url[1] == 'w' && url[2] == 'w' && url[3] == '.')
		{
			offset = 4;
		}
		else
		{
			int ind = 0;
			while(url[ind] != ':' && ind < len)
			{
				ind++;
			}
			if(ind < len)
			{
	
				offset = ind+3;
				if(url[offset] == 'w' && url[offset+1] == 'w' && url[offset+2] == 'w' && url[offset+3] == '.')
					offset += 4;
			}

		}

		int i = offset; 
		while(i < len && url[i] != '/')
		{
			i++;
		}

		int filenamelength = len - i;
		char *filename = (char *)malloc(sizeof(char)*(filenamelength+1));

		int index = 0;
		while(i < len)
		{
			filename[index] = url[i];
			index++;
			i++;
		}
		filename[filenamelength] = '\0';
		return filename;
	}

	return "No file specified";
}

char *extracthostname(char *url, int len)
{
	char hostname[256];	//Temporary buffer to store the hostname;
	int hostnamelen;
	bzero((char *)&hostname, 256);

	if(len > 4)
	{
		int offset = 0;

		if(url[0] == 'w' && url[1] == 'w' && url[2] == 'w' && url[3] == '.')
		{
			offset = 4;
		}
		else
		{
			int ind = 0;
			while(url[ind] != ':' && ind < len)
			{
				ind++;
			}
			if(ind < len)
			{
				offset = ind+3;
				if(url[offset] == 'w' && url[offset+1] == 'w' && url[offset+2] == 'w' && url[offset+3] == '.')
					offset += 4;
			}

		}

		int i = 0; 
		while(i+offset < len && url[i+offset] != '/')
		{
			hostname[i] = url[i+offset];
			i++;
		}
		hostname[i] = '\0';
		hostnamelen = strlen(hostname);
		char *c = (char *) malloc((hostnamelen+1)*sizeof(char));
		for(int i = 0; i < hostnamelen; i++)
		{
			c[i] = hostname[i];
		}
		c[hostnamelen] = '\0';

		return c;
	}

	return "Illegal URL";
	
}

int main(int argc, char *argv[])
{
	int fd, portno;	//socket_file_descriptor, portnumber of server

	struct sockaddr_in serv_addr;
	struct hostent *server;
	char request[REQ_SZ];
	char buffer[BUFFER_SIZE];
	char *url = argv[1];
	int len = strlen(url);
	if(argc < 3)
	{
		if(argc < 2)
		{
			fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
			exit(1);
		}
		else
		{
			fprintf(stderr, "Usage: %s %s port\n",argv[0], url);
			exit(1);
		}
	}

	bzero(request, REQ_SZ);
	bzero(buffer, BUFFER_SIZE);
	
	char *hostname = extracthostname(url, len);
	char *filename = extractfilename(url, len);
	portno = atoi(argv[2]);

	printf("\nDomain %s\n", hostname);
	printf("\nPort Number: %d\n", portno);
	printf("\nPort Number in network byte order: %d\n", htons(portno));
	printf("\nFilename: %s\n", filename);

	//sprintf(request, "GET %s HTTP/1.0 \r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.84 Safari/537.36\r\n\r\n", url);

	sprintf(request,"GET %s HTTP/1.0\r\nHOST: %s\r\n\r\n", filename, hostname );

    fd = socket_connect(hostname, portno); 

	char* rootCacheDirectory = "/tmp/networkprogramming/";	//Path to the temporary cached files root directory
    
    int filenamelen = len < 255 ? len: 255;	//Maximum allowed filename length is 255 characters in LINUX

    //TODO 
    //Truncate the url to have length less than 256 for converting it to filename with proper extension

    char* cachedfilename = (char*)malloc(filenamelen+1);	//CachedFilename 
    
    //Replace all the forward slashes and colons with underscores to construct the filename
    for(int i = 0; i < filenamelen; i++)
    {
    	if(url[i] == '/' || url[i] == ':')
    		cachedfilename[i] = '_';
    	else
    		cachedfilename[i] = url[i];
    }

    cachedfilename[filenamelen] = '\0';

    char cachedfilepath[1000];	//Complete path to the cached file

    bzero(cachedfilepath, 1000);	//Initialize the character array with '\0'
    strncpy(cachedfilepath, rootCacheDirectory,strlen(rootCacheDirectory));
    strcat(cachedfilepath,cachedfilename);

    char mimetype[100];
    //Check if the file already exists in the CACHE
    if(access(cachedfilepath, F_OK) != -1)
    {
    	printf("\nThe file exists in the system CACHE.\n");
    	printf("\nThe complete path to the file is %s\n", cachedfilepath);
    	printf("\nRetreiving the existing file without any request to the server.\n");
    	
	} 
	else
	{
		write(fd, request, strlen(request)); 
	    bzero(buffer, BUFFER_SIZE);
	    char buffer[1024 * 1024];

		memset(buffer, 0, sizeof(buffer));
		char* pBuf = buffer;
		int bytesLeft = sizeof(buffer) - sizeof(char);

		bool readdone = false;
		while(bytesLeft > 0)
		{
		    int n = read(fd, pBuf, bytesLeft);
		    printf("These many bytes read : %d\n", n);
		    if (n < 0)
		    {
		        printf("%s\n","ERROR reading from socket");
		        break;
		    }
		    if(n == 0)
		    {
		        printf("%s\n","peer shutted down");
		        break;
		    }
		    pBuf += n;
		    bytesLeft -= n;
		    readdone = true;
		}

	    if(readdone)
	    {
	        printf("GOT A RESPONSE\n");
	        //fprintf(stderr, "%s", buffer);
		    getmimetype(buffer, strlen(buffer), mimetype);
		    printf("The mime type has been retreived successfully\n");
		    printf("The mime type is %s\n", mimetype);
	       	printf("\n The cached file path will be %s\n", cachedfilepath);

	       	int file_count = 0;
			DIR * dirp;
			struct dirent * entry;

			dirp = opendir("/tmp/networkprogramming/");
			while ((entry = readdir(dirp)) != NULL) 
			{
			    if (entry->d_type == DT_REG)
			    { /* If the entry is a regular file */
			         file_count++;
			    }
			}
			closedir(dirp);
		    if(file_count >= CACHE_SZ)
		    {
		    	if(system("cd /tmp/networkprogramming/")!= -1)
			    {
			    	printf("Changed the directory to the cache.\n");
			    }
			    else
			    {
			    	printf("Can't access the cache directory\n");
			    	exit(1);
			    }
		    	if(system("ls -dtr -1 $PWD/*.* | head -1 | xargs rm") != -1)
		    	{
		    		printf("%s\n", system("ls -dtr -1 $PWD/*.* | head -1"));
		    		printf("\nRemoved oldest file from the cache.\n");
		    	}
		    	else
		    	{
		    		printf("\nFailed to remove oldest file from the system.\n");
		    		printf("\nCache is full.\n");
		    		exit(1);
		    	}
		    }
		    printf("%s\n", "before");
		    int ci = 0;
		    while(ci < 10000)
		    {
		    	if(buffer[ci] == '\r' && buffer[ci+1] ==  '\n' && buffer[ci+2] == '\r' && buffer[ci+3] ==  '\n') break;
		    	ci++;
		    }
	        FILE *fp;
		    fp=fopen(cachedfilepath,"w+");
		    if(fp==NULL)
		    {
		        printf("Unable to create a new file\n");
		    }
		    //printf("Length of buffer: %s\n",strlen(buffer+ci+4));

			printf("%s %d \n", "after",strlen(buffer));
	        fput(buffer+ci+4, fp);
	    }
	    bzero(buffer, BUFFER_SIZE); // clears buffer
	}

	close(fd); 
	char command[4110];	//Command to open the file
	bzero(command, 4100);
	strcpy(command, "xdg-open ");
	strcat(command, cachedfilepath);	//Maximum allowed filepath length on linux is 4096 characters
	if(system(command) != -1)
	{
		printf("\nFile opened successfully.\n");
	}
	else
	{
		printf("\nError opening file.\n");
	}
	
	exit(0); 
    return (EXIT_SUCCESS);
	
}

