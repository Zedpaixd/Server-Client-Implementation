# Compiling

Due to the winsock2.h library, you will need a dependency when compiling, that one being **ws2_32** however you can just compile it from the terminal via MinGW with the following command:

g++ name.cpp -o name -lws2_32
