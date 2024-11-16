// SES6-02b-UDP-S.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

int main()
{
    const int PORT = 8888; //Port auf den der Server hört
    const int MESSAGELENGTH = 512; //Max message Length

    SOCKET udpSocket; //Socket Objekt benötigt für socket()
    struct sockaddr_in serverAdress, clientAddress; //Structs mit denen wir die Server und Client Angaben speichern
    int slen, recv_len; // Variablen für sendto() und recvfrom()
    char message[MESSAGELENGTH]; //Daten die wir empfangen/versenden wollen
    WSADATA wsaData; //WSADATA Objekt gebraucht für WSAStartup()
    slen = sizeof(clientAddress); //grösse der clientAdresse

    // Initialise Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup fehlgeschlagen: " << WSAGetLastError() << endl;
        system("pause");
        return 1;
    }

    // Create udp socket
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        cout << "Socket erstellen fehlgeschlagen: " << WSAGetLastError() << endl;
        system("pause");
        return 1;
    }

    // Block 1
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_addr.s_addr = INADDR_ANY;
    serverAdress.sin_port = htons(PORT);

    // Block 2
    if (bind(udpSocket, (struct sockaddr *)&serverAdress, sizeof(serverAdress)) == SOCKET_ERROR)
    {
        cout << "Binding fehlgeschlagen: " << WSAGetLastError() << endl;
        system("pause");
        return 1;
    }
    while(1) {
        cout << "Server gestartet." << endl;
        cout << "Warte auf Daten:" << endl;

        // Block 3
        memset(message, '\0', MESSAGELENGTH); //Reserviert Memory als Buffer in welchem wir die Daten speichern
        if ((recv_len = recvfrom(udpSocket, message, MESSAGELENGTH, 0, (struct sockaddr *) &clientAddress, &slen)) ==
            SOCKET_ERROR) {
            cout << "Daten empfangen fehlgeschlagen: " << WSAGetLastError() << endl;
            system("pause");
            return 1;
        }
        cout << "Daten empfangen von " << inet_ntoa(clientAddress.sin_addr) << " " << ntohs(clientAddress.sin_port)
             << ": " << message << endl;
    }
    // Block 4
    closesocket(udpSocket);
    WSACleanup();

    system("pause");
    return 0;

}