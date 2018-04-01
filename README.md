# Multi-process Proxy server with caching and blacklist

# Table of Contents
&nbsp;[Build Instruction](https://github.com/stevealbertwong/proxy/blob/master/README.md#build-instructions)  <br/> 
&nbsp;[Versions](https://github.com/stevealbertwong/proxy/blob/master/README.md#versions)  <br/> 
&nbsp;[Set up testing](https://github.com/stevealbertwong/proxy/blob/master/README.md#set-up-testing)  <br/> 
&nbsp;[Pull request welcome](https://github.com/stevealbertwong/proxy/blob/master/README.md#pull-request)  <br/> 

--- 

Checkout the [proxy](https://github.com/stevealbertwong/proxy/blob/v3_http_request_response_proxy/proxy.cpp) program for a complete logic of proxy.

Accompanying blogpost: https://stevealbertwong.github.io/2017/01/20/multiprocess-proxy/

# Build instructions
This project has been developed using mac as client and linux ubuntu as server. The server should work on linux server.
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
# Versions
Currently, there 3 versions. v1 implements a vanilia proxy server with no caching or multiprocess or blacklist/ v2 has blacklist and multi-process, v3 has caching and http request, response, header parsing.



# Set up testing
To Set up testing with home linux server acting as proxy:
setting firefox browser to proxy to your own dynamic external ip at the time assigned by your router to your linux machine. then your code on linux machine proxy request to google server and proxy back to your client machine
(code only compile on linux machine)

1. port forwarding

* 192.168.0.1 to login to router to check port forwarding setting

* port forwarding if set up your own computer as web server at home. check inet addr desingated by home router ifconfig e.g. 192.168.0.6. then set your router to open http port 80(http://www.instructables.com/id/Set-up-your-very-own-Web-server/)

* google external ip to check your own home server's external ip address when setting up port forwarding

![alt text](https://raw.githubusercontent.com/stevealbertwong/stevealbertwong.github.io/master/assets/miscellaneous/router_port_forwarding.png)

2. firefox proxy setting

* download Firefox on client for any os(http://www.mozilla.org/en-US/firefox/new/)
 
* Once you download and launch Firefox, you can configure it (on Macs) to connect to the Internet through *proxy* by launching *Preferences* from the Apple menu, selecting *Advanced*, selecting *Network* within *A**dvanced*, selecting *Connection* within *Network*, and then activating a manual proxy (On Windows, proxy settings can be configured by selecting *Tools → **Options*).

![alt text](https://raw.githubusercontent.com/stevealbertwong/stevealbertwong.github.io/master/assets/miscellaneous/configure_firefox_proxy.png)

* sue firefox to go to www.stanford.edu/about/

(some websites like google did not work at the moment cause request parsing library failed. I am working on this, pull request welcome!)

# Pull request welcome!
* current version of http request, response only deals with get request and there are some efforts needed on different types of request and response parsing. It could then support many other websites.
* i am working on incorporating threadpool (checkout my blogpost)
* other features are very welcome as well !!
