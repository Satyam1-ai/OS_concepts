#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>
#include <stdio.h>
#include <dirent.h>


#define PAGE_SIZE 100
char* RSI;
int RDI;
char* RXI;

struct paging_entry{
    int page_index;
    bool is_present;
    int ram_frame_id;
    int hd_offset;
};



void start_(char *stack_segment,char *heap,char *text_data){
    int argc=2;
    const char *A="ls -l";

    stack_segment[0]=argc;
    memcpy(&stack_segment[4],&A[0],strlen(A)+1);
    const char *env="USER=SATYAM PATH=/bin/actual_path";
    memcpy(&stack_segment[20],&env[0],strlen(env)+1);


    RDI=stack_segment[0];
    RSI=&stack_segment[4];
    RXI=&stack_segment[20];

    std::cout<<RDI<<std::endl<<(char*) RSI<<std::endl<<(char*)RXI;

}

std::vector<bool> ram_allocation(char* RAM){
    int number_of_frames=6000/100;
    std::vector<bool> A(60,false);
    return A;
}

int is_available(std::vector<bool> &A){
    for(int i=0;i<A.size();i++){
        if(A[i]==false) 
            return i;
    }
    return -1;
}

void simulation(struct paging_entry* pages,char *hardrive,char *RAM){
    //FOR THE start_ funciton ..ONLY .
    std::vector<bool> A=ram_allocation(RAM);
    pages[2].is_present=true;
    pages[2].ram_frame_id=is_available(A);
    //ACTUAL ADDRESS WOULD BE ..RAM+(Pages[2].ram_frame_id*100) 

    memcpy(RAM+(pages[2].ram_frame_id*100),hardrive,100);


}


int main(){
    char buffer[10000];

    char virtual_memory[2000];
    char *stack_segment=&virtual_memory[0];
    char *heap=&virtual_memory[100];
    char *text_data=&virtual_memory[500];

    start_(stack_segment,heap,text_data);
    char hard_drive[1000];
    char* RAM=&buffer[4000];
    const char *full_file=R"(void start_(char *stack_segment,char *heap,char *text_data){
    int argc=2;
    const char *A="ls -l";

    stack_segment[0]=argc;
    memcpy(&stack_segment[4],&A[0],strlen(A)+1);
    const char *env="USER=SATYAM PATH=/bin/actual_path";
    memcpy(&stack_segment[20],&env[0],strlen(env)+1);


    RDI=stack_segment[0];
    RSI=&stack_segment[4];
    RXI=&stack_segment[20];

    std::cout<<RDI<<std::endl<<(char*) RSI<<std::endl<<(char*)RXI;

})";

   memcpy(&hard_drive[0],full_file,strlen(full_file)+1);
    struct paging_entry* pages=(struct paging_entry*)&buffer;
    //STACK
    pages[0].hd_offset=-1;
    pages[0].is_present=false;
    pages[0].ram_frame_id=-1;
    pages[0].page_index=1;
    //HEAP
    pages[1].hd_offset=-1;
    pages[1].is_present=false;
    pages[1].ram_frame_id=-1;
    pages[1].page_index=2;
    //TEXT
    pages[2].hd_offset=-1;
    pages[2].is_present=false;
    pages[2].ram_frame_id=-1;
    pages[2].page_index=3;
    //TEXT2
    pages[3].hd_offset=-1;
    pages[3].is_present=false;
    pages[3].ram_frame_id=-1;
    pages[3].page_index=4;

}