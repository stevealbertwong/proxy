# Proxy

Compile
```
// compile multiple sources to object then dynamically linked by g++ linker to executable

g++ -g -Wall -Werror -o proxy.o -c proxy.cpp
g++ -g -Wall -Werror -o proxy_parse.o -c proxy_parse.c
g++ -g -Wall -Werror -o main.o -c main.cpp
g++ -g -Wall -Werror -o proxy proxy_parse.o proxy.o main.o

// or simple make
make

// -x c++ -x c when compile both c++ and c sources tgt => -x c++ use c++ compiler 
clang++ -x c++ main.cpp proxy.cpp -x c proxy_parse.c -o http-proxy
```

---

## Testing with home linux server acting as proxy 

setting firefox browser to proxy to your own dynamic external ip at the time assigned by your router to your linux machine. then your code on linux machine proxy request to google server and proxy back to your client machine
(code only compile on linux machine)

1. port forwarding

192.168.0.1 to login to router to check port forwarding setting

port forwarding if set up your own computer as web server at home. check inet addr desingated by home router ifconfig e.g. 192.168.0.6. then set your router to open http port 80(http://www.instructables.com/id/Set-up-your-very-own-Web-server/)

google external ip to check your own home server's external ip address when setting up port forwarding

2. firefox proxy setting

download Firefox on client for any os(http://www.mozilla.org/en-US/firefox/new/)
 
 Once you download and launch Firefox, you can configure it (on Macs) to connect to the Internet through *proxy* by launching *Preferences* from the Apple menu, selecting *Advanced*, selecting *Network* within *A**dvanced*, selecting *Connection* within *Network*, and then activating a manual proxy (On Windows, proxy settings can be configured by selecting *Tools → **Options*).

www.stanford.edu/about/
(websites like google did not work cause request parsing library failed)
