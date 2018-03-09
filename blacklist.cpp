#include <fstream> // ifstream, getline()
#include <iostream>

#include <regex>
#include <string>

#include "blacklist.h"

using namespace std;

/*
1. when init(), read blacklist-websites.txt into std::vector<std::regex> blacklist_websites
2. public method to check std::vector<std::regex> blacklist_websites
*/
HTTPBlacklist::HTTPBlacklist(const char* file_name){
    ifstream infile(file_name);

    while(true){
        string line;
        getline(infile, line);
        regex re(line);
        blacklist_websites.push_back(re);
    }    
}

bool HTTPBlacklist::is_blacklisted(const char* website){
    for (const regex& re: blacklist_websites){
        if (regex_match(website,re)){
            return true;
        }
    }
    return false;
}