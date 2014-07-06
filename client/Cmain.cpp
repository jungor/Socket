#include "AgendaUI.h"

AgendaUI aui;

int main(int argc, char **argv) {
    if (argc != 2){
        printf("usage: ./client <ipaddress>\n");
        exit(1);
    }
    strncpy(aui.ipsrc, argv[1], strlen(argv[1]));
    aui.OperationLoop();
    return 0;
}
