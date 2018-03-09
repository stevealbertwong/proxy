#include <sys/stat.h> // lstat()
#include <dirent.h> // opendir(), 
#include <sys/stat.h> // mkdir()
#include <stdlib.h> // getenv()
#include "cache.h"
using namespace std;


static const string kCacheSubdirectory = ".http-proxy-cache";

HTTPCache::HTTPCache() {    
  string homeDirectoryEnv = getenv("HOME"); // return string of environment variable e.g. "PATH"
  cacheDirectory = homeDirectoryEnv + "/" + kCacheSubdirectory;
  ensureDirectoryExists(cacheDirectory);
}

// create dir to save cache 
static const int kDefaultPermissions = 644;
void HTTPCache::ensureDirectoryExists(const string& directory, bool empty) const {
  struct stat st;
  // if file exist, return 0, else 1
  if (lstat(directory.c_str(), &st) != 0)
    mkdir(directory.c_str(), kDefaultPermissions);
  
  if (!empty) return;

  DIR *dir = opendir(directory.c_str());

  /* struct dirent
  file system independent directory entry/ format of directory entries
  Different file system types may have different directory entries 
  dirent structure defines a file system independent directory entry, 
  which contains information common to directory entries in different file system types
  */
  int return_code;
  struct dirent entry;
  struct dirent *result; 
  for (return_code = readdir_r(dir, &entry, &result); 
        result != NULL && return_code == 0; 
        return_code = readdir_r(dir, &entry, &result)) {
    string dirEntry = entry.d_name;
    if (dirEntry != "." && dirEntry != "..") 
      remove(dirEntry.c_str());
  }
  
  closedir(dir);
}