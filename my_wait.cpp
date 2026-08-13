


#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>





int main(){
    for(int i=0;i<4;i++){
        int pid=fork();
        
        if(pid==0){
            std::cout<<"I AM CHILD "<<i<<"\n";
            exit(0);
        }
    }
    wait(nullptr);
    wait(nullptr);
    wait(nullptr);
    wait(nullptr);

    //WE COULD USE ..FUNCTION OR IF STATEMENTS IF WE WANT OUT CHILDREN TO PERFORM DIFFERENT ..ACTIVITIES AND DESTROY THEM ..LATER WITH EXIT(0)
    std::cout<<"finally the parent executes ";

}