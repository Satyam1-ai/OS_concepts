
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>



struct my_dirent{
    char file_name[50];
    int d_ino;
};
struct mySuperBlock{
    int location_of_inode_table;
    int location_of_directory;
    int location_of_data;
    int gap_size;
};
struct my_inode{
    int file_size;
    char permissions[4];
    int link_count;
    int data_block_offset;
};

int main(){

    char buffer[1024]={0};
    mySuperBlock *s=(mySuperBlock*)(&buffer[0]);
    s->location_of_data=800;
    s->location_of_inode_table=500;
    s->location_of_directory=50;


    my_dirent *my_directory=(my_dirent*)&buffer[s->location_of_directory];
    my_inode *inode_entry=(my_inode*)&buffer[s->location_of_inode_table];
    char *data=&buffer[s->location_of_data];

    strncpy(data,"MY NAME IS SATYAM",50);


    inode_entry[0].file_size=20;
    inode_entry[0].link_count=3;
    strncpy(inode_entry[0].permissions,"rwx",sizeof(inode_entry[0].permissions)-1);
    inode_entry[0].data_block_offset=0;


    strncpy(my_directory[0].file_name,"first.txt",sizeof(my_directory[0].file_name));
    strncpy(my_directory[1].file_name,"second.txt",sizeof(my_directory[1].file_name));
    strncpy(my_directory[2].file_name,"third.txt",sizeof(my_directory[2].file_name));


    my_directory[0].d_ino=0;
    my_directory[1].d_ino=0;
    my_directory[2].d_ino=0;


    int directory_count=3;

    std::string arr[3]={"first.txt","second.txt","third.txt"};

    for(int i=0;i<3;i++){
        std::string s=arr[i];
        std::cout<<"WE ARE IN THE ....FILE :"<<s<<"\n";
        int index=-1;
        int extracted_information=-1;
        for(int j=0;j<directory_count;j++){
            if(std::strcmp(s.c_str(),my_directory[j].file_name)==0){
                index=j;
                extracted_information=my_directory[index].d_ino;
                std::cout<<"Hello";
                break;
            }
        }
        if(index==-1){
            continue;
        }

        my_directory[i]=my_directory[i+1];
        directory_count--;

        my_inode* target_inode=&inode_entry[extracted_information];
        target_inode->link_count--;

        if(target_inode->link_count==0){
            int data_offset=target_inode->data_block_offset;
            memset(&data[data_offset],0,target_inode->file_size);
            target_inode->file_size=0;
            target_inode->data_block_offset=-1;
            strncpy(target_inode->permissions,"000",4);
        }
    }
    return 0;
}