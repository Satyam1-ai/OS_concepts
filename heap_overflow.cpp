
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>




int process_tracker=0;


struct process_block{
    int pid;
    int frames_allocated;
};

struct page_table_entries{
    int page_id;
    int ram_frame_number;
    
};


int creating_new_process(struct process_block* process_table){
    if(process_tracker>10)
        return -1;
    struct process_block p;
    p.frames_allocated=-1;
    p.pid=process_tracker+1;
    process_table[process_tracker]=p;
    process_tracker++;


    return 1;
}


void initialize_PageTable(struct page_table_entries * page_table,int x){
    page_table->page_id=x;
    page_table->ram_frame_number=-1;
}


int finding_ram_frame(int size){
    return 5;//NOT DOING THE ..WHOLE BITMAP METHOD OF ..SEEING WHAT IS AVAILABLE AND WHAT IS NOT .
}


char* malloc_(struct process_block *p,struct page_table_entries* page_table,char* program_break,int size,char* limit){//BASIC MALLOC ..DID NOT HANDLE ANY FRAMENTATION OR CEHCKEING FOR PREVIOUS

    if(program_break+size>=limit){
        std::cout<<"CANT ADD MORE ....HEAP FULL\n";
        return nullptr;
    }
    //lets assume next available..page number is 2 ..
    initialize_PageTable(page_table,2);
    char* temp=program_break;
    program_break=program_break+size;

    page_table[2].page_id=2;
    int x=finding_ram_frame(size);
    page_table[2].ram_frame_number=5;
    p->frames_allocated++;
    return temp;
}

int checker(struct process_block *p){
    for(int i=0;i<4;i++){
        if(p[i].frames_allocated>10){
            std::cout<<"CANNOT ALLOCATE MORE ..ABORT ASSAAP WITH kill_function";
            return -1;
        }
    }
    return 1;
}


int main(){
    char buffer[5000];

    //process table ..from 0 to...200
    struct process_block* process_table=(struct process_block*)&buffer[0];
    
    creating_new_process(process_table);
    creating_new_process(process_table);
    creating_new_process(process_table);
    creating_new_process(process_table);

    //page tables from ...200 to 2000 with focus on process 2

    struct page_table_entries* page_table=(struct page_table_entries*)&buffer[400];

    //FOCUS ON THE HEAP MALLOC

    char* virtual_memory=&buffer[2000];//2000 to ..4000 ..for this process dummy example..stack willstart at ..4000 ..
    char* text_portion=virtual_memory;//ASSUME ALLOCATED IN A PAGE ..IN ANOTHER .SANDBOX ..HYPOTHETICALLY 
    char* program_break=text_portion+100;//sbrk(0) ..emulation
    int size=10;
    char* temp=malloc_(&process_table[2],page_table,program_break,size,&buffer[3000]);
    //CAN FINALLY WRITE .USING THE ..TEMP...We have forces ..allocation of the ..ram frame instead of just keeping it as -1

    while(true){
        int x=checker(process_table);
        if(x==1)
            char* temp=malloc_(&process_table[2],page_table,program_break,size,&buffer[3000]); //EMULATING THE OOMS ..WONT WORK ..BEBCASUE NO REAL RAM ALLOCATION AND EXPLICIT IF IN THE .FUNCTION
        else{
            std::cout<<"YOU'RE OUT MATE ";
            break;
        }
    }

}