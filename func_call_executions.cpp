#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>



int RSP=1000;
int RIP=0;
int RDI=0;
int RBP=0;

int RO_POINTER=0;
int text_pointer=0;
struct stacks{

    void push_integer(char* stack_segment,int x){
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
    int pop_int(char* stack_segment){
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

    text[0]='M';
    text[1]='F';
    text[2]='R';
    text[3]=3;
    text[4]='\0';

}


void func_calls(char *text,char *ro_data,char *stack_segment){
    struct stacks calls;
    
    while(RSP<1000){

        int current_opcode=text[RIP];
        if(current_opcode=='\0'){
            current_opcode='E';
        }

        if(current_opcode=='M'){
            calls.push_integer(stack_segment,RBP);
            RBP=RSP;
            RIP++;
        }   
        else if(current_opcode=='R'){
            int n=(int)text[RIP+1];
            if(n==0){
                RIP+=2;
            }
            calls.push_integer(stack_segment,RIP+2);
            calls.push_integer(stack_segment,RBP);
            RBP=RSP;
            text[RIP+1]=(char)(n-1);
        }
        else if(current_opcode=='E'){
            RSP=RBP;
            RBP=calls.pop_int(stack_segment);
            RIP=calls.pop_int(stack_segment);

        }
        else{
            break;
        }
    }
}


void _start(char *ro_data,char *stack_segment,int n,char *s,char *env){
    struct stacks os_stack;
    os_stack.push_string(ro_data,stack_segment,env);
    uint8_t env_pointer_index = stack_segment[RSP];
    os_stack.push_string(ro_data,stack_segment,s);
    uint8_t argv_pointer_index = stack_segment[RSP];
    os_stack.push_integer(stack_segment,0);
    os_stack.push_integer(stack_segment, env_pointer_index);
    os_stack.push_integer(stack_segment,0); 
    os_stack.push_integer(stack_segment, argv_pointer_index);
    os_stack.push_integer(stack_segment,n);
    RIP=text_pointer;
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