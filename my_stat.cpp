#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <cstring>
#include <dirent.h>



struct MySuperBlock{
    int location_of_inode_table;
    int gap;
};


struct MyInode{
    int file_size;
    char permissions[4];
};



int main(){
    char mock_address[1000]={0};
    MySuperBlock *s=(MySuperBlock*)&mock_address[0];


    s->gap=8;
    s->location_of_inode_table=500;
    MyInode *A=(MyInode*)&mock_address[s->location_of_inode_table];

    A[0].file_size=10;
    std::strncpy(A[0].permissions,"rwx",4);
    A[1].file_size=20;
    std::strncpy(A[1].permissions,"rw0",4);
    A[2].file_size=30;
    std:strncpy(A[2].permissions,"rwx",4);
    DIR *dir=opendir(".");

    struct dirent *entry;
    while((entry=readdir(dir))!=nullptr){
        if(std::string(entry->d_name)== "." || std::string(entry->d_name)==".."){
            continue;
        }
        int offset=(entry->d_ino)%3;
        int location=s->location_of_inode_table+(offset*s->gap);
        MyInode *A=(MyInode*)&mock_address[location];

        std::cout<<entry->d_name<<"\n";
        std::cout<<A->file_size<<"\n";
        std::cout<<A->permissions<<"\n";
        
    }

}