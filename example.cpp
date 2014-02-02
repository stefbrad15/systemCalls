/*
 * Directory example for EECS 2550.
 * Author:  Jerry Heuring
 * Date:  August 23, 2007
 *
 * This is an example that demonstrates how you could
 * use system calls to go through directories on Unix.
 * Technically, I believe that it is POSIX compliant and
 * may work with Microsoft Windows but have not tested
 * it.  
 *
 * Note that the system calls are set up for C.  You may
 * wish to wrap them up as C++ classes to neaten things 
 * up.  
 *
 * Bugs:
 *
 * Revisions:
 *    Touched the file to force a remake 1/15/2009
 */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <time.h>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

// Prototypes
void listDirectory(DIR *directory, char *dirName);
string testMode (mode_t mode, mode_t flag, string description);
void decodeMode(mode_t mode);

/*
 * The main program processes it's arguments assuming each one is
 * a directory and then lists the contents of that directory. 
 * If no arguments are given it lists the contents of the current
 * directory (.).  
 */
int main (int argCount, char * argValues[]) {
    DIR    *directory;
    int     argNbr;
    char   *argument;

    if (argCount == 1) {
	cout << "No directory given -- using current directory" << endl;
	argument = (char*)(".");
	directory = opendir (".");
	if (directory != NULL) {
	    listDirectory (directory, argument);
	} else {
	    cout << "Failed to open directory." << endl;
	}
    } else {
	for (argNbr = 1; argNbr < argCount; argNbr++) {
	    argument = argValues[argNbr];
	    cout << "Processing Directory " << argument << endl;
	    directory = opendir(argument);
	    if (directory != NULL) {
		listDirectory (directory, argument);
	    } else {
		cout << "Failed to open directory." << endl;
	    }
	}
    }

    return 0;
}
/*
 * This method steps through all the entries in a directory
 * and outputs information on each entry.  It isn't in a 
 * convenient format but it does work and give pretty good
 * information.
 */
void listDirectory(DIR *directory, char *dirName) {
    struct dirent *entry;
    string filename;
    struct stat fileInfo;

    while ((entry = readdir(directory)) != NULL){
	filename = dirName;
	filename = filename + "/";
	filename = filename + entry->d_name;
	lstat(filename.c_str(), &fileInfo);
	cout << "Name    : " << filename << endl 
	     << "I-node  : " << fileInfo.st_ino << endl
	     << "Mode    : ";
	decodeMode(fileInfo.st_mode);
	cout << "User ID : " << fileInfo.st_uid << endl
	     << "Group ID: " << fileInfo.st_gid << endl
	     << "Size    : " << fileInfo.st_size << " bytes" << endl
	     << "Last Access Time : " << ctime(&fileInfo.st_atime)
	     << "Last Modify Time : " << ctime(&fileInfo.st_mtime)
	     << "Creation Time    : " << ctime(&fileInfo.st_ctime);
	
    }
    return;
}

/*
 * This is a utility method that allows me to write 
 * a cleaner decodeMode method.  If I was really smart
 * I'd probably encode the modes and the descriptions
 * into an array of structures and step through them.
 */
string testMode (mode_t mode, mode_t flag, string description) {
    if ((mode & S_IFMT) == flag)
	return description;
    else
	return "";
}

/*
 * decode the mode bits for the file to tell which
 * type of file it is.  The information for this was
 * found on a variety of man pages (stat, mknod)
 */
void decodeMode(mode_t mode) {
    cout << testMode(mode, S_IFREG, "Regular File\n");
    cout << testMode(mode, S_IFDIR, "Directory\n");
    cout << testMode(mode, S_IFCHR, "Character-special device file\n");
    cout << testMode(mode, S_IFBLK, "Block-special device file\n");
    cout << testMode(mode, S_IFLNK, "Symbolic Link\n");
    cout << testMode(mode, S_IFIFO, "FIFO file\n");
    cout << testMode(mode, S_IFSOCK, "Unix Domain Socket\n");
    return;
}
