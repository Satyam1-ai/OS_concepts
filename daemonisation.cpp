#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <string>




int main(){
    pid_t pid=fork();
    if(pid>0){
        std::cout<<"Done with the parent ....now switch off \n";
        exit(0);
    }

    std::cout<<"Now we are in the child process and our sessions is : "<<getsid(0)<<"\n";
    pid_t new_session_id=setsid();
    std::cout<<"We are in the child process and our session ID is now changed : "<<new_session_id<<" or even "<<getsid(0)<<"\n";

    //Now we double fork() to make sure ..that our ..session leader aka child process 
    //won't be ..doing something ..silly like openign ..a new terminal device/character buffer ...other the ..userr could jsut exit it 
    
    pid_t ppid=fork();
    if(ppid>0)
    {
        //abort
        std::cout<<"child...will now have another child ...THE GRANDCHILD";
        exit(0);
    }
    //Now we are in the ..ghost process ..finally ...YAYYYY ...
    std::cout<<"Completed by double fork () ..now ..i should be having a session ID of ..1 : "<<getsid(0);

}
