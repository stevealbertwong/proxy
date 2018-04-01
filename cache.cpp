#include "cache.h"

static const string cacheDirectory = "cachedir";
HttpCache::HttpCache(){
	// ensureDirectoryExists();
	char buffer[1000];
	char *answer = getcwd(buffer, sizeof(buffer));	
	cwd = string(answer);
	cd = cwd + "/" + cacheDirectory;	
	createCacheDirectory(cd);
}

//https://techoverflow.net/2013/04/05/how-to-use-mkdir-from-sysstat-h/
static const int kDefaultPermissions = 0777; // rwxrwxrwx 
void HttpCache::createCacheDirectory(const string& directory, bool empty) const {  
  // if dir not exist, creates new one
  struct stat st;  
  if (lstat(directory.c_str(), &st) != 0){
  	mkdir(directory.c_str(), kDefaultPermissions);
  }
    
  // if empty flag, delete cache dir files
  if (!empty) return;  
  DIR *dir = opendir(directory.c_str());
  int return_code;
  struct dirent entry;
  struct dirent *result;    
  for (return_code = readdir_r(dir, &entry, &result); 
        result != NULL && return_code == 0; 
        return_code = readdir_r(dir, &entry, &result)) {
    
    string dirEntry = entry.d_name;
    if (dirEntry != "." && dirEntry != ".."){
    	// remove(dirEntry.c_str());
    	cout << "delete cache" << endl;
    }          	
  }
  closedir(dir);
}


/*
check hash directory inside cache directory + not empty + is dir
https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
*/
bool HttpCache::ensureEntryExists(const string& request) const {
  
  string requestHash = hashRequest(request);
  // string path = cd + "/" + requestHash;    
  string path = cd;
  struct stat st;
  
  // check exist => lstat is improved version of stat that does symbolic links
  if (lstat(path.c_str(), &st) != 0) return false;
  
  // is directory
  if (!S_ISDIR(st.st_mode)) return false; 
  
  // check emptiness
  int return_code;  
  struct dirent entry;
  struct dirent *result;    
  DIR *dir = opendir(path.c_str());
  bool exists = false;  
  for (return_code = readdir_r(dir, &entry, &result); 
        result != NULL && return_code == 0; 
        return_code = readdir_r(dir, &entry, &result)) {
    string dirEntry = entry.d_name;
	
    // exists = dirEntry != "." && dirEntry != "..";
    exists = requestHash.compare(dirEntry) == 0;
  }
  closedir(dir);
  
  return exists;
}

// TODO full directory path + hash 
void HttpCache::saveCache(const string& request, const string& response) const {
	// ios::out => opens file for writing
	// ios::binary => data is read or written without translating new line characters to and from \r\n on the fly
	// | => both => writing without translating

	string requestHash = hashRequest(request);
	string cacheEntryPath = cd + "/" + requestHash;
	ofstream outfile(cacheEntryPath.c_str(), ios::out | ios::binary);
	outfile << response << flush;
}

// given request + hashed to get full directory path + return response
string HttpCache::retrieveCache(const string& request) const {

	string requestHash = hashRequest(request);
	string cacheEntryPath = cd + "/" + requestHash;
	
	ifstream instream(cacheEntryPath.c_str(), ios::in | ios::binary);	
	string response;
	// https://stackoverflow.com/questions/116038/what-is-the-best-way-to-read-an-entire-file-into-a-stdstring-in-c
	// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring	
	// http://cpp.indi.frih.net/blog/2014/09/how-to-read-an-entire-file-into-memory-in-cpp/
	response = static_cast<stringstream const&>(stringstream() << instream.rdbuf()).str();
						
	// if(instream.eof()) break;  	
  return response;
}

string HttpCache::hashRequest(const string& request) const {
  // std::size_t str_hash = std::hash<std::string>{}(request);
  hash<string> hasher;
  ostringstream oss;
  size_t str_hash = hasher(request); 
  oss << str_hash;
  return oss.str();
}



