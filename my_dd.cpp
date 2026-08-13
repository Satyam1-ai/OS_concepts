
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>







void stream_copy(char *file_location,char *destination,int n){
    char buffer[10];
    int j=0;
    int m=0;
    for(int i=0;i<n;i++){
        buffer[j]=file_location[i];
        if(j==9){
            for(int k=0;k<10;k++){
                destination[m++]=buffer[k];
            }
            j=0;
            continue;
        }
        j++;
    }

    int i=0;
    while(i<j){
        destination[m++]=buffer[i];
    }
}






int main(){
    char Sandbox[1024]={0};
    char *file_location=Sandbox;
    char *destination=Sandbox+500;
    strcpy(file_location,"KERNEL_PAYLOAD_EXEC_ROOTKIT_INITIALIZED");
    stream_copy(file_location,destination,40);

    int i=0;
    while(i<40){
        std::cout<<destination[i];
        i++;
    }
    return 0;

}
