#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>

#pragma comment(lib, "Ws2_32.lib")

#define NO_ERROR 0

int main(int argc, char *argv[])
{

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }

    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrServer.sin_port = htons(8080);

    iResult = connect(ConnectSocket, (SOCKADDR *)&addrServer, sizeof(addrServer));
    if (iResult == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        printf("Unable to connect to server: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    char buf[1024 + 1];
    int count; //_read(0, buf, 1024);
    count = recv(ConnectSocket, buf, 1024, 0);
    printf("%s", buf);
    while (1)
    {
        count = _read(0, buf, 1024);
        if (count <= 0)
            break;
        int sendCount, currentPosition = 0;
        while (count > 0 && (sendCount = send(ConnectSocket, buf + currentPosition, count, 0)) != SOCKET_ERROR)
        {
            count -= sendCount;
            currentPosition += sendCount;
        }
        if (sendCount == SOCKET_ERROR)
            break;

        count = recv(ConnectSocket, buf, 1024, 0);
        if (count == 0)
            break;
        if (count == SOCKET_ERROR)
            break;
        buf[count] = '\0';
        printf("%s", buf);
    }
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}