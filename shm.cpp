
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <sstream>
#include <cstdlib>


#define PAGE_SIZE 100


int shared_counter=0;

struct process{
    int pid;
    struct page_table_allocation *page_address;
};

struct page_entry{
    int page_id;
    int ram_frame_number=-1;
};

struct page_table_allocation{
    struct page_entry B[5];
    bool A[5]={false};
};


struct shared_memory_register{
    int key_id;
    int ram_frame_number;
    bool is_active;

};


int find_ram_frame(char *ram_index){
    for(int i=0;i<10;i++){
        if(ram_index[i]==false){
            ram_index[i]=true;
            return i;
        }
    }
    return -1;
}

void initialise_ram_frame(char *ram){
    bool A[10]={false};
    memcpy(ram,A,10);
    return;
}

int create_process(int pid,struct process* process_table,int size,struct page_table_allocation* base,char *buffer){
    if(pid==size){
        return -1;
    }
    int index=pid-1;
    process_table[index].pid=pid;
    process_table[index].page_address = &base[index]; 
    struct page_table_allocation* p = process_table[index].page_address;
    int x=checking_page_availability(p);
    if(x!=-1){
        p->B[x].page_id=x+1;
        p->B[x].ram_frame_number=find_ram_frame(&buffer[3000]);
    }
    return 0;
}
int checking_page_availability(struct page_table_allocation *p){
    for(int i=0;i<5;i++){
        if(p->A[i]==false){
            p->A[i]=true;
            return i;
        }
    }
    return -1;
}

int generate_key_id(){
    return 1234;
}


int check_match_in_shared_space(struct shared_memory_register *s,int id){
    for(int i=0;i<shared_counter;i++){
        if(s[i].is_active &&  s[i].key_id==id){
            return i;
        }
    }
    return -1;

}



void add_page(int pid,struct process* process_table,struct shared_memory_register *s,int id,char *buffer){
    struct process* temp=nullptr;
    for(int i=0;i<10;i++){
        if(process_table[i].pid==pid){
            temp=&process_table[i];
            break;
        }
    }

    int x=checking_page_availability(temp->page_address);
    if(x!=-1)
    {
        temp->page_address->B[x].page_id=x+1;
        int crazy=check_match_in_shared_space(s,id);
        if(crazy==-1){
            int allocated_frame = find_ram_frame(&buffer[3000]); 
            temp->page_address->B[x].ram_frame_number=allocated_frame;
            s[shared_counter].key_id=generate_key_id();
            s[shared_counter].is_active=true;
            s[shared_counter].ram_frame_number=allocated_frame;
            shared_counter++;
        }
        else{
            temp->page_address->B[x].ram_frame_number=s[crazy].ram_frame_number;
        }
    }
    
    else{
        std::cout<<"STOP IT YOU GREEDY FUCK\n";
    }
}

int main(){
    char buffer[5000]={0};

    struct process* process_table=(struct process*) &buffer[0];
    struct page_table_allocation* page_table=(struct page_table_allocation*)&buffer[500];//we cap at ...3000->each have only 5 entries ...for each page 
    struct shared_memory_register* shared_mapping=(struct shared_memory_register*)&buffer[4000];

   // initialisation_individual_page_table_entry(&buffer[500]);
    int number_of_page_tables=2500/500;//each process will be getting a 500 page .page table..

    int process_table_size=(500)/sizeof(process);
    create_process(1,process_table,process_table_size,page_table,buffer);
    create_process(2,process_table,process_table_size,page_table,buffer);


    add_page(1,process_table,shared_mapping,1234,buffer);
    return 0;

}