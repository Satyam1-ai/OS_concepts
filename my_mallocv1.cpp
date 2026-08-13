#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <cstdint>




// .bss pointer is ..given by the ..bs pointer
uint64_t *program_break=nullptr;
int user_request(){
    //how many bytes .needed ..
    //int x;
    //std::cin>>x;
    //return x;
    return 4;
}
uint64_t* easy_malloc(){
    int x=user_request();
    int slots=((x+7)/8)+1;
    uint64_t *temp=program_break;
    program_break+=slots;
    return temp;
}
int main(){
    char buffer[5000];
    uint64_t *text_portion=(uint64_t *)&buffer[0];

    uint64_t *bs_pointer=text_portion;
    memcpy(&bs_pointer[0],"Hello",sizeof("Hello"));

    bs_pointer+=1;
    program_break=bs_pointer;
    uint64_t* temp=easy_malloc();
    memcpy(temp,"ONE",sizeof("ONE"));
    std::cout<<(char*)&buffer[8];
}