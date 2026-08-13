#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   


int main(int argc,char* argv[]){
    int fd=0;
    

    if(argc>1){
        fd=open(argv[1],O_RDONLY,0644);
        if(fd==-1){
            std::cerr<<"Invalid entry for the file ....piss off cunt ";
            return 1;
        }
    }
    int inWord=0;

    char buffer[1024];

    int BytesRead;
    int cnt=0;
    int line_count=0;
    int word_count=0;
    while((BytesRead=read(fd,buffer,1024))>0){
        for(int i=0;i<BytesRead;i++){
            char c=buffer[i];
            if(c=='\n')
                line_count++;
            if(c==' ' || c=='\t' || c=='\n'){
                inWord=0;
            }
            else{
                if(inWord==0){
                    word_count++;
                    inWord=1;
                }
            }
        }
    }

    std::cout << "  " << line_count<< "  " << word_count<<"\n";

}