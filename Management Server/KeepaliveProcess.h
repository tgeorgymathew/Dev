#ifndef KEEPALIVEPROCESS_H_INCLUDED
#define KEEPALIVEPROCESS_H_INCLUDED

int k_process(MTCQ *db, packet *pkt)
{
    int sock;
    sprintf(logdata, "INFO::Checking for non-Active MTCs.");
    logger(logdata);
    int cnt = 0;
    while(1)
    {
        sprintf(logdata, "INFO::Checking %s:%d.", db->mtc_IP, db->mtc_Port);
        logger(logdata);
        sock = mtcConnect(db->mtc_IP, db->mtc_Port);
        if(sock == FAILURE)
        {
            db->isMTCActive = NO;
        }

        if(sock != FAILURE)
        {
            if(sendData(sock, pkt) == FAILURE)
            {
                sleep(3);
                cnt = cnt + 1;
                close(sock);
                if (cnt < 3)
                    continue;
                else
                {
                    db->isMTCActive = NO;
                }
            }
            else
            {
                while(1)
                {
                    if((pkt = recvData(sock)) == NULL)
                    {
                        db->isMTCActive = NO;
                        break;
                    }

                    if(strcmp(pkt->message, "KEEPALIVES_UPDATE") == 0)
                    {
                        if(strcmp(pkt->status, "BUSY") == 0)
                        {
                            db->isMTCActive = BUSY;
                            strcpy(pkt->status, "BUSY");
                        }
                        else if(strcmp(pkt->status, "ACTIVE") == 0)
                        {
                            db->isMTCActive = YES;
                            strcpy(pkt->status, "ACTIVE");
                        }
                        else
                        {
                            db->isMTCActive = NO;
                            strcpy(pkt->status, "NO");
                        }
                        strcpy(pkt->process, "SERVER_MSG");
                        strcpy(pkt->message, "KEEPALIVES_DONE");
                        sendData(sock, pkt);
                    }
                    else if(strcmp(pkt->message, "KEEPALIVES_DONE") == 0)
                    {
                        sprintf(logdata, "INFO::Closing the socket");
                        logger(logdata);
                        close(sock);
                        break;
                    }
                    else
                    {
                        sprintf(logdata, "ERROR::Invalid message '%s' received.", pkt->message);
                        logger(logdata);
                    }
                }
            }
        }


        cnt = 0;
        if(!db->next)
            break;
        db = db->next;
    }

    db = deleteNotActiveMTC(db);
    printQueue(db);
    return TRUE;
}

int mtcKeepaliveProcessing(MTCQ *db)
{
    packet *pkt = (packet *)malloc(sizeof(packet));
    int status;

    strcpy(pkt->type, "KEEPALIVES");
    strcpy(pkt->status, "NIL");
    strcpy(pkt->os, "NIL");
    strcpy(pkt->process, "SERVER_MSG");
    strcpy(pkt->message, "KEEPALIVES_UPDATE");

    sprintf(logdata, "INFO::Keepalives process started.");
    logger(logdata);
    while(1)
    {
        sprintf(logdata, "INFO::Keepalives process running.");
        logger(logdata);
        if(db->initial->first)
        {
            sprintf(logdata, "The status is %d:%d", db->first->isMTCActive, BUSY);
            logger(logdata);
            if(db->first->isMTCActive == YES)
                strcpy(pkt->status, "ACTIVE");
            else if(db->first->isMTCActive == BUSY)
                strcpy(pkt->status, "BUSY");
            else
                strcpy(pkt->status, "NIL");

            status = k_process(db->first, pkt);
            if(status == FALSE)
                break;
            sleep(KEEPALIVE_TIMEOUT);
        }
        else
            sleep(KEEPALIVE_TIMEOUT);
    }
    return SUCCESS;
}

#endif // KEEPALIVEPROCESS_H_INCLUDED
