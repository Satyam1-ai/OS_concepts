
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>



void recursive_search(const std::string &path){
    DIR *dir=opendir(path.c_str());
    if(dir==nullptr)
        return;

    struct dirent *entry;
    while((entry=readdir(dir))!=nullptr){
        if(std::string(entry->d_name)=="." || std::string(entry->d_name)==".."){
            continue;
        }
        std::string s=path+"/"+entry->d_name;
        std::cout<<"FOUND: "<<s<<"\n";
        if(entry->d_type==DT_DIR)
            recursive_search(s);
    }

    closedir(dir);
}


int main(){
    recursive_search(".");
    return 0;    
}