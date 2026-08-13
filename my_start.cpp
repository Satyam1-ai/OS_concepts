
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <cstring>
#include <dirent.h>
#include <cstdint>




uint64_t  SIM_RDI=0;
uint64_t* SIM_RSI=nullptr;

uint64_t* global_env=nullptr;

struct process{
    int pid;
    int state;
    int pending_states;      
};

int main(){

    char buffer[5000];
    struct process *p=(struct process*)buffer;
    uint64_t *data_block=(uint64_t*)&buffer[1000];

    std::memcpy(data_block,"hello",sizeof("hello"));

    //STACK segment
    const char *text;
    uint64_t *stack_segment=(uint64_t *)&buffer[2000];
    stack_segment[0]=2;
    text="ls";
    memcpy(&stack_segment[1],text,strlen(text)+1);
    text="-l";
    memcpy(&stack_segment[2],text,strlen(text)+1);


    stack_segment[3]=0;
    text="USER=elliot";
    memcpy(&stack_segment[4],text,strlen(text)+1);

    text="PATH=usr/local/bin";
    memcpy(&stack_segment[6],text,strlen(text)+1);

    stack_segment[9]=0;



    SIM_RDI=stack_segment[0];
    SIM_RSI=&stack_segment[1];
    



    global_env=&stack_segment[4];

    while((*global_env)!=0){
        std::cout<<(char *)global_env<<"\n";
        int string_length=strlen((char *)global_env);
        int slots_used=(string_length/8)+1;
        global_env+=slots_used;
    }



    //RESISTERS ARE LOADED ..ENTIRELY ....NOW WE PROCEED TO MAIN. ...FUNCTION ...
/*
    uint64_t *x=stack_segment;
    for(int i=0;i<=9;i++){
        if(i==0 || i==3 || i==9)
            std::cout<<stack_segment[i]<<" ";
        else
            std::cout<<(char *)&stack_segment[i]<<" ";
    }
*/
}