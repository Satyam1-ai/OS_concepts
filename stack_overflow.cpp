
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



struct stack{
    char *base;
    int RSP;
    void init(char* buffer_start){
        base=buffer_start;
        RSP=1800;
    }
    void push(char* x,int size){
        RSP=RSP-size+1;
        memcpy(&base[RSP],x,size);
        RSP-=1;
        base[RSP]=(uint8_t)size;  
    }
    void pop(char *destination){
        int current_element_size=base[RSP];
        RSP++;
        memcpy(destination,&base[RSP],current_element_size);
        RSP=RSP+current_element_size;
    }
};

int main(){

    char buffer[2000];
    struct stack *dynamic_stack_frame=(struct stack* )&buffer[200];


    dynamic_stack_frame->init(buffer);
    dynamic_stack_frame->push("Hello\0",6);
    dynamic_stack_frame->push("Satyam\0",7);
    char x[100];
    dynamic_stack_frame->pop(x);
    std::cout<<x<<std::endl;
    std::cout<<(int)dynamic_stack_frame->base[dynamic_stack_frame->RSP];



}