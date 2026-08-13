

#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   




int main(){
    int n=10;

    int fd=open("random_file.txt",O_RDWR);
    if(fd<0){
        return 1;
    }


    char buffer[1024];

    int line_count=0;
    int bytes;
    while(line_count<n && (bytes=read(fd,buffer,1024))>0){
        for(int i=0;i<bytes;i++){
            char x=buffer[i];
            write(STDOUT_FILENO,&buffer[i],1);
            if(x=='\n'){
                line_count++;
                if(line_count==n)
                    break;
            }
        }
    }

    close(fd);
}