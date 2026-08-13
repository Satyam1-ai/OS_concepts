
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
    int pid=fork();
    if(pid==0){
        //child process
        std::cout<<"The child process's pid is : "<<getpid()<<"\n";
        std::cout<<"My parent's proccess ID is : "<<getppid()<<"\n";
        exit(0);
    }
    else{
        //parent process
        wait(NULL);
        std::cout<<"The parent process's pid is :"<<getpid();
    }


}