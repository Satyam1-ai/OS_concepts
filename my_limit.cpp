#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   


#define RLIMIT_CPU   0
#define RLIMIT_FSIZE 1




struct rlim {
    int soft_limit;
    int hard_limit;
};


struct process{
    int process_id;
    struct rlim limits[6]; 
};



void set_default(struct process &p){
    for(int i=0;i<6;i++){
        p.limits[i].soft_limit=-1;
        p.limits[i].hard_limit=-1;
    }
}
void my_setlimit(struct process *p,int process_id,int new_soft_limit){
    int index=-1;
    for(int i=0;i<3;i++){
        if(p[i].process_id==process_id)
        {
            index=i;
            break;
        }
    }
    if(index!=-1)
        p[index].limits[0].soft_limit=new_soft_limit;

}


int main(){
    char buffer[2048];
    struct process* p=(struct process *)buffer;

    for(int i=0;i<3;i++){
       p[i].process_id=i+1;
       set_default(p[i]);
    }

    int x=5;
    int process_id=2;
    my_setlimit(p,process_id,x);


    for(int i=0;i<3;i++){
        std::cout<<p[i].limits[0].soft_limit<<" ";
    }

}