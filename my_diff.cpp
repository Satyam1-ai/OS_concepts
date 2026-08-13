
#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <poll.h>
#include <cstring>

#include <stdio.h>
#include <dirent.h>



void diff(char *file_name_a,char *file_name_b){
    const char* p=file_name_a;
    const char* q=file_name_b;
    int line_num=1;
    bool line_is_matching=true;

    while(*p!='\0' || *q!='\0'){
        if(*p!='\n' && *p!='\0' && *q!='\n' && *q!='\0'){
            if(*p!=*q){
                line_is_matching=false;
            }
            p++;
            q++;
            continue;
        }
        if(*p=='\n' && *q!='\n' && *q!='\0'){
            line_is_matching=false;
            while(*q!='\n' && *q!='\0'){
                q++;    
            }
        }

        if(*q=='\n' && *p!='\n' && *p!='\0'){
            line_is_matching=false;
            while(*p!='\n' && *p!='\0'){
                p++;
            }
        }
        if (*p != '\n' && *p != '\0' && *q == '\0') {
            line_is_matching = false;
            p++; 
            continue;
        }
        if (*q != '\n' && *q != '\0' && *p == '\0') {
            line_is_matching = false;
            q++;
            continue;
        }



        if(line_is_matching==false){
            std::cout<<"The line "<<line_num<<" does not match\n";
        }
        else{
            std::cout<<"The line "<<line_num<<" matches\n";
        }
        if(*p=='\n'){
            p++;
        }
        if(*q=='\n'){
            q++;
        }

        line_num++;
        line_is_matching=true;

    }

}



int main(){
    char sandbox[1024];

    char *file_name_a=sandbox;
    char *file_name_b=sandbox+500;

    std::strcpy(file_name_a,"Hello\nmynameissatyam\n");
    std::strcpy(file_name_b,"Hello\nmynameissat");

    diff(file_name_a,file_name_b);

    return 0;

}