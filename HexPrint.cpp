



#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   





int main(){
    int fd=open("random_file.txt",O_RDWR);
    char buffer[1024];
    int bytes;
    while((bytes=read(fd,buffer,1024))>0){
        for(int i=0;i<bytes;i++){
            char x=buffer[i];
            std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)x << " ";
        }
        std::cout<<"\n";
    }
    

}