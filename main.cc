/*

network programming:
http://www.bogotobogo.com/cplusplus/sockets_server_client.php

style:
https://google.github.io/styleguide/cppguide.html
https://users.ece.cmu.edu/~eno/coding/CppCodingStandard.html
*/
#include <iostream>
#include "proxy.h"

using namespace std;

int main(int argc, char *argv[]){
    // parse argv

    HTTPProxy httpproxy(3500);
    
    // cout << "listening on port: " << httpproxy.GetPort() << endl;
    while(true){
        httpproxy.ProxyRequest();
    }

    return 0;
}