#include "headers.h"

int main()
{
 //   MTCQ *temp, *perm_mtc;
    int port;

    port = 90;
//    packet *data = (packet *)malloc(sizeof(packet));
    logdata = (char *)malloc(LOGSIZE);

    server_run(port);

}
