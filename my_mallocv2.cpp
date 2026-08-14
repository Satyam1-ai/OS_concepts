#include <iostream>
#include <fcntl.h>   
#include <iomanip>
#include <unistd.h>   
#include <cstdint>


//bs_pointer is the pointer to the .... .bss


uint64_t* program_break;


struct meta_data{
    uint16_t size;
    uint16_t status;
};

std::string user_request(){
   // std::string temp;
    //std::cout<<"Enter the string you want to enter : ";
    //std::cin>>temp;
    //return temp;
    return "hello";
}

uint64_t* new_allocation(std::string x){

    std::cout<<"STARTING A NEW BLOCK NOW ..MATE .."<<std::endl;
    int size=x.size();
    bool status=true;

    struct meta_data temporary;
    temporary.size=size;
    temporary.status=status;
    struct meta_data* header=(struct meta_data *)program_break;
    memcpy(header,&temporary,sizeof(temporary));

    int slots=((x.size()+7)/8);
    uint64_t* temp=program_break+1;
    program_break=program_break+slots+1;
    return temp;
}


uint64_t* checking_for_free(uint16_t required_size,uint64_t *bs_pointer){
    
    uint64_t *temp=bs_pointer;
    while(temp<program_break){
        struct meta_data *p=(struct meta_data*)temp;
        if(p->status==(uint16_t)0 && p->size>=required_size){
            p->status=1;
            return temp+1;
        }
        else{
            int slots=(((p->size)+7)/8);
            temp=temp+1+slots;
        }
    }
    std::cout<<"COULD NOT FIND ANY FREE SLOT UNFORTUANELTY..SO WE ARE STARTING A NEW BLOCK ..\n";
    return nullptr;
}



void my_malloc(std::string x,uint64_t* bs_pointer){
    uint64_t *checker=checking_for_free(x.size(),bs_pointer);
    if(checker==nullptr){
        uint64_t* temp=new_allocation(x);
        memcpy(temp,x.data(),x.size());
    }
    else{
        memcpy(checker,x.data(),x.size());
    }
}

int main(){
    char buffer[5000];
    const char *s="HI I AM A TEXT PORTION HERE \n";
    memcpy(buffer,s,strlen(s)+1);

    uint64_t* bs_pointer=(uint64_t*)&buffer[1000];
    program_break=bs_pointer;

    std::string x=user_request();
    my_malloc(x,bs_pointer);



}