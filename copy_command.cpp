
#include <iostream>
#include <fcntl.h>   
#include <unistd.h>   



int main(){
    int fd=open("random_file.txt",O_RDWR);

    int newfd=open("destination_file.txt",O_WRONLY | O_CREAT | O_TRUNC,0644);
    char buffer[1024];

    int bytesRead;
    while((bytesRead=read(fd,buffer,1024))>0){
        write(newfd,buffer,bytesRead);
    }

    close(fd);
    close(newfd);
}