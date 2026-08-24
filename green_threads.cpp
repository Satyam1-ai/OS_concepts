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
int memory_pointer=0;
int RO_POINTER=0;
int text_pointer=0;

int limit=1000;


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


struct green_thread{
    int id;
    int local_RIP;
    int local_RSP;
    int local_RBP;
    int is_active;
    int limit;
    char* ptr;
};

void writing_into_text(char *text){

    text[0]='M';
    text[1]='H';
    text[2]='F';
    text[3]='R';
    text[4]=3;
    text[5]='H';
    text[6]='F';
    text[7]='R';
    text[8]=2;
    text[9]='\0';
}
char* context_switch(struct green_thread *A){
    int current_running_thread;
    for(int i=0;i<4;i++){
        if(A[i].is_active==2){
            current_running_thread=i;
            break;
        }
    }
    A[current_running_thread].local_RSP=RSP;
    A[current_running_thread].local_RBP=RBP;
    A[current_running_thread].local_RIP=RIP;
    A[current_running_thread].is_active=1;



    int next_thread=(current_running_thread+1)%4;
    int count=0;
    while((A[next_thread].is_active!=1) && count<4){
        next_thread=(next_thread+1)%4;
        count++;
    }

    if(A[(next_thread)].is_active==0){
        return nullptr;
    }
    else{
        RSP=A[next_thread].local_RSP;
        RBP=A[next_thread].local_RBP;
        RIP=A[next_thread].local_RIP;
        A[next_thread].is_active=2;
        limit=A[next_thread].limit;
        return A[next_thread].ptr;
    }

}

void func_calls(char *text,char *stack_segment,struct green_thread *A){
    struct stacks calls;
    
    while(RSP<limit){

        int current_opcode=text[RIP];
        if(current_opcode=='\0'){
            current_opcode='E';
        }

        if(current_opcode=='M'){
            calls.push_integer(stack_segment,RBP);
            RBP=RSP;
            RIP++;
        }   
        else if(current_opcode=='H'){
            stack_segment=context_switch(A);
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


void initialise_thread(struct green_thread *A,char* memory_management_segment){
    for(int i=1;i<4;i++){
        A[i].ptr=&memory_management_segment[memory_pointer];
        memset(&memory_management_segment[memory_pointer],0,200);
        memory_pointer+=200;
        A[i].local_RSP=memory_pointer;
        A[i].local_RBP=0;
        A[i].limit=memory_pointer;
    }

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

    struct green_thread A[4];

    A[0].id=1;
    A[0].is_active=2;
    A[0].local_RIP=RIP;
    A[0].local_RSP=RSP;
    A[0].limit=1000;

    A[1].id=2;
    A[1].local_RIP=2;
    A[1].is_active=1;

    A[2].id=3;
    A[2].local_RIP=6;
    A[2].is_active=1;


    //we have our memory_managent ..thing from ...5000 till 9000 ...which can be used ...to create ..big dynamic memory...aka more than 
    //1024 kb ...and also the worker thread stacks ...each stack ..could be allocated ..around ..200 bytes .for this sandbox 
    initialise_thread(A,memeory_management_segment);




}