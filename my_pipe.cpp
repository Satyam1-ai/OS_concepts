
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>
#include <sys/wait.h>
#include <stdio.h>
#include <dirent.h>








int main(){
    int pipefd[2];

    if(pipe(pipefd)==-1){
        return 0;
    }
    pid_t pid=fork();
    if(pid==0){
        close(pipefd[0]);
        char *s=(char *)"hello";
        write(pipefd[1],s,6);
        close(pipefd[1]);
        exit(0);
    }
    pid_t ppid=fork();
    if(ppid==0){
        close(pipefd[1]);
        char s[100];
        read(pipefd[0],s,10);
        close(pipefd[0]);
        std::cout<<"The thing that was writeen  onto the ...pipe was : "<<s;
        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(nullptr);
    wait(nullptr);



}