

#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <string>

#include <stdio.h>
#include <dirent.h>

int main(){
    DIR *dir=opendir(".");
    if(dir==NULL){
        std::cout<<"Could not open directory"<<"\n";
        return 1;
    }

    struct dirent *entry;

    while((entry=readdir(dir))!=NULL){
        std::cout<<"Found : "<<entry->d_name<<"\n";
    }

    closedir(dir);
    return 0;



}