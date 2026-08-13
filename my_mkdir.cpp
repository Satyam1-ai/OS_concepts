#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>




struct dirent{
    char file_name[20];
    int d_ino;
};


struct inode_table{
    char permissions[4];
    int data_block_offset;
    int file_size;
};


struct mySuperBlock{
    int location_of_directory;
    int location_of_data_block;
    int location_of_inode_table;

};


int main(){
    mySuperBlock s=(mySuperBlock*)();
    



}