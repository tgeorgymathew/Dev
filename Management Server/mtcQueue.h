#ifndef MTCQUEUE_H_INCLUDED
#define MTCQUEUE_H_INCLUDED


struct MTCQueue{
    int isMTCActive;
    char mtc_IP[16];
    int mtc_Port;
    struct MTCQueue *next,*first, *initial;
};

typedef struct MTCQueue MTCQ;


MTCQ *createMTC(int status, char ip[16], int Port)
{
    sprintf(logdata, "INFO::Creating entry(%s:%d) for the Queue.", ip, Port);
    logger(logdata);
    MTCQ *temp = malloc(sizeof(struct MTCQueue));
    temp->isMTCActive = status;
    strcpy(temp->mtc_IP, ip);
    temp->mtc_Port = Port;
    temp->first = temp;
    temp->next = NULL;
    temp->initial = NULL;
    return temp;
}


MTCQ *initializeQueue()
{
    sprintf(logdata, "INFO::Initializing Queue.");
    logger(logdata);
    MTCQ *temp = malloc(sizeof(struct MTCQueue));
    temp->isMTCActive = INITIAL;
//    temp->mtc_IP = "\0";
//    temp->mtc_Port = NULL;
    temp->first = NULL;
    temp->next = NULL;
    temp->initial = temp;
    return temp;
}


int addMTCQueue(MTCQ *data, MTCQ *perm)
{
    if(perm->first)
    {
        perm = perm->first;
        while(1)
        {
            if(strcmp(data->mtc_IP, perm->mtc_IP) == 0)
            {
                sprintf(logdata, "INFO::Entry for %s:%d is already present and hence entry will be modified", data->mtc_IP, data->mtc_Port);
                logger(logdata);
                perm->isMTCActive = data->isMTCActive;
                perm->mtc_Port = data->mtc_Port;
                free(data);
                return MODIFIED;
            }
            if(perm->next == NULL)
                break;
            else
                perm = perm->next;
        }

        while(perm->next != NULL)
        {
            perm = perm->next;
        }
        perm->next = data;
        data->first = perm->first;
        data->initial = perm->initial;
    }
    else if(!perm->first)
    {
        data->initial = perm->initial;
        perm->initial->next = data;
        perm->initial->first = data;
        data->first = data;
    }

    sprintf(logdata, "INFO::Successfully added entry for %s:%d to the queue.\n", data->mtc_IP, data->mtc_Port);
    logger(logdata);
    return SUCCESS;
}

/* Delete Non Ative MTCs.
Return Value: NULL(if first MTC entry is the only entry, and there exist no queue after deleting this entry.) OR <First Entry> First entry of the queue */
MTCQ *deleteNotActiveMTC(MTCQ *perm)
{
    MTCQ *temp;
    int cnt = 0;
    sprintf(logdata, "INFO::Delete process for non-Active MTCs Started.");
    logger(logdata);
    if(perm->first)
    {
        /*Verify and delete if the first MTC is not active.*/
        perm = perm->first;
        while(perm == perm->first && perm->isMTCActive == NO)
        {
            temp = perm;
            sprintf(logdata, "INFO::Processing delete for non active MTC : %s::%d", temp->mtc_IP, temp->mtc_Port);
            logger(logdata);
    //        printf("Processing delete if non active: %d::%s::%d\n", temp->isMTCActive, temp->mtc_IP, temp->mtc_Port);
            /* Check if there is next entry in the queue.
            If not then free the only entry in queue and return NULL, declaring no queue left. */
            if(perm->next)
                perm = perm->next;
            else
            {
                sprintf(logdata, "INFO::Deleting non active MTC: %s::%d", temp->mtc_IP, temp->mtc_Port);
                logger(logdata);
                perm->initial->first = NULL;
                perm->initial->next = NULL;
                free((void *)temp);
                sprintf(logdata, "INFO::Delete process for non-Active MTCs completed.\n");
                logger(logdata);
                return perm->initial;
            }
            /* Delete/Free the current entry and assign next entry as the first entry. Log the process in log file. */
            while(1)
            {
                perm->first = temp->next;
                if(!perm->next)
                {
                    perm->initial->first = temp->next;
                    break;
                }
                perm = perm->next;
            }
            sprintf(logdata, "INFO::Deleting non active MTC: %s::%d", temp->mtc_IP, temp->mtc_Port);
            logger(logdata);
    //        printf("Deleting non active: %d::%s::%d\n", temp->isMTCActive, temp->mtc_IP, temp->mtc_Port);
            free((void *)temp);
            cnt = cnt + 1;
            perm = perm->first;
        }

        //Deleting the non-first MTC entry which are non-Active.
        while(1)
        {
            if(perm->next != NULL && perm->next->isMTCActive == NO)
            {
                temp = perm->next;
                perm->next = temp->next;
                temp->next = NULL;
                temp->first = NULL;
                sprintf(logdata, "INFO::Deleting non active MTC: %s::%d", temp->mtc_IP, temp->mtc_Port);
                logger(logdata);
                free(temp);
                cnt = cnt + 1;
            }
            else if(perm->next != NULL && perm->next->isMTCActive == YES)
            {
                perm = perm->next;
            }
            else if(perm->next == NULL)
            {
                if(cnt == 0)
                {
                    sprintf(logdata, "INFO::No non-active MTCs found.");
                    logger(logdata);
                }
                sprintf(logdata, "INFO::Delete process for non-Active MTCs completed.\n");
                logger(logdata);
                break;
            }
        }
        perm = perm->first;
    }
    else if(!perm->first)
    {
        sprintf(logdata, "INFO::Queue is initialized but there is no MTC entry.");
        logger(logdata);
        perm = perm->initial;
    }
    return perm;
}

void printQueue(MTCQ *temp)
{
    if(temp->first)
    {
        temp = temp->first;
        sprintf(logdata, "INFO::Printing details of MTC Queue:");
        logger(logdata);
        while(1)
        {
            sprintf(logdata, "\t %d::%s::%d", temp->isMTCActive, temp->mtc_IP, temp->mtc_Port);
            logger(logdata);
 //           printf("1. %d \n2. %s \n3. %d \n", temp->isMTCActive, temp->mtc_IP, temp->mtc_Port);
            if(!temp->next)
                break;
            temp = temp->next;
        }
        sprintf(logdata, "INFO::Printed details of MTC Queue completed:\n");
        logger(logdata);
    }
    else
    {
        sprintf(logdata, "INFO::DB is initialized but there is no entry to print.");
        logger(logdata);
    }
}
#endif // MTCQUEUE_H_INCLUDED
