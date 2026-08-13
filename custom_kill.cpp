//LETS KILL ...

#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <string>




struct process{
    int pid;
    int state;
    int pending_states;   
};

void custom_handler(int signum){
    if(signum==10)
        std::cout<<"INTERRUPT INTERCEPTED ..AND VOILA ..WE HAVE ....something";
}

void kill(struct process *p,int process_id,int signal,int n){
    bool is_valid=false;
    int index=-1;
    for(int i=0;i<n;i++){
        if(p[i].pid==process_id){
            std::cout<<"FOUND..DELETING NOW ";  
            is_valid=true; 
            index=i;
            break;    
        }
    }

    if(is_valid){
        p[index].pending_states=signal;
    }
}
void clean_up(struct process *p,int index){
    p[index].pid=-1;
    p[index].pending_states=-1;
    p[index].state=-1;

    
}

void kernel_scheduler(struct process *p,int n){
    for(int i=0;i<n;i++){
        if(p[i].pending_states==9){
            std::cout<<"INITIATING CLEAN UP \n";
            clean_up(p,i);
            //GOES TO ZOMBIE STATE ..AFTER REMOVING FROM THE MEMORY ..
        }
    }
}




int main(){
    //signal(10,custom_handler);
    char buffer[2048];
    struct process *process_table=(struct process*)buffer;
    //12 for each ..
    for(int i=0;i<3;i++){
        struct process temp;
        temp.pid=i+1;
        temp.state=1;
        temp.pending_states=-1;
        process_table[i]=temp;
    }

    for(int i=0;i<3;i++){
        std::cout<<process_table[i].pid<<" ";
    }
    std::cout<<'\n';

    kill(process_table,2,9,3);
    kernel_scheduler(process_table,3);


    for(int i=0;i<3;i++){
        std::cout<<process_table[i].pid<<" ";
    }



}