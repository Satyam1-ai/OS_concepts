#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <string>




int main(){
    int pipe_fd[3][2];


    for(int i=0;i<3;i++){
        if(pipe(pipe_fd[i])==-1){
            std::cerr<<"There is an issue with making this pipe";
            return 1;
        }
        pid_t pid=fork();
        if(pid==0){
            close(pipe_fd[i][0]);
            std::string messege="HELLO FROM CHILD "+ std::to_string(i)+"\n";
            write(pipe_fd[i][1],messege.c_str(),messege.size());
            close(pipe_fd[i][1]);
            exit(0);
        }
    }

    for(int i=0;i<3;i++){
        close(pipe_fd[i][1]);
        int bytes_read;
        char buffer[1024];
        while((bytes_read=read(pipe_fd[i][0],buffer,1024))>0){
            write(1,buffer,bytes_read);
        }

        close(pipe_fd[i][0]);
    }
    for(int i=0; i<3; i++){
        wait(NULL);
    }
}