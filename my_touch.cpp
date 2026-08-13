#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>
#include <ctime>
#include <stdio.h>
#include <dirent.h>



struct my_inode{
    int file_size;
    int permission;
    int data_block_offset;
    long access_time;
    long modification_time;
};


struct my_dirent{
    char file_name[20];
    int d_ino;

};





int main(){
    char buffer[2048]={0};

    my_dirent *directory=(my_dirent*)(buffer);
    my_inode *inode_entry=(my_inode*)(buffer+300);

    std::strncpy(directory[0].file_name,"first.txt",20);
    directory[0].d_ino=0;
    inode_entry[0].file_size=10;
    inode_entry[0].permission=755;
    inode_entry[0].data_block_offset=0;
    inode_entry[0].access_time=0;
    inode_entry[0].modification_time=0;

    char target_file[20];
    strncpy(target_file,"first.txt",20);
    int target_ino=-1;
    int directory_count=1;
    for(int i=0;i<directory_count;i++){
        if(std::strcmp(directory[i].file_name,target_file)==0){
            target_ino=i;
            break;
        }
    }

    if(target_ino==-1){
        std::cout<<"The file does not exist ....So we have to make a new one "<<"\n";

        return 1; 
    }
    long current_system_time = std::time(nullptr);
    my_inode *target_inode=(my_inode*)&inode_entry[target_ino];

    target_inode->access_time = current_system_time;
    target_inode->modification_time = current_system_time;

    return 0;

}