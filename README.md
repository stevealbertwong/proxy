# Proxy

Compile
```
clang++ -x c++ main.cpp proxy.cpp -x c proxy_parse.c -o http-proxy

g++ -g -Wall -Werror -o proxy.o -c proxy.cpp
g++ -g -Wall -Werror -o proxy_parse.o -c proxy_parse.c
g++ -g -Wall -Werror -o main.o -c main.cpp
g++ -g -Wall -Werror -o proxy proxy_parse.o proxy.o main.o
```

---

## Testing 

1. set up linux server as proxy server (code only compile on linux machine)

port forwarding if set up your own computer as web server at home. check inet addr desingated by home router ifconfig e.g. 192.168.0.6. then set your router to open http port 80(http://www.instructables.com/id/Set-up-your-very-own-Web-server/)

google external ip to check your own home server's external ip address

2. 

download Firefox on client for any os(http://www.mozilla.org/en-US/firefox/new/)
 
 Once you download and launch Firefox, you can configure it (on Macs) to connect to the Internet through *proxy* by launching *Preferences* from the Apple menu, selecting *Advanced*, selecting *Network* within *A**dvanced*, selecting *Connection* within *Network*, and then activating a manual proxy (On Windows, proxy settings can be configured by selecting *Tools → **Options*).

 query proxy home server!
