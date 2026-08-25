#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <cstring>
#include <dirent.h>
#include <sys/shm.h>




/*
Pretty counter intuitiive ...design because .I am using forks() ...which is ..purely for processes ..however in this scenarios
to implement ...the ..TAS .and its waiting loop .i introduced shared_memory ..shmget() ...instead ..
Ideally when a fork() is passes ..it will be executing a COW ...where the .page entries are entirely filled ..but ..the mapping to the 
RAM Fragments are ..only done on demand ...however ..if we are considering the ...shared memory it bypasses the .fork()'s COW
to allow writing into the shared_segment.
*/



bool hardware_simulation(char* temp){
    bool* tas_lock=(bool*)&temp[0];
    bool old=tas_lock[0];
    tas_lock[0]=true;
    return old;
}



void waiting_loop(char *temp){
    while(hardware_simulation(temp)==true){
        std::cout<<"ANOTHER THREAD IS USING IT MATE ...GOTTA WAIT ..\n";
        usleep(100000);
    }
}


void release_lock(char *temp) {
    bool* tas_lock = (bool*)&temp[0];
    tas_lock[0] = false;
}

int main(){

    int buffer_size=5000;
    int my_key=1234;
    //I am making ..the first byte.as .the lock ..this would enssure that we are targetting this particular slice of the meory 
    //otherwise .we'd have to .......map ...each segemnt with a vairbale ..using .arrays .

    int shmid = shmget(my_key, buffer_size, IPC_CREAT | 0666);
    char* buffer=(char*) shmat(shmid,NULL,0);
    buffer[0]=0;
    char* data_payload=&buffer[1];
    bool force_child_first=true;
    int pid=fork();


    if(pid==0){
        waiting_loop(buffer);
        std::cout<<"THE CHILD IS RUNNING \n";
        data_payload[0]='C';
        usleep(500000);
        release_lock(buffer);
        exit(0);

    }
    else{
        if(force_child_first){
            usleep(100000);
        }
        waiting_loop(buffer);
        std::cout<<"THE PARENT IS RUNNING \n";
        data_payload[0]='P';
        release_lock(buffer);

        shmctl(shmid,IPC_RMID,NULL);

    }

    return 0;


}