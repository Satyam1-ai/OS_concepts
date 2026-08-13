
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   





int main(int argc,char* argv[]){
    int fd=0;
    if(argc>1){
        fd=open(argv[1],O_RDWR | O_CREAT,0644);

        if(fd==-1){
            std::cerr<<"Invalid argument!!Fuck off ....gimme the right anwer "<<"\n";
            return 1;
        }
    }

    char buffer[1024];
    int bytesRead;  
    while((bytesRead=read(fd,buffer,1024))>0){
        write(1,buffer,bytesRead);
    }

    if(fd>0)
        close(fd);

    return 0;
}
