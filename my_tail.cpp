#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   


int main(){
    int fd=open("random_file.txt",O_RDWR);
    if(fd<0)
    {
        std::cerr<<"ERROR ::CANNOT OPENT THE FILE";
        return 1;
    }

    off_t file_size=lseek(fd,0,SEEK_END);
    if(file_size<=0){
        std::cerr<<"EMPTY FILE";
        return 0;
    }

    char buffer[1024];
    off_t offset =file_size;
    int line_count=0;
    int n=2;
    while(offset>0 && line_count<n){
        size_t bytes_to_read;
        if(offset>1024)
            bytes_to_read=1024;
        else
            bytes_to_read=offset;

        offset=offset-bytes_to_read;
        lseek(fd,offset,SEEK_SET);

        size_t bytes_read=read(fd,buffer,bytes_to_read);

        if(bytes_read<0)
            break;
        for(ssize_t i=bytes_read-1;i>=0;i--){
            char x=buffer[i];
            if(x=='\n'){
                line_count++;
                if(line_count==n){
                    offset=offset+i+1;
                    break;
                }
            }

        }


    }

    lseek(fd, offset, SEEK_SET);
    ssize_t bytes_to_print;
    while ((bytes_to_print = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_to_print);
    }
    close(fd);
    return 0;
}