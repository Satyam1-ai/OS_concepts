
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>



int main(){

    int pid=fork();

    if(pid==0){
        char *arr[]={(char*)"ls",(char*)"-l",nullptr};
        execve("/bin/ls",arr,nullptr);
        std::cout<<"ERROR";
        exit(0);
    }


    else{
        wait(nullptr);
        std::cout<<"CHILD PROCESS EXECUTED ..THE ...ERROR IS NOT ..PRINTED ..SO YEAH";

    }



}