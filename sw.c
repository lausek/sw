#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/socket.h>

#define VERSION 0.1

#define IN_BUFFER_SIZE 256

#define SW_EOPEN -1
#define SW_EMISSING -2
#define SW_ENOFILE -3

void
help()
{
    printf("sw - socket write \n\
Version:    %.2f \n\
Usage:      sw file -m message [options] \n\
            prog | sw file [options]", VERSION);
    exit(0);
}

void
error(int code)
{
    if (!code)
        return;

    perror("Error");
    exit(1);
}

int
main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    char buffer[IN_BUFFER_SIZE];
    char *msg = NULL, *filename = NULL;
    int sock, in_buffer;

    if (argc < 2)
        help();
    
    for (int i = 1; i < argc; i++) {
        if (strcmp("-m", argv[i]) == 0) {
            if (i+1 < argc) {
                msg = argv[++i]; 
            } else {
                error(SW_EMISSING);
            }
        } else if (strcmp("--help", argv[i]) == 0) {
            help();
        } else {
            filename = argv[i]; 
        }
    } 
    
    if (filename == NULL)
        error(SW_ENOFILE);

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
        error(SW_EOPEN);
    
    memset(&addr, 0, sizeof(struct sockaddr_un));
    memcpy(addr.sun_path, filename, 108);
    addr.sun_family = AF_UNIX;

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)))
        error(errno);
    
    if (msg != NULL)
        write(sock, msg, strlen(msg));
    else {
        memset(buffer, 0, IN_BUFFER_SIZE);
        while ((in_buffer = fread(buffer, 1, IN_BUFFER_SIZE, stdin)))
            write(sock, buffer, in_buffer);
    }

    close(sock);
}
