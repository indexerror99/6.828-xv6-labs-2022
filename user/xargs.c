#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void
xargs(int argnum, char* args[])
{
    int i = 0;
    int index = argnum;
    char buf[512];
    char c = 0;

    while(read(0, &c, 1) > 0){
        if((c == '\n' || c == ' ') && strlen(buf) > 0){
            args[index] = malloc(i + 1);
            memcpy(args[index], buf, strlen(buf));
            args[index++][i] = '\0';
            memset(buf, 0, sizeof(buf));
            i = 0;
        }
        else{
            buf[i++] = c;
        }
    }
}

int
main(int argc, char* argv[])
{
    if(argc < 2){
        fprintf(2, "Usage: <command> | xargs <command> <param>\n");
    }

    if(argc > MAXARG + 1){
        fprintf(2, "too many args\n");
    }
    char* args[MAXARG];
    
    for(int i = 1;i < argc;i++){
        args[i - 1] = malloc(sizeof(argv[i]));
        memcpy(args[i - 1], argv[i], sizeof(argv[i]));
    }

    xargs(argc - 1, args);
    exec(args[0], args);

    exit(0);
}