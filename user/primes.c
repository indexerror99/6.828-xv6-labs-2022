#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    int p[2];
    pipe(p);

    // some shares the write() which the 2nd parameter in &x sytle
    // but in xv6 book it's given in no & style
    // write all candidates one time
    for(char i = 2;i < 36;i++){
        write(p[1], &i, 1);
        // printf("writing %d\n", i);
    }
    close(p[1]);

    // seems for(;;) is better than while cuz it is more simple after compile
    for(;;) {
        int p_read = p[0];
        char num, first;
        pipe(p);

        if(fork() == 0) {
            // there is data in pipe
            if(read(p_read, &first, 1) == 1) {
                printf("prime %d\n", first);
            }
            // no data
            else {
                exit(0);
            }
            while(read(p_read, &num, 1) > 0) {
                // not a factor
                
                // printf("keep reading!\n");
                if(num % first != 0) {
                    write(p[1], &num, 1);
                }
            }
            // close pipe
            close(p[1]);
            close(p_read);
        }
        else {
            close(p_read);
            close(p[0]);
            close(p[1]);
            wait(0);
            exit(0);
        }
    }
    exit(0);
}