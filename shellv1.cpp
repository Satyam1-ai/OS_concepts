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




std::string findingpath(std::string s){
    char *path_env=getenv("PATH");
    std::string path(path_env);
    std::string directory;
    std::string final_binary_path="";
    while(!path.empty()){
        int colon_position=path.find(":");
        if(colon_position==std::string::npos){
            directory=path;
            path="";
        }
        else{
            directory=path.substr(0,colon_position);
            path=path.substr(colon_position+1);
        }

        std::string full_path=directory+"/"+s;
        if(access(full_path.c_str(), X_OK)==0){
            final_binary_path=full_path;
            break;
        }
    }

    return final_binary_path;
}




int main(){


    while(true){
        std::cout<<"my_shell_v1>";
        std::vector<std::string> tokens;

        std::string input_line;
        getline(std::cin,input_line);
        int pid=fork();
        if(pid==0){
            std::stringstream ss(input_line);
            std::string word;

            while(ss >> word){
                tokens.push_back(word);
            }

            std::string x=findingpath(tokens[0]);
            const char *full_path=&x[0];
            if(!x.empty()){
                std::vector<char *> args;
                for(auto &command : tokens){
                    char *temp=&command[0];
                    args.push_back(temp);
                }
                args.push_back(nullptr);
                execve(full_path,args.data(),nullptr);
            }
            else{
                std::cout<<"No function like this exists \n";
            }
            exit(0);
        }
        else{
            wait(nullptr);

        }
    }
}