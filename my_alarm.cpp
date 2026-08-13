


#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <string>




typedef void (*alarm_handler)();

struct process{


    int pid;
    int state;
    int pending_states;  
    int alarm_countdown;
    alarm_handler on_alarm_fire;
    
};



void my_alarm(struct process* p,int process_id,int ticks,alarm_handler x){

    for(int i=0;i<3;i++){
        if(p[i].pid==process_id){
            p[i].alarm_countdown=ticks;
            p[i].on_alarm_fire=x;
            break;
        }
    }

}

void kernel_clock_tick(struct process *p,int n){
    for(int i=0;i<n;i++){
        if(p[i].pid>0 && p[i].alarm_countdown>0){
            p[i].alarm_countdown--;
            if(p[i].alarm_countdown==0 && p[i].on_alarm_fire!=nullptr){
                std::cout<<"EXECUTING THE HANDLER FUNCITON \n";
                p[i].on_alarm_fire();
            }
        }
    }
}


void handler_1(){
    std::cout<<"EXECUTING THE .HANDLER 1 FUCNTION WHEN ..PID 1's ..ALARM GOES OFF \n";
}


void handler_3(){
    std::cout<<"EXECUTING THE .HANDLER 3 FUCNTION WHEN ..PID 3's ..ALARM GOES OFF \n";

}


int main(){
    char buffer[2048];

    struct process *p=(struct process *)buffer;
    
    for(int i=0;i<3;i++){
        p[i].pid=i+1;
        p[i].state=-1;
        p[i].pending_states=0;
        p[i].alarm_countdown=-1;
        p[i].on_alarm_fire=nullptr;
    }

    my_alarm(p,1,5,handler_1);
    my_alarm(p,3,3,handler_3);


    for(int i=1;i<=5;i++){
        std::cout<<"\n SYSTEM TICK "<<i<<"----";
        kernel_clock_tick(p,3);
    }

}


