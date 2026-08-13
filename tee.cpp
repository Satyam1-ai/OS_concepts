#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   


int main(){
    int bytes;
    char buffer[1028];
    int fd=open("test_write.txt",O_RDWR | O_CREAT | O_TRUNC ,0644);
    while((bytes=read(0,buffer,1028))>0){
        write(1,buffer,bytes);
        write(fd,buffer,bytes);
    }

    close(fd);

}