#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv){
    int p0[2], p1[2];
    char* byte = "a";
    pipe(p0);
    pipe(p1);
    
    int id = fork();
    if(id == 0) {
        close(p1[0]);
        write(p1[1], byte,1);
        if(read(p0[0], byte, 1) == 1)
            printf("%d: received ping\n", getpid());
        else
            fprintf(2, "son read error\n");
        exit(0);
    } else {
        close(p0[0]);
        write(p0[1], byte, 1);
        wait(0);
        if(read(p1[0], byte, 1) == 1)
            printf("%d: received pong\n", getpid());
        else
            fprintf(2, "parent read error\n");
    }
    exit(0);
}