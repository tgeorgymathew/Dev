#ifndef MTC_H_INCLUDED
#define MTC_H_INCLUDED

char* MTC_Process(int new_sock, packet *data)
{
    int status, i;
    char *s[2];
    s[0] = (char *)malloc(32);
    s[1] = (char *)malloc(sizeof(int));
    bzero(s[0], 32);
    bzero(s[1], sizeof(int));
    char *msg;

    MTCQ *temp;

    sprintf(logdata, "INFO: MTC process started.");
    logger(logdata);

    if(strcmp(data->message, "CLIENT_REGISTER") == 0)
    {
        strcpy(data->process, SERVER_IP_REQ);
        strcpy(data->message, SERVER_IP_REQ_MSG);
        status = sendData(new_sock, data);
    }

    while(1)
    {
        data = recvData(new_sock);
        sprintf(logdata, "DEBUG: I am here. %s", data->process);
        logger(logdata);
        if(strcmp(data->process, CLIENT_IP_RES) == 0)
        {
            msg = data->message;
            i = 0;
            while(i < 2)
            {
                sscanf(msg, "%[^||]s", s[i]);
                sprintf(logdata, "DEBUG: I am here 1. %s", s[i]);
                logger(logdata);
                msg = msg + strlen(s[i]) + strlen("||");
                i = i + 1;
            }
            temp = createMTC(YES, s[0], atoi(s[1]));
//            if(addMTCQueue(temp, db) == FAILURE)
//            {
//                sprintf(logdata, "INFO: MTC Status is %d.", status);
//                logger(logdata);
//            }

            strcpy(data->process,SERVER_IP_REQ_DONE);
            status = sendData(new_sock, data);
            if(status == FAILURE)
            {
                sprintf(logdata, "ERROR: Send data failed.");
                logger(logdata);
                exit(FAILURE);
            }
        }
        else if(strcmp(data->process, SERVER_IP_REQ_COMPLETED) == 0)
        {
            sprintf(logdata, "INFO: MTC IP:PORT is %s.", data->message);
            logger(logdata);
            break;
        }
        else
        {
            sprintf(logdata, "ERROR: MTC Error in the receiving packet");
            logger(logdata);
            exit(FAILURE);
        }
    }

    sprintf(logdata, "DEBUG: MTC details is %s:%d.", temp->mtc_IP, temp->mtc_Port);
    logger(logdata);
    return (char *)temp;
}

int mtc_malware_test(int sock, packet *data)
{
    sprintf(logdata, "INFO: Malware testing started");
    logger(logdata);
    strcpy(data->type, "MTC");
    strcpy(data->status, "BUSY");
    sendData(sock, data);
    char *temp_process;
    packet_var *temp = (packet_var *)malloc(sizeof(packet));

    memcpy((void *)temp->type, (void *)data->type, sizeof(data->type));
    memcpy((void *)temp->status, (void *)data->status, sizeof(data->status));
    memcpy((void *)temp->os, (void *)data->os, sizeof(data->os));
    temp_process = data->message;
    temp->message = temp_process;
    temp_process = data->process;
    temp->process = temp_process;
    sprintf(logdata, "DEBUG: Temp data is %s, %s, %s, %s, %s", temp->type, temp->status, temp->os, temp->message, temp->process);
    logger(logdata);

    while(1)
    {
        if(strcmp(data->process, "fileless") == 0)
        {
            sprintf(logdata, "DEBUG: Requested for Fileless attack");
            logger(logdata);
            sprintf(logdata, "DEBUG: Size of data is %d", (int)strlen(FILELESS_COMMAND));
            logger(logdata);

            temp_process = (char *)malloc(strlen(FILELESS_COMMAND)+1);
            bzero(temp_process, (size_t)strlen(FILELESS_COMMAND)+1);
            memcpy(temp_process, FILELESS_COMMAND, (size_t)strlen(FILELESS_COMMAND));
            sprintf(logdata, "DEBUG: New message is %s", temp_process);
            logger(logdata);

            temp->process = temp_process;
            sprintf(logdata, "DEBUG: Copied message to structure is %s", temp->process);
            logger(logdata);
            break;
        }
        else if(strcmp(data->process, "dbt") == 0)
        {
            sprintf(logdata, "DEBUG: Requested for Dynamic Behaviour attack");
            logger(logdata);
            sprintf(logdata, "DEBUG: Size of data is %d", (int)strlen(DBT_COMMAND));
            logger(logdata);

            temp_process = (char *)malloc(strlen(DBT_COMMAND)+1);
            bzero(temp_process, (size_t)strlen(DBT_COMMAND)+1);
            memcpy(temp_process, DBT_COMMAND, (size_t)strlen(DBT_COMMAND));
            sprintf(logdata, "DEBUG: New message is %s", temp_process);
            logger(logdata);

            temp->process = temp_process;
            sprintf(logdata, "DEBUG: Copied message to structure is %s", temp->process);
            logger(logdata);
            break;
        }
        else if(strcmp(data->process, "ds") == 0)
        {
            sprintf(logdata, "DEBUG: Requested for Document Script Attack");
            logger(logdata);
            sprintf(logdata, "DEBUG: Size of data is %d", (int)strlen(DOCUMENT_SCRIPTS_COMMAND));
            logger(logdata);

            temp_process = (char *)malloc(strlen(DOCUMENT_SCRIPTS_COMMAND)+1);
            bzero(temp_process, (size_t)strlen(DOCUMENT_SCRIPTS_COMMAND)+1);
            memcpy(temp_process, DOCUMENT_SCRIPTS_COMMAND, (size_t)strlen(DOCUMENT_SCRIPTS_COMMAND));
            sprintf(logdata, "DEBUG: New message is %s", temp_process);
            logger(logdata);

            temp->process = temp_process;
            sprintf(logdata, "DEBUG: Copied message to structure is %s", temp->process);
            logger(logdata);
            break;
        }
        else if(strcmp(data->process, "FAILURE") == 0)
        {
            free(temp_process);
            return FAILURE;
        }

        data = recvData(sock);
    }


    sprintf(logdata, "DEBUG: Temp data is %s, %s, %s, %s, %s", temp->type, temp->status, temp->os, temp->message, temp->process);
    logger(logdata);
    sendData_var(sock, temp);
    sprintf(logdata, "INFO: Malware testing completed");
    logger(logdata);

    data = recvData(sock);
    if(strcmp(data->process, "FAILURE")==0)
        return FAILURE;

    free(temp);
    free(temp_process);
    return SUCCESS;
}
#endif // MTC_H_INCLUDED
