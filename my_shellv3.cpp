
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
#include <sys/wait.h>
#include <cstdlib>



//POSIX MACROS ..ARE NOTHING BUT ...THINGS MUTUALLY SHARED BY ALL THE OPERATING SYSTESM ....SUCH THAT ..WE WOULD NOT HAVE 
//TO MANUALLY CALL ...Bitwise operators ....JUST LIKE FOR STATUS ..here .



std::string find_path(std::string user_input){
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
        pid_t saved_pid;
        int status;
        while((saved_pid=waitpid(-1,&status,WNOHANG))>0){
            std::cout<<"The child "<<saved_pid<<" has completed execution\n";
        }
        std::cout<<"my_shell_v3 >>";
        std::string input_line;
        getline(std::cin,input_line);
        std::stringstream ss(input_line);
        std::vector<std::string> tokens;
        std::string word;
        while(ss>>word){
            tokens.push_back(word);
        }
        bool is_pipe=false;
        int pipe_location;
        for(int i=0;i<tokens.size();i++){
            if(tokens[i]=="|"){
                //indication of a pipe from the input 
                is_pipe=true;
                pipe_location=i;
                break;
            }
        }
        if(is_pipe){
            std::vector<std::string> left_command(tokens.begin(),tokens.begin()+pipe_location);
            std::vector<std::string> right_commands(tokens.begin()+pipe_location+1,tokens.end());

            int pipefds[2];
            pipe(pipefds);

            int pid1=fork();

            if(pid1==0){
                dup2(pipefds[1],1);
                close(pipefds[0]);
                close(pipefds[1]);

                std::string full_path=find_path(left_command[0]);
                if(!full_path.empty()){
                    std::vector<char *>args;
                    for(auto &i:left_command){
                        args.push_back(&i[0]);
                    }
                    args.push_back(nullptr);
                    execve(full_path.c_str(),args.data(),nullptr);

                }
                else{
                    std::cout<<"COMMAND DOES NOT EXIST \n";
                    exit(1);
                }
            }
            int pid2=fork();
            if(pid2==0){
                dup2(pipefds[0],0);
                close(pipefds[0]);
                close(pipefds[1]);
                std::string full_path=find_path(right_commands[0]);
                if(!full_path.empty()){
                    std::vector<char *>args;
                    for(auto &i:right_commands){
                        args.push_back(&i[0]);
                    }
                    args.push_back(nullptr);
                    execve(full_path.c_str(),args.data(),nullptr);

                }
                else{
                    std::cout<<"COMMAND DOES NOT EXIST \n";
                    exit(1);
                }
            }   
            close(pipefds[0]);
            close(pipefds[1]);
            waitpid(pid1, nullptr, 0);
            waitpid(pid2, nullptr, 0);
        }

        else{
            bool is_background = false;
            if(tokens.back() == "&"){
                is_background = true;
                tokens.pop_back();
            }
            if(tokens.empty()) continue;

            int pid = fork();
            if(pid == 0){
                std::string full_path = find_path(tokens[0]);
                if(!full_path.empty()){
                    std::vector<char *> args;
                    for(auto &i: tokens) args.push_back(&i[0]);
                    args.push_back(nullptr);
                    execve(full_path.c_str(), args.data(), nullptr);
                } else {
                    std::cout << "FUNCTION DOES NOT EXIST\n";
                }
                exit(1);
            } else {  
                if(is_background){
                    std::cout << "Running background process with PID: " << pid << "\n";
                } else {
                    waitpid(pid, nullptr, 0);
                }
            }
        }
    }
}    
