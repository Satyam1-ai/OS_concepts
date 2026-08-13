

#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>
#include <ctime>
#include <stdio.h>
#include <dirent.h>




struct my_super_block{
    char*  inode_table;
    char*  data_block;

};

struct MyInode{
    int file_size;
    char permissions[4];
    int block_pointers[4]; 
};





bool allocation(char *data_bitmap,MyInode *inode){
    int size=inode->file_size;
    int blocks_needed=(size+63)/64;
    if(blocks_needed>4)
        return false;
    
    int blocks_found=0;
    for(int bit_index=0;bit_index<24;bit_index++){
        int byte_num=bit_index/8;
        int bit_num=bit_index%8;

        char mask=(1<<bit_num);
        if((data_bitmap[byte_num] & mask) == 0){
            inode->block_pointers[blocks_found]=bit_index;
            blocks_found++;

            if(blocks_found==blocks_needed)
                break;
        }
    }

    if(blocks_found<blocks_needed)
        return false;
    for(int i=0;i<blocks_needed;i++){
        int target_block=inode->block_pointers[i];
        int byte_num=target_block/8;
        int bit_num=target_block%8;
        char mask=(1<<bit_num);
        data_bitmap[byte_num]|= mask;
    }

    return true;


}



int main(){

    char data_bitmap[3];
    data_bitmap[0]=0;
    data_bitmap[1]=0;
    data_bitmap[2]=0;




    char sandbox[2048];
    my_super_block *s=(my_super_block*)sandbox;
    s->inode_table=sandbox+100;
    s->data_block=sandbox+500;
    
    MyInode *inode_location=(MyInode*)s->inode_table;
    
    char *data_location=(char *)s->data_block;

    
    inode_location[0].file_size=100;
    std::strcpy(inode_location[0].permissions,"0644");
    allocation(data_bitmap,&inode_location[0]);
    std::string text0 = "FILE 0!";
    std::copy(text0.begin(), text0.end(), data_location + (inode_location[0].block_pointers[0] * 64));


    inode_location[1].file_size=200;
    std::strcpy(inode_location[1].permissions,"0666");
    allocation(data_bitmap,&inode_location[1]);
    std::string text1 = "FILE 1!";
    std::copy(text1.begin(), text1.end(), data_location + (inode_location[1].block_pointers[0] * 64));

    for(int i=0;i<7;i++){
        int offset=inode_location[1].block_pointers[0];
        offset=offset*64;
        char *ptr=reinterpret_cast<char *>(data_location+offset+i);
        char val=*ptr;
        std::cout<<val;
    }
    std::cout<<"\n";

}