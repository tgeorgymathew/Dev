#ifndef SERVERPROCESS_H_INCLUDED
#define SERVERPROCESS_H_INCLUDED


void *server_run(int port)
{
    int server_sock, new_sock, status = FAILURE;
    int shm_fd;

    pthread_mutex_init(&lock, NULL);
    pthread_t kat;
    pthread_attr_t kat_attr;
    pid_t p;
    packet *data;
    MTCQ *mtcDB, *temp, *mtc_shared, *temp1;

    mtcDB = initializeQueue();

    //Create a thread to start keepalive process.
    //"status" default value is FAILURE but if keepalive process exits, which is less likely possible and it will return with value 'SUCCESS'.
    pthread_attr_init(&kat_attr);
    status = pthread_create(&kat, &kat_attr, (void *)mtcKeepaliveProcessing, (void *)mtcDB);
    printf("Print status :: %d. \n", status);

//    temp = createMTC(YES, "10.5.119.24", 2444);
//    status = addMTCQueue(temp, mtcDB);
    temp = createMTC(YES, "192.168.38.4", 61221);
    status = addMTCQueue(temp, mtcDB);

    //Start the server process t accept MTC/MIT connection and processing the data received.
    sprintf(logdata, "INFO: Server process started.");
    logger(logdata);
    server_sock = server_socket(port);

    struct sockaddr_in client_sock_info;
    int len = sizeof(client_sock_info);

    while(1)
    {
        if((new_sock = accept(server_sock, (struct sockaddr *)&client_sock_info, (socklen_t *)&len)) < 0)
        {
            if(errno == EFAULT)
            {
                sprintf(logdata, "ERROR::Unable to peer IP address because unable to write to the address space. Hence shutdown the server process.");
                logger(logdata);
            }
            else
            {
                sprintf(logdata, "ERROR::Unable to connect with the MTC/MIR %s:%d\n. Hence shutdown the server process.", inet_ntoa(client_sock_info.sin_addr), (int)client_sock_info.sin_port);
                logger(logdata);
            }

            exit(FAILURE);
        }

        //++++++++++++++++++++++++++++++++++++++++++
        shm_fd = shm_open("MTC", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
        if(shm_fd < 0)
        {
            sprintf(logdata, "ERROR::Unable to create FD for memory mapping.");
            logger(logdata);
            exit(FAILURE);
        }

        if(ftruncate(shm_fd, sizeof(MTCQ)) < 0)
        {
            sprintf(logdata, "ERROR::Unable to truncate FD for memory mapping.");
            logger(logdata);
            exit(FAILURE);
        }
        mtc_shared = (MTCQ *)mmap(NULL, sizeof(MTCQ), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(atoi((char *)mtc_shared) == MAP_SHARED)
        {
            sprintf(logdata, "ERROR::Unable to create memory mapping.");
            logger(logdata);
            exit(FAILURE);
        }
        bzero((void *)mtc_shared, sizeof(MTCQ));
        //++++++++++++++++++++++++++++++++++++++++++

        sprintf(logdata, "INFO::Request received from %s:%d\n..", inet_ntoa(client_sock_info.sin_addr), ntohs(client_sock_info.sin_port));
        logger(logdata);
        sprintf(logdata, "INFO::Processing to decide on MTC or MIR..");
        logger(logdata);
        data = recvData(new_sock);
        if(data)
        {
            p = fork();
            if(p == 0)
            {
                if(strcmp(data->type, "MTC") == 0)
                {
                    temp1 = (MTCQ *)MTC_Process(new_sock, data);
                    bzero((void *)mtc_shared, sizeof(MTCQ));
                    memcpy((void *)mtc_shared, (void *)temp1, sizeof(MTCQ));
                }
                else if(strcmp(data->type, "MIR") == 0)
                    status = MIR_Process(new_sock, data, mtcDB->initial);
                else
                {
                    sprintf(logdata, "ERROR::Invalid type received from the Client. The process cannot decide between MTC or MIR..");
                    logger(logdata);
                    exit(FAILURE);
                }
                sprintf(logdata, "INFO::Child process %d completed processing..", getpid());
                logger(logdata);
                exit(SUCCESS);
            }
            else if (p>0 && (strcmp(data->type, "MTC")) == 0)
            {
                waitpid(p, &status, 0);
                sprintf(logdata, "ERROR::Child process status is %d.", status);
                logger(logdata);
                if(status == 139 || status == 134)
                {
                    sprintf(logdata, "ERROR::Child process failed.");
                    logger(logdata);
                    exit(FAILURE);
                }
                sprintf(logdata, "INFO::Adding MTC to the Queue.");
                logger(logdata);
                temp1 = (MTCQ *)malloc(sizeof(MTCQ));
                bzero((void *)temp1, sizeof(MTCQ));
                memcpy((void *)temp1, (void *)mtc_shared, sizeof(MTCQ));
                addMTCQueue(temp1, mtcDB);
                munmap((void *)mtc_shared, sizeof(MTCQ));
                shm_unlink("MTC");
            }
            else if (p>0 && (strcmp(data->type, "MIR")) == 0)
            {
                sprintf(logdata, "INFO: Working on the MIR process");
                logger(logdata);
            }
        }
    }



//    temp = createMTC(YES, "172.16.16.16", 65099);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.17", 64094);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.18", 61030);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES    , "172.16.16.17", 60036);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.120", 61999);
//    status = addMTCQueue(temp, mtcDB);
//    sleep(20);
//    temp = createMTC(YES, "172.16.17.1", 60136);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.121", 60036);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.191", 31036);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.217", 6036);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.17", 4036);
//    status = addMTCQueue(temp, mtcDB);
//    sleep(30);
//    temp = createMTC(YES, "192.168.38.4", 61222);
//    status = addMTCQueue(temp, mtcDB);
//    temp = createMTC(YES, "172.16.16.191", 31036);
//    status = addMTCQueue(temp, mtcDB);


//    printQueue(mtcDB);
    printf("\n==========================================================================\n");

    sleep(10);
//    mtcDB = deleteNotActiveMTC(mtcDB);
//    printQueue(mtcDB);

    //Configuring keepalive process to update status for MTCs.
    while(1)
    {
        if(mtcDB->first)
        {
            printf("mtvDB is available.\n");
            sprintf(logdata, "INFO::mtvDB is available.");
            logger(logdata);
            sleep(5);
        }
        else if(!mtcDB->first)
        {
            printf("mtvDB is not available.\n");
            sprintf(logdata, "INFO::mtvDB is not available.");
            logger(logdata);
            sleep(5);
        }
    }

    //    int sock = server_socket(port);
//    if(sock == '\0')
//        exit(0);
//    while(1)
//    {
//        new_sock = accept(sock, NULL, NULL);
//        p = fork();
//        if(p > 0)
//        {
//            data = recvData(new_sock);
//            if(strcmp(data->type, "keepalives") == 0)
//            {
////                printf("Received data from client: %s :: %s :: %s :: %s :: %s\n", data->type, data->status, data->os, data->message, data->process);
//                bzero((void *)data, sizeof(data));
//                strcpy(data->type, "keepalives");
//                strcpy(data->status, "isActive");
//                strcpy(data->os, "win7");
//                strcpy(data->message, "SERVER_MESSAGE");
//                strcpy(data->process, "NIL");
//                sendData(new_sock, data);
//            }
//            else
//                return status;
//        }
//    }



    pthread_join(kat, NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}
#endif // SERVERPROCESS_H_INCLUDED
