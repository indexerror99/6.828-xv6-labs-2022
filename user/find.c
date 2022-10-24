#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// first mount the primary directory
// search every diectory till find the matching file
void
find(char *path, char* filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // first cd the destined directory
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    // then stat
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(st.type != T_DIR){
        fprintf(2, "find: not a directory %s\n", path);
        close(fd);
        return;
    }

    // judge if path fits in buffer
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        return;
    }

    // put path in buffer
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    // search the directory
    // if is diretory, go deep recursively
    // else compare if the filename is same
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if(strcmp(de.name, filename) == 0){
            printf("%s\n", buf);
            continue;
        }
        else if(st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
            find(buf, filename);
        }
    }
    close(fd);
}

int
main(int argc, char* argv[])
{
    // according to description
    // it's find <path> <filename> in directory tree
    if(argc != 3){
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(0);
    }
    char* path = argv[1];
    char* filename = argv[2];
    find(path, filename);
    exit(0);
}