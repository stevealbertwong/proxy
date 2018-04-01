#ifndef _CACHE_H_
#define _CACHE_H_

#include <sys/stat.h> // lstat()
#include <dirent.h> // opendir(), readdir_r()
#include <string>
#include <stdio.h> // getline()
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class HttpCache{
public:
	HttpCache();
	// 1st const reference parameter: ensure orignal parameter passed wont get amended, if pass by value(copy) changed should not matter
	// 2nd const function: function wont change any class variable but ok local variable of function + cannot call non-const function even if such non-const function does not change class variable
	void createCacheDirectory(const string& directory, bool empty = false) const; 
	bool ensureEntryExists(const string& request) const; 

	void saveCache(const string& request, const string& response) const;
	string retrieveCache(const string& request) const;
	string hashRequest(const string& request) const;
		
private:
	string cwd; // current working dir
	string cd; // cache directory

};

#endif