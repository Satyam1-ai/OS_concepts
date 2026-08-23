
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>



int RSP=1000;
int RSI=0;
int RDI=0;
int RBI=0;

int RO_POINTER=0;
int text_pointer=0;
struct stacks{

    void push_integer(char* ro_data,char* stack_segment,int x){
        RSP=RSP-1;
        uint8_t temp=uint8_t(x);
        memcpy(&stack_segment[RSP],&temp,1);
    }
    void push_string(char *ro_data,char *stack_segment,char *s){
        memcpy(&ro_data[RO_POINTER],s,strlen(s)+1);
        int temp=char(RO_POINTER);
        RO_POINTER+=strlen(s)+1;
        RSP=RSP-1;
        stack_segment[RSP]=temp;
    }
    int pop_int(char *ro_data,char* stack_segment){
        int x=(int)stack_segment[RSP];
        RSP++;
        return x;
    }
    char* pop_string(char *ro_data,char* stack_segment){
        char *s=&ro_data[stack_segment[RSP]];
        RSP++;
        return s;
    }
};

void writing_into_text(char *text){

    //static ..writing
    int text_indexer=0;
    memcpy(&text[text_indexer],"main()",7);
    text_indexer+=7;
    memcpy(&text[text_indexer],"func1()",8);
    text_indexer+=8;
    memcpy(&text[text_indexer],"func2()",8);
}


void _start(char *ro_data,char *stack_segment,int n,char *s,char *env){
    struct stacks os_stack;
    os_stack.push_string(ro_data,stack_segment,env);
    uint8_t env_pointer_index = stack_segment[RSP];
    os_stack.push_string(ro_data,stack_segment,s);
    uint8_t argv_pointer_index = stack_segment[RSP];
    os_stack.push_integer(ro_data, stack_segment,0);
    os_stack.push_integer(ro_data, stack_segment, env_pointer_index);
    os_stack.push_integer(ro_data, stack_segment,0); 
    os_stack.push_integer(ro_data, stack_segment, argv_pointer_index);
    os_stack.push_integer(ro_data, stack_segment,n);
    RSI=text_pointer;
}
int main(){

    char buffer[10000];
    char* text=&buffer[0];//text portion
    char* ro_data=&buffer[400];
    char* heap_start=&buffer[3000]; //sbrk(0)
    char* memeory_management_segment=&buffer[5000];
    char *stack_segment=&buffer[9000];

    writing_into_text(text);
    //INITIALLY THE RSI will be at the _start 
    int number_of_arguments=2;
    char *s="ls -l";
    char *env="USER=SATYAM PATH=/bin/";
    _start(ro_data,stack_segment,number_of_arguments,s,env);

}