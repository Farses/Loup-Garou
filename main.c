#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "client.h"
#include  "server.h"



#pragma comment(lib, "ws2_32.lib")



int main() {
    // Selon votre besoin, décommentez l'une des lignes ci-dessous
    int var = 0 ;
    ALLEGRO_COLOR color = al_map_rgb(0,0,0 ) ;

    scanf("%d", &var) ;
    if(var == 1){
        client() ;
    }
    else {
        server();
    }
    return 0;
}