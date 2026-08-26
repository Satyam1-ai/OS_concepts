#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <cstring>
#include <dirent.h>
#include <sys/shm.h>



int RIP=0;


struct threads{
    int id;
    int local_RIP;
    struct threads* next=nullptr;
};


struct semaphores_a{
    bool TAS_flag;
    int count;
    struct threads* queue_head=nullptr;
    struct threads* queue_tail=nullptr;
   
};  


void initialise_semaphore(struct semaphores_a *s,int count){
    s->count=count;
    s->TAS_flag=false;
}
void initialise_threads(struct threads *A){
    for(int i=0;i<5;i++){
        A[i].id=i+1;
        A[i].local_RIP=0;
    }
}
void add_to_queue(struct semaphores_a* s,struct threads* t){
    if(s->queue_head==nullptr){
        s->queue_head=t;
        s->queue_tail=t;
    }
    else{
        s->queue_tail->next=t;
        s->queue_tail=s->queue_tail->next;
    }
}

struct threads* remove_from_queue(struct semaphores_a* s){
    struct threads* temp=s->queue_head;
    s->queue_head=s->queue_head->next;
    return temp;
}


void waiting_loop(struct semaphores_a *s,int thread_id){
    if(s->count>0){
        s->count--;
    }
    else{
        std::cout<<"TOO FULL ...MAX CAPACITY ...GO TO THE QUEUE \n";
        struct threads* temp=(struct threads*)malloc(sizeof(struct threads));
        temp->id=thread_id;
        temp->local_RIP;
        temp->next=nullptr;
        add_to_queue(s,temp);
    }
}
void signal(struct semaphores_a* s){
    if(s->queue_head!=nullptr){
        struct threads* temp=remove_from_queue(s);
        s->count++;
    }
    else{
        std::cout<<"NOTHING IS WAITING";
    }
}
void simulation(struct threads* A){
    struct threads* ready_queue_head=nullptr;
    struct threads* ready_queue_tail=nullptr;
    for(int i=0;i<5;i++){
        if(ready_queue_head==nullptr)
            ready_queue_head=&A[i];
        else{
            ready_queue_tail->next=&A[i];
            ready_queue_tail=ready_queue_tail->next;
        }
    }


}



int main(){
    char buffer[5000];

    //text section 
    char* text=(char*) &buffer[0];

    text[0]='S';
    text[1]='A';
    text[2]='T';
    text[3]='Y';
    text[4]='A';
    text[5]='M';
    struct threads A[5];
    initialise_threads(A);







}