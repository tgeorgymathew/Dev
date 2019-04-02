#ifndef MIR_H_INCLUDED
#define MIR_H_INCLUDED


char* get_available_mtc(MTCQ *db)
{
    if(!db->initial)
        return NULL;

    while(1)
    {
        if(db->isMTCActive == YES)
            break;
        if(!db->next)
            return NULL;
        db = db->next;

    }

    return (char *)db;
}

//Main process to serve the MIR request for the Malware testing.
//int MIR_Process(int mir_sock, packet *data, MTCQ *db)
//{
//    MTCQ* temp;
//    sprintf(logdata, "INFO: MIR process started.");
//    logger(logdata);
//
//    while(1)
//    {
//        if(strcmp(data->status, MALWARE_TEST) == 0)
//        {
//            if(strcmp(data->process, MALWARE_TEST_REQ) == 0)
//            {
//                //Get the active mTC available from the database. No MTC is available.
//                temp = (MTCQ *)get_available_mtc(db->initial);
//                if(temp == NULL)
//                {
//                    strcpy(data->process, MTC_UNAVAILABLE);
//                    strcpy(data->message, "NIL");
//                    sendData(mir_sock,data);
//                    close(mir_sock);
//                    exit(FAILURE);
//                }
//
//                //Connect to the MTC and reserve it for the use of Malware testing. During this time MTC should make itself busy for other requests.
//
//
//                strcpy(data->process, GET_MALWARE_PROCESS);
//                if(sendData(mir_sock, data) == FAILURE)
//                {
//                    sprintf(logdata, "INFO: MIR process failed.");
//                    logger(logdata);
//                    close(mir_sock);
//                    exit(FAILURE);
//                }
//
//            }
//            else if(strcmp(data->process, MALWARE_TEST_RES) == 0)
//            {
//                sprintf(logdata, "INFO: MIR processing.");
//                logger(logdata);
//                strcpy(data->process, MALWARE_TEST_DONE);
//                if(sendData(mir_sock, data) == FAILURE)
//                {
//                    sprintf(logdata, "INFO: MIR process failed.");
//                    logger(logdata);
//                    close(mir_sock);
//                    exit(FAILURE);
//                }
//            }
//            else if(strcmp(data->process, MALWARE_TEST_CLOSED) == 0)
//            {
//                sprintf(logdata, "INFO: MIR processing completed.");
//                logger(logdata);
//                close(mir_sock);
//                break;
//            }
//        }
//
//        bzero((void *)data, sizeof(packet));
//        data = recvData(mir_sock);
//    }
//
//
//    return SUCCESS;
//}

int __mtcStatusVerification(MTCQ *temp)
{
    sprintf(logdata, "INFO: MIR: MTC Verification started.");
    logger(logdata);
    int mtc_sock;
    packet *data = (packet *)malloc(sizeof(packet));
    mtc_sock = mtcConnect(temp->mtc_IP, temp->mtc_Port);
    if(mtc_sock == FAILURE)
    {
        sprintf(logdata, "INFO: MIR: MTC Verification Failed");
        logger(logdata);
        return FAILURE;
    }
    strcpy(data->type, "MTC");
    strcpy(data->status, "ACTIVE");
    strcpy(data->os, "NIL");
    strcpy(data->process, "TEST");
    strcpy(data->message, MALWARE_TEST);
    sendData(mtc_sock, data);
    data = recvData(mtc_sock);
    if(strcmp(data->status, "NO") == 0 || strcmp(data->status, "BUSY") == 0)
    {
        temp->isMTCActive = NO;
        sprintf(logdata, "INFO: MIR: MTC Verification Failed");
        logger(logdata);
        return FAILURE;
    }
    sprintf(logdata, "INFO: MIR: MTC Verification Successful");
    logger(logdata);
    return mtc_sock;
}


int __mir_request_process(int mir_sock, packet *data, MTCQ *db)
{
    int status, mtc_sock;
    MTCQ *temp;
    sprintf(logdata, "INFO: MIR: Processing MIR request.");
    logger(logdata);
    while(1)
    {
        if(db->isMTCActive == YES)
        {
            temp = db;
            logger(logdata);
            mtc_sock = __mtcStatusVerification(temp);

            if(mtc_sock)
            {
                status = YES;
                break;
            }
            else
            {
                if(db->next)
                    db = db->next;
                else
                {
                    status = NO;
                    break;
                }
            }
        }
        else
        {
            if(db->next)
                db = db->next;
            else
            {
                status = NO;
                break;
            }
        }
    }

    if(status == NO)
    {
        strcpy(data->type, "MIR");
        strcpy(data->status, "NIL");
        strcpy(data->message, MALWARE_TEST);
        strcpy(data->process, "NOT_AVAILABLE");

        sendData(mir_sock, data);

        close(mir_sock);
        exit(SUCCESS);
    }

    strcpy(data->type, "MIR");
    strcpy(data->status, "NIL");
    strcpy(data->message, MALWARE_TEST);
    strcpy(data->process, AVAILABLE);
    sendData(mir_sock, data);

    return mtc_sock;
}

int MIR_Process(int mir_sock, packet *data, MTCQ *db)
{
    int mtc_sock, status;
    char *req_msg;
    sprintf(logdata, "INFO: MIR Process Started.");
    logger(logdata);

    while(1)
    {
        if(strcmp(data->message, MALWARE_TEST)==0 && strcmp(data->process, "REQUEST")==0)
        {
            if(db->first)
            {
                sprintf(logdata, "INFO: MIR: MTC present in database.");
                logger(logdata);
                mtc_sock = __mir_request_process(mir_sock, data, db->first);
                req_msg = "TRACE_1";
            }
            else
            {
                strcpy(data->type, "MIR");
                strcpy(data->status, "NIL");
                strcpy(data->message, MALWARE_TEST);
                strcpy(data->process, "NOT_AVAILABLE");

                sendData(mir_sock, data);

                close(mir_sock);
                exit(FAILURE);
            }
        }
        else if(strcmp(data->message, MALWARE_TEST)==0 && strcmp(data->process, "CLOSING_SOCK")==0)
        {
            sprintf(logdata, "ERROR: MIR: MIR wants to close the connection probably due to INVALID_REQUEST");
            logger(logdata);
            close(mir_sock);
            sprintf(logdata, "ERROR: MIR: MIR connection closed");
            logger(logdata);
            exit(FAILURE);
        }
        else if(strcmp(data->message, MALWARE_TEST)==0 && strcmp(data->process, "PERFORM")==0)
        {
            if(strcmp(req_msg, "TRACE_1")==0)
            {
                sprintf(logdata, "INFO: Malware testing initiated");
                logger(logdata);
                data = recvData(mir_sock);
                status = mtc_malware_test(mtc_sock
                                          , data);
                if(status == SUCCESS)
                    strcpy(data->process, "COMPLETED");
                else
                    strcpy(data->process, "FAILURE");
                sendData(mir_sock, data);
                strcpy(req_msg, "TRACE_2");
            }
            else
            {
                strcpy(data->type, "MIR");
                strcpy(data->status, "NIL");
                strcpy(data->message, MALWARE_TEST);
                strcpy(data->process, INVALID_REQUEST);
                sendData(mir_sock, data);
                close(mir_sock);
                exit(FAILURE);
            }
        }
        else if(strcmp(data->message, MALWARE_TEST)==0 && strcmp(data->process, "DONE")==0)
        {
            if(strcmp(req_msg, "TRACE_2")==0)
            {
                close(mir_sock);
                sprintf(logdata, "INFO: MIR: Process completed successfully. Hence exiting from the MIR Process.");
                logger(logdata);
                exit(FAILURE);
            }
        }
        else
        {
            strcpy(data->type, "MIR");
            strcpy(data->status, "NIL");
            strcpy(data->message, MALWARE_TEST);
            strcpy(data->process, INVALID_REQUEST);
            sendData(mir_sock, data);
        }

        data = recvData(mir_sock);
    }

}

#endif // MIR_H_INCLUDED
