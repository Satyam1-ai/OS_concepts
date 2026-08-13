#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>
#include <stdio.h>
#include <dirent.h>
//0->keyboard
//1->monitor 
//2->error

//fd->file.txt


//What is happening ..here is that ...dup2(oldfd,newfd) ......
//The ..newfd...which point to something ..will change its ..pointer to point to ..what the old points to ...
int main(){
    int fd=open("file.txt",O_RDWR);
    dup2(fd,1);

    std::cout<<"hello..i AM A GUY";



}