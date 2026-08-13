#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <string>








int main(){
    int fd=open("random_file.txt",O_RDWR);
    if(fd<0)
        return 1;

    char buffer[1024];
    int bytes_read;
    char *target="SATYAM";
    int index=0;
    while((bytes_read=read(fd,buffer,1024))>0){
        for(int i=0;i<bytes_read;i++){
            if(target[index]==buffer[i]){
                index++;
                if(target[i]=='\0'){
                    std::cout<<"THIS STRING EXISTS in the file ..";
                    index=0;
                    return 1;
                }
            }
            else{
                index=0;
            }

        }
    }  

    



}