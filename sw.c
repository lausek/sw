#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/socket.h>

void
help()
{
    printf("Usage: sw [options] file message");
    exit(0);
}

void
error(int code)
{
    if (!code)
        return;

    printf("Error %d: ", code);
    switch (code) {
        case -1:
            printf("socket couldn't be opened"); break;
        case EACCES:
        case EPERM:
            printf("the address is protected and user is not root"); break;
        case EADDRINUSE:
            printf("the given address is already in use"); break;
        case ECONNREFUSED:
            printf("connection refused"); break;
        case EBADF:
            printf("socket is not a valid file descriptor"); break;
        case EFAULT:
            printf("address points outside the users space"); break;
        case ELOOP:
            printf("too many symbolic links in address"); break;
        case ENOENT:
            printf("insufficient kernel memory available"); break;
        case ENOMEM:
            printf("insufficient kernel memory available"); break;
        case ENOTDIR:
            printf("a component of the path is not a direcotry"); break;
        case EROFS:
            printf("the socket inode resides on a read-only filesystem"); break;
        default:
            printf("unknown error"); break;
    }

    exit(1);
}

int
main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sock;

    if (argc < 3)
        help();
    
    // TODO: validate arguments
    char *filename = argv[1];
    char *msg = argv[2];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
        error(-1);
    
    memset(&addr, 0, sizeof(struct sockaddr_un));
    memcpy(addr.sun_path, filename, 108);
    addr.sun_family = AF_UNIX;

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)))
        error(errno);

    write(sock, msg, strlen(msg));

    close(sock);
}
