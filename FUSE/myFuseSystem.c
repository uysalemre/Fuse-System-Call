/*
------------------ INFORMATION -----------------------
** This is a Website Read Only File System for users.
** Simply it does not allows write operations in it.
** Main purpose is giving tidied HTML files to users.
** In read operation code creates tidied output of HTML.

------------------ REQUIREMENTS ----------------------
** First we need to install libtidy 
** git clone git@github.com:htacg/tidy-html5.git
** cd {your-tidy-html5-directory}/build/cmake
** cmake ../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
** make
** sudo make install

--------------------- USAGE---------------------------
** Create a directory ( for example  Website ) 
** In Website directory create html directory 
** The other directories in website does not make sense.
** Move your .html files into html directory
** Go to out of your website.  
** Create a directory named test
** Create a configuration file config.txt to make configuration
** For a true configuration change the config_file_path to your code directory
** Then compile using : gcc -I/usr/include/tidy myFuseSystem.c -o myFuse -Wall -ansi -W -std=c99 -g -ggdb -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -lfuse -ltidy
** Execute : ./myFuse -d website test
** Open another terminal and go to test/html directory
** Read the Html files using cat try.html 
** Output is tidied.

*/


#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <dirent.h>
#include <unistd.h>
#include <fuse.h>
// tidy header files 
#include <tidy.h> 
#include <tidybuffio.h>

char *rw_path;
char *config_file_path = "/home/mac/Desktop/config.txt";
int global_file_size = 8192;    // for first initialization for reading file bytes

// change the path for ex website/html/ to  website/html
static char* translate_path(const char* path)
{

    char *rPath= malloc(sizeof(char)*(strlen(path)+strlen(rw_path)+1));

    strcpy(rPath,rw_path);
    if (rPath[strlen(rPath)-1]=='/') {
        rPath[strlen(rPath)-1]='\0';
    }
    strcat(rPath,path);
    return rPath;
}

// get attribute function 
static int myFuse_getattr(const char *path, struct stat *st_data)
{
    int res;
    char *upath=translate_path(path);
    // make initialization for stat structure
	memset(st_data, 0, sizeof(struct stat));
    // if path include .html string then it needs to be tidied 
    // else it no tidy opearation
	if(strstr(upath,".html")==NULL)
	{
		res = lstat(upath, st_data);
	}
	else
	{
		st_data->st_mode = S_IFREG | 0444;  // it shows it is a regular file and just read is allowed
        st_data->st_nlink = 1;              // give one hardlink to point it
        st_data->st_size = global_file_size;// set the bytes for reading file	
	}
    free(upath);
    if(res == -1) 
    {
        return -errno;
    }
    return 0;
}

// readlink function 
static int myFuse_readlink(const char *path, char *buf, size_t size)
{
    int res;
    char *upath=translate_path(path);
    // use readlink function for place the contents of path in the buffer
    res = readlink(upath, buf, size - 1);
    free(upath);
    if(res == -1) 
    {
        return -errno;
    }
    buf[res] = '\0';
    return 0;
}

// this function reads the directory and gives the content of the directory to user using filler function
static int myFuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    int res;

    (void) offset;
    (void) fi;

    char *upath=translate_path(path);

    dp = opendir(upath);        // open the directory
    free(upath);
    if(dp == NULL) 
    {
        res = -errno;
        return res;
    }

    // give the user all things in the directory
    while((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}


// open function checks the operation type and opens the given path according to given flags
static int myFuse_open(const char *path, struct fuse_file_info *finfo)
{
    int res;
    int flags = finfo->flags;

    // check if it is read only or not
    if ((flags & O_WRONLY) || (flags & O_RDWR) || (flags & O_CREAT) || (flags & O_EXCL) || (flags & O_TRUNC) || (flags & O_APPEND)) {
        return -EROFS;
    }

    char *upath=translate_path(path);

    // open
    res = open(upath, flags);

    free(upath);
    if(res == -1)
    {
        return -errno;
    }
    close(res);
    return 0;
}

// read function reads the content of the file
static int myFuse_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *finfo)
{
    int fd;
    int res;
    (void)finfo;
	memset(buf,0,size);                 // clear the last content for reading properly 
    char *upath=translate_path(path);
    fd = open(upath, O_RDONLY);         
    free(upath);
    if(fd == -1) 
    {
        res = -errno;
        return res;
    }
  
    // read content into buffer from file descriptor
    res = pread(fd, buf, size, offset);
    
    // if path includes /html/ 
    if(strstr(path,".html")!=NULL)
    {
		
		TidyBuffer output = {0};
		TidyBuffer errbuf = {0};
		int rc = -1;
		
		/*
		Bool ok;
		int ind;
		int indentation ;
		*/
		
		TidyDoc tdoc = tidyCreate();                     // Initialize "document"
		//printf( "Tidying:\t%s\n", input );             // for seeing input in the debug screen
		
		int loader = tidyLoadConfig(tdoc,config_file_path);	// read configuration from file
		
		/*ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );  // Convert to XHTML
		ind = tidyOptSetInt(tdoc,TidyIndentContent,yes); // Make indentation possible
		indentation = tidyOptSetInt(tdoc,TidyIndentSpaces,4);   // set indent space to 4
		*/
		
		if ( loader == 0 )
			rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
		if ( rc >= 0 )
			rc = tidyParseString( tdoc, buf );           // Parse the input
		if ( rc >= 0 )
			rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
		if ( rc >= 0 )
			rc = tidyRunDiagnostics( tdoc );               // Kvetch
		if ( rc > 1 )                                    // If error, force output.
			rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
		if ( rc >= 0 )
			rc = tidySaveBuffer( tdoc, &output );          // Pretty Print

		if ( rc >= 0 )
		{
			if ( rc > 0 )
				printf( "\nDiagnostics:\n\n%s", errbuf.bp );            // see the errors in the debug screen
			//printf( "\nAnd here is the result:\n\n%s", output.bp );   // see the output in the debug screen
			
			tidySaveString(tdoc,buf,&output.size);                      // save tidy output to buffer to show user
			res = output.size;                                          // make res equals to output size for returning byte number
			global_file_size = res;                                     // make global file size number returned byte
			printf("RESULT IS: \n %s  %d\n",buf,res);                   // see the result in the buffer in debug screen
		}
		else
			printf( "A severe error (%d) occurred.\n", rc );            // if tidy operation is not happened

		tidyBufFree( &output );
		tidyBufFree( &errbuf );
		tidyRelease( tdoc );
	}

    if(res == -1)
    {
        res = -errno;
    }
    close(fd);
    return res;
}



// obtain information about file system from the given path
static int myFuse_statfs(const char *path, struct statvfs *st_buf)
{
    int res;
    char *upath=translate_path(path);

    res = statvfs(upath, st_buf);
    free(upath);
    if (res == -1) 
    {
        return -errno;
    }
    return 0;
}

// check the calling process can access file by using path
static int myFuse_access(const char *path, int mode)
{
    int res;
    char *upath=translate_path(path);

    if (mode & W_OK)
        return -EROFS;

    res = access(upath, mode);
    free(upath);
    if (res == -1) {
        return -errno;
    }
    return res;
}

// write operation is not allowed to show read only file system 
static int myFuse_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *finfo)
{
    (void)path;
    (void)buf;
    (void)size;
    (void)offset;
    (void)finfo;
    return -EROFS;
}


struct fuse_operations myFuse_oper = {
    .getattr     = myFuse_getattr,
    .readlink    = myFuse_readlink,
    .readdir     = myFuse_readdir,
    .open        = myFuse_open,
    .read        = myFuse_read,
    .statfs      = myFuse_statfs,
    .access      = myFuse_access,
    .write		 = myFuse_write
};

static int myFuse_parse_opt(void *data, const char *arg, int key,
                          struct fuse_args *outargs)
{
    (void) data;

    switch (key)
    {
    case FUSE_OPT_KEY_NONOPT:
        if (rw_path == 0)
        {
            rw_path = strdup(arg);
            return 0;
        }
        else
        {
            return 1;
        }
    case FUSE_OPT_KEY_OPT:
        return 1;
    default:
        fprintf(stderr, "This is an error for giving args !\n");
        exit(1);
    }
    return 1;
}


// start point of the fuse 
int main(int argc, char *argv[])
{
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    int res;

    res = fuse_opt_parse(&args, &rw_path, NULL, myFuse_parse_opt);
    if (res != 0)
    {
        fprintf(stderr, "Invalid arguments\n");
        exit(1);
    }
    if (rw_path == 0)
    {
        fprintf(stderr, "Missing readwritepath\n");
        exit(1);
    }
    fuse_main(args.argc, args.argv, &myFuse_oper, NULL);
    return 0;
}
