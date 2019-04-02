#ifndef SERVERSOCKET_H_INCLUDED
#define SERVERSOCKET_H_INCLUDED


int server_socket(int port)
{
    int opt, sock;
    struct sockaddr_in *server1;

    server1 = malloc(sizeof(server1));
    server1->sin_family = AF_INET;
    server1->sin_addr.s_addr = INADDR_ANY;
    server1->sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("Socket creation failed. \n");
        exit(1);
    }


    if((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) < 0)
    {
        printf("Set Socket option failed. \n");
        exit(1);
    }

    if((bind(sock, (struct sockaddr *)server1, sizeof(struct sockaddr))) < 0)
    {
        printf("Error: Bind failed. \n");
        exit(1);
    }

    if((listen(sock, 30)) < 0)
    {
        printf("Error: Socket failed to listen.\n");
        exit(1);
    }

    return sock;
}

int mtcConnect(char *dstip, int port)
{
    int sock, opt;
    struct sockaddr_in *mtcip = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    char err_data[20];

    mtcip->sin_addr.s_addr = inet_addr(dstip);
    mtcip->sin_port = htons(port);
    mtcip->sin_family = AF_INET;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("MTC communication Socket creation failed. \n");
        exit(1);
    }

    struct timeval timeout;
    timeout.tv_sec = SOCKET_TIMEOUT_RECV;
    timeout.tv_usec = 0;
    int flag_1 = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    int flag_2 = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    int flag_3 = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if(flag_1 < 0 || flag_2 < 0 || flag_3 < 0)
    {
        printf("Setting MTC Socket option failed. \n");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *)mtcip, sizeof(*mtcip)) < 0)
    {
        if(errno == ECONNREFUSED)
            strcpy(err_data, "Connection Refused");
        else if(errno == ENETUNREACH)
            strcpy(err_data, "Network unreachable");
        else if(errno == EAGAIN)
            strcpy(err_data, "No entry in route cache.");
        else
            strcpy(err_data, "Error in connecting.");

        sprintf(logdata, "ERROR::Unable to connect to the MTC %s\n, error: %s", dstip, (char *)&err_data);
        logger(logdata);
        return FAILURE;
    }
    logger(logdata);
    return sock;
}

#endif // SERVERSOCKET_H_INCLUDED
