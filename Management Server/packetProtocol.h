#ifndef PACKETPROTOCOL_H_INCLUDED
#define PACKETPROTOCOL_H_INCLUDED

//*******Changes needed to done on(if required):********
#define P_NMEMBERS 5
#define MEMBER_SIZE 30

#define DELIMITER "&&"

struct packetStructure
{
    //*******Changes needed to done on(if required):********
    char type[MEMBER_SIZE], status[MEMBER_SIZE], message[MEMBER_SIZE], process[MEMBER_SIZE], os[MEMBER_SIZE];
};

typedef struct packetStructure packet;

//Serializing data that needs to send over the socket.
char *serialize(packet *data)
{
    char *sd[P_NMEMBERS];
    char *sdata =(char *)malloc(sizeof(packet));
    int i = 0;

    //*******Changes needed to done on(if required):********
    sd[0] = data->type;
    sd[2] = data->status;
    sd[3] = data->message;
    sd[1] = data->os;
    sd[4] = data->process;

    bzero((void *)sdata, sizeof(packet));
    while(i < P_NMEMBERS)
    {
        strcat(sdata, sd[i]);
        if(i != P_NMEMBERS - 1)
            strcat(sdata, "&&");
        i = i + 1;
    }
    return sdata;
}

//Deserializing the data received from the socket.
packet *deserialize(char *sdata)
{
    packet *pdata = (packet *)malloc(sizeof(packet));
    int i;
    char *s[P_NMEMBERS];

    i = 0;
    while(i < P_NMEMBERS)
    {
        s[i] = (char *)malloc(MEMBER_SIZE);
        sscanf(sdata, "%[^&&]s", s[i]);
//        printf("Deserialize result s[%d]: %s.\n", i, s[i]);
        sdata = sdata+strlen(s[i]) + strlen(DELIMITER);
        i++;
    }

    //*******Changes needed to done on(if required):********
    strcpy(pdata->type, s[0]);
    strcpy(pdata->status, s[2]);
    strcpy(pdata->message, s[3]);
    strcpy(pdata->os, s[1]);
    strcpy(pdata->process, s[4]);

    i = 0;
    while(i < P_NMEMBERS)
    {
        free(s[i]);
        i++;
    }
    return pdata;
}

int sendData(int sock, packet *data)
{
    sprintf(logdata, "INFO::Sending following data:\n\t type:%s \n\t status::%s \n\t os::%s \n\t process::%s \n\t message::%s", data->type, data->status, data->os, data->process, data->message);
    logger(logdata);
    char *sdata = serialize(data);
    if(send(sock, (void *)sdata, strlen(sdata), 0) < 0)
    {
        free(sdata);
        return FAILURE;
    }
    free(sdata);
    return SUCCESS;
}

packet *recvData(int sock)
{
    int data_length;
    char *sdata = (char *)malloc(sizeof(packet));
    bzero((void *)sdata, sizeof(packet));


    data_length = recv(sock, (void *)sdata, sizeof(packet), 0);
    if (data_length <= 0)
    {
        sprintf(logdata, "ERROR::Data received is corrupted.");
        logger(logdata);
        return NULL;
    }
    sprintf(logdata, "INFO::Received data length is  '%d'", data_length);
    logger(logdata);
    sprintf(logdata, "INFO::Received data is  '%s'", sdata);
    logger(logdata);

    packet *pdata = deserialize(sdata);
    free(sdata);
    sprintf(logdata, "INFO::Received following data:\n\t type:%s \n\t status::%s \n\t os::%s \n\t process::%s \n\t message::%s", pdata->type, pdata->status, pdata->os, pdata->process, pdata->message);
    logger(logdata);
    return pdata;
}


struct packetStructure1
{
    //*******Changes needed to done on(if required):********
    char type[MEMBER_SIZE], status[MEMBER_SIZE], os[MEMBER_SIZE];
    char *message;
    char *process;
};

typedef struct packetStructure1 packet_var;

//Serializing data that needs to send over the socket.
char *serialize_var(packet_var *data)
{
    char *sd[P_NMEMBERS];
    char *sdata =(char *)malloc(sizeof(data));
    int i = 0, data_size = 0;

    sprintf(logdata, "DEBUG::Size of data to serialized is '%d'", (int)sizeof(data));
    logger(logdata);

    //*******Changes needed to done on(if required):********
    sd[0] = data->type;
    sd[2] = data->status;
    sd[3] = data->message;
    sd[1] = data->os;
    sd[4] = data->process;

    bzero((void *)sdata, sizeof(data));
    while(i < P_NMEMBERS)
    {
        data_size = strlen(sdata) + strlen(sd[i]);
        sdata = (char *)realloc(sdata, data_size);
        sprintf(logdata, "DEBUG::Size of data '%d' and Concatenating '%s'", data_size, sd[i]);
        logger(logdata);
        strcat(sdata, sd[i]);
        if(i != P_NMEMBERS - 1)
            strcat(sdata, "&&");
        i = i + 1;

        sprintf(logdata, "DEBUG::Concatenated data '%s'", sdata);
        logger(logdata);
    }

    return sdata;
}

int sendData_var(int sock, packet_var *data)
{
    sprintf(logdata, "INFO::Sending following data:\n\t type:%s \n\t status::%s \n\t os::%s \n\t process::%s \n\t message::%s", data->type, data->status, data->os, data->process, data->message);
    logger(logdata);
    char *sdata = serialize_var(data);
    sprintf(logdata, "DEBUG::Data to be send is '%s'", sdata);
    logger(logdata);
    if(send(sock, (void *)sdata, strlen(sdata), 0) < 0)
    {
        free(sdata);
        return FAILURE;
    }
    sprintf(logdata, "INFO::Send data sucessfull.");
    logger(logdata);
    free(sdata);
    return SUCCESS;
}

#endif // PACKETPROTOCOL_H_INCLUDED
