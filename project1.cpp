#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

//Prototypes
void searchDirectory(string dirPath, char *searchString);
void searchFile(string fileName, string toSearch);
string GWP();

int main (int argCount, char * argValues[]) {
	DIR    *directory;
    int     argNbr;
    char   *argument;
    string cwd;

    cwd = GWP();

    if (argCount == 1) {
		cout << "No search string given" << endl;
    } else {
		for (argNbr = 1; argNbr < argCount; argNbr++) {
	    	argument = argValues[argNbr];
	    	cout << "Searching for: " << argument << endl;
	    	directory = opendir (cwd.c_str());
	    	if (directory != NULL) {
			  searchDirectory (cwd, argument);
	   	 	} else {
			cout << "Failed to open directory." << endl;
	   		}
		}
	}
    return 0;
}

void searchDirectory(string dirPath, char *searchString){
    const char* fileString;
    struct dirent *entry;
    struct stat fileInfo;
    bool   parent;
    mode_t dirFlag;
    mode_t regFile;
    int    processID, status, lstatI;

    string   filename, path;
    DIR      *dirToSearch;

    parent    = true;
    dirFlag   = S_IFDIR;
    regFile   = S_IFREG;
    processID = 0;

    dirToSearch = opendir (dirPath.c_str());
    while ((entry = readdir(dirToSearch)) != NULL && parent )
	{
	filename = entry->d_name;
	path 	 = dirPath + "/" + filename;
	fileString = filename.c_str();	
	
	lstatI = lstat(path.c_str(), &fileInfo); //get stats on current scanned file

	if (lstatI < 0) perror("lstat() error");

	//checks if file is a directory and forks process if so
	//if file is not a directory checks if filename contains search string 
	if ((fileInfo.st_mode & S_IFMT) == dirFlag)
		{
		//fork operation, recursive call?
		if (strcmp(fileString,".") != 0 && strcmp(fileString,"..") != 0 )			
			{
			processID = fork();
			if (processID == 0)
				{
				parent  = false;								
				cout << "Searching subdirectory: "<< path << endl;
				searchDirectory(path, searchString);
				}
			else
				{
				waitpid(processID, &status, 0);
				}
			}
		}
	else if((fileInfo.st_mode & S_IFMT) == regFile)
		{
			searchFile(path, searchString);	
		}
	else
		{
			cout << "Skipping search for non-regular file: " << filename << endl;
		}	
    }
    //if (foundIn == 0) cout<<"Search string not found"<<endl;
    
    return;
}

string GWP(){
	char temp[1024];
	if ( getcwd(temp, sizeof(temp))!= 0)
		return string ( temp );
	else return "error";
}

void searchFile(string fileName, string toSearch){
	ifstream inputFile;
	bool notFound;
	string line;

	notFound = true;

	inputFile.open(fileName.c_str());
	//checks if file is readable and opens it
	if(inputFile.good())
		{
			while (getline( inputFile, line) && notFound)
			{
			if (line.find(toSearch) != string::npos)
				{ 
				cout<<"Search string found in file: "<<fileName<<endl;
				notFound = false; 
				}
			}
			inputFile.close();
		}
}

