

#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <string>







int main(){
    int pipe_fd[3][2];
    struct pollfd fds[3];
    
    for(int i=0;i<3;i++){
        pipe(pipe_fd[i]);
        pid_t pid=fork();
        if(pid==0){
            close(pipe_fd[i][0]);//reading closed...hmm
            std::string messege="HELLO FROM "+std::to_string(i)+" \n";
            write(pipe_fd[i][1],messege.c_str(),messege.size());
            close(pipe_fd[i][1]);
            exit(0);
        }
        else{
            close(pipe_fd[i][1]);
            fds[i].fd=pipe_fd[i][0];
            fds[i].events=POLLIN;
            fds[i].revents=0;
        }
    }

    std::cout<<"MONITORING ALL OF THEM PIPES ..TO SEE WHO IS ..NUMBER 1..";
    int number_of_children=3;


    while(number_of_children>0){
        int ready_count=poll(fds,3,-1);

        for(int i=0;i<3;i++){
            if(fds[i].revents & POLLIN){
                char buffer[1024];
                int bytes_read;
                if((bytes_read=read(fds[i].fd,buffer,1024))>0){
                    write(1,buffer,bytes_read);
                }
                else{
                    close(fds[i].fd);
                    fds[i].fd=-1;
                    number_of_children--;
                    std::cout<<"Ending the child  .."<<i<<" \n";
                }
            }
        }
    }

}