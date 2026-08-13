
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <sstream>
#include <cstdlib>

std::string findpath(std::string user_input){
    std::string path_variable=getenv("PATH");
    std::string folder;
    std::string full_path="";
    while(!path_variable.empty()){
        int colon_position=path_variable.find(":");
        if(colon_position!=std::string::npos){
            folder=path_variable.substr(0,colon_position);
            path_variable=path_variable.substr(colon_position+1);
        }
        else{
            folder=path_variable;
            path_variable="";
        }
        std::string temp_full_path=folder+"/"+user_input;
        
        if(access(temp_full_path.c_str(),X_OK)==0){
            full_path=temp_full_path;
            break;
        }
    }
    return full_path;
}


int main(){
    while(true){
        int status;
        int saved_pid;
        while((saved_pid=waitpid(-1,&status,WNOHANG))>0){
            std::cout<<"CHILD HAS FINISHED ..EXECUTING ITS TASK ..\n";

        }
        
        std::cout<<"my_shell_v2 >>";
        std::string input_line;
        getline(std::cin,input_line);

        std::stringstream ss(input_line);
        std::vector<std::string> tokens;
        std::string word;
        while(ss>>word){
            tokens.push_back(word);
        }
        bool is_background=false;
        if(!tokens.empty() && tokens.back()=="&"){
            //it is a background process....have to execute it ...like that..
            is_background=true;
            tokens.pop_back();
            
        }

        if(!tokens.empty() && tokens.back()=="&"){
            continue;
        }


        int pid=fork();
        if(pid==0){
            std::string full_path=findpath(tokens[0]);

            if(!full_path.empty()){
                std::vector<char *> args;
                for(auto &i: tokens){
                    args.push_back(&i[0]);
                }
                args.push_back(nullptr);

                execve(full_path.c_str(),args.data(),nullptr);
            }
            else{
                std::cout<<"FUCNTION DOES NOT EXIST";
            }

            exit(1);
        }
        else{  
            if(is_background){
                std::cout<<"Runnning background process...okay.. with PID :  "<<pid<<"\n";
            }
            else{
                waitpid(pid,nullptr,0);
            }
    
        }

    }
}



