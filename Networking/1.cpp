#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <string>
#include <net/bpf.h>
#include <arpa/inet.h>

#include <iostream>


struct __attribute__((__packed__))ethernet_box{
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t ether_type;
};


struct __attribute__((__packed__))ip_box{
    uint8_t ver_ihl;
    uint8_t tos;
    uint16_t total_length;
    uint16_t id;
    uint16_t flags_fo;
    uint8_t ttl;
    uint8_t proto;
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dest_ip;
};

struct __attribute__((__packed__))udp_box{
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t length;
    uint16_t checksum;

};

struct __attribute__((__packed__))dns_header{
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;


};

struct __attribute__((__packed__))dns_tail{
    uint16_t qtype;
    uint16_t qclass;
};

uint16_t calculate_checksum(uint16_t *addr,int count) {
    uint32_t sum = 0;
    while (count > 1) {
        sum += *addr++;
        count -= 2;
    }
    if (count > 0) {
        sum += *(uint8_t *)addr;
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (uint16_t)~sum;
}

int main(){

    int fd=-1;
    char buffer[12];
    for(int i=0;i<100;i++){
        sprintf(buffer,"/dev/bpf%d",i);
        fd=open(buffer,O_RDWR);
        if(fd>0)
            break;

    }
    struct ethernet_box a;
    struct ifreq ifr;
    strcpy(ifr.ifr_name,"en0");
    if(ioctl(fd,BIOCSETIF,&ifr)<0){
        std::cout<<"ERROR";
        return 1;
    }


    a.src_mac[0]=0x0a;
    a.src_mac[1]=0x1d;
    a.src_mac[2]=0xf6;
    a.src_mac[3]=0x94;
    a.src_mac[4]=0xca;
    a.src_mac[5]=0xd4;
    a.dest_mac[0]=0xf4;
    a.dest_mac[1]=0x27;
    a.dest_mac[2]=0x56;
    a.dest_mac[3]=0x8a;
    a.dest_mac[4]=0x27;
    a.dest_mac[5]=0xb7;

    a.ether_type=htons(0x0800);

    struct ip_box b;
    b.id=htons(1);
    b.proto=0x11;
    b.ttl=64;
    b.flags_fo=0;
    b.ver_ihl=0x45;
    b.dest_ip=inet_addr("8.8.8.8");
    b.src_ip=inet_addr("192.168.1.2");
    b.tos=0;
    b.checksum=0;

    struct udp_box c;
    c.checksum=0;
    c.src_port=htons(1234);
    c.dest_port=htons(53);

    c.checksum=0;


    dns_header d;
    d.id=htons(1111);
    d.ancount=htons(0);
    d.arcount=htons(0);
    d.flags=htons(0x0100);
    d.qdcount=htons(1);
    d.nscount=htons(0);

    char dns_payload[]={0x06,'g','o','o','g','l','e',0x03,'c','o','m',0x00};
    int dns_length=sizeof(dns_payload)+sizeof(dns_header)+sizeof(dns_tail);
    c.length=htons(sizeof(udp_box)+dns_length);
    b.total_length=htons(sizeof(ip_box)+sizeof(udp_box)+dns_length);
    b.checksum = 0;
    b.checksum = calculate_checksum((uint16_t *)&b, sizeof(struct ip_box));

    struct dns_tail e;
    e.qclass=htons(1);
    e.qtype=htons(1);


    int total_packet_size = sizeof(struct ethernet_box) + sizeof(struct ip_box) + sizeof(struct udp_box) + dns_length;
    uint8_t *master_buffer = (uint8_t *)malloc(total_packet_size);

    uint8_t *cursor = master_buffer;
    memcpy(cursor, &a, sizeof(struct ethernet_box));           cursor += sizeof(struct ethernet_box);
    memcpy(cursor, &b, sizeof(struct ip_box));                 cursor += sizeof(struct ip_box);
    memcpy(cursor, &c, sizeof(struct udp_box));                cursor += sizeof(struct udp_box);
    memcpy(cursor, &d, sizeof(struct dns_header));             cursor += sizeof(struct dns_header);
    memcpy(cursor, dns_payload, sizeof(dns_payload));          cursor += sizeof(dns_payload);
    memcpy(cursor, &e, sizeof(struct dns_tail));

    ssize_t bytes_sent = write(fd, master_buffer, total_packet_size);


    int read_buff_size=4096;
    uint8_t *read_buffer=(uint8_t *)malloc(read_buff_size);

    while(1){
        int bytes_read=read(fd,read_buffer,read_buff_size);
        if(bytes_read<0)
            break;
        
        uint8_t *ptr=read_buffer;
        while(ptr <(read_buffer+bytes_read)){
            struct bpf_hdr* hdr=(struct bpf_hdr*)ptr;
            uint8_t *packet_start=ptr+hdr->bh_hdrlen;

            struct ethernet_box *recieving_ethernet_frame_header=(struct ethernet_box*) packet_start;
            if(ntohs(recieving_ethernet_frame_header->ether_type)==0x0800){
                struct ip_box *recieving_ip_packet=(struct ip_box *)(packet_start+sizeof(ethernet_box));
                if(recieving_ip_packet->src_ip==inet_addr("8.8.8.8") && recieving_ip_packet->proto==0x11){
                    struct udp_box *recieving_udp=(struct udp_box*)((uint8_t*)recieving_ip_packet+sizeof(ip_box));

                    if(ntohs(recieving_udp->dest_port)==1234){
                        std::cout<<"YAYY!!dns response caught!!\n";
                        uint8_t *recieving_dns=(uint8_t*)recieving_udp+sizeof(udp_box);
                        struct dns_header *recieving_dns_header=(struct dns_header*)recieving_dns;
                        if(ntohs(recieving_dns_header->id)==1111){
                            uint8_t *answer_pointer=recieving_dns+sizeof(dns_header)+sizeof(dns_payload)+sizeof(dns_tail);
                            uint8_t *raw_ip=answer_pointer+12;
                            printf("[+] Resolved google.com to IP: %d.%d.%d.%d\n", 
                                   raw_ip[0], raw_ip[1], raw_ip[2], raw_ip[3]);
                            
                        } 
                        close(fd);
                        return 0;
                    }
                }
                
            }
            ptr += BPF_WORDALIGN(hdr->bh_hdrlen + hdr->bh_caplen);

        }
    }

}