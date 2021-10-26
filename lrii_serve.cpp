#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define MAX_EVENT_MEMBER 5
#define TCP_BUFFER_SIZE 256

int setnonblocking(int fd){
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addfd(int epollfd,int fd,bool enable_et=true){
    epoll_event event;
    event.data.fd=fd;
    event.events = EPOLLIN;
    if(enable_et){
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}

void removefd(int epollfd,int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}

void modfd(int epollfd,int fd ,int ev){
    epoll_event event;
    event.data.fd= fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP ;
    epoll_ctl(epollfd,EPOLL_CTL_MOD ,fd ,&event);
     
}


int main(int argc ,char* argv[]){
    if(argc<=2)
    {
        printf("the parameter is incorrect\n");
        return 1;
    }

    const char *ip =argv[1];
    int port = atoi(argv[2]);

    int ret=0;

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int listen_fd = socket(PF_INET,SOCK_STREAM,0);
    assert(listen_fd>=0);

    ret =bind(listen_fd,(struct sockaddr *)&address,sizeof(address) );
    assert(ret!=-1);

    ret = listen(listen_fd,5);
    assert(ret!=-1);

    epoll_event events[MAX_EVENT_MEMBER];
    int epoll_fd = epoll_create(5);
    assert(ret!=-1);
    addfd(epoll_fd,listen_fd,true);
    printf("start epoll wait\n");
    while(1){
        int number = epoll_wait(epoll_fd,events,MAX_EVENT_MEMBER,-1);
        if(number<0){
            printf("epoll failure\n");
            break;
        }
        for(int i=0;i<number;i++){
            int sockfd= events[i].data.fd;
            if(sockfd==listen_fd){
                struct sockaddr_in sock_client ;
                socklen_t client_addr_length = sizeof(sock_client);
                int connfd = accept(listen_fd, (struct sockaddr *)&sock_client,&client_addr_length);
                if(connfd < 0){
                    printf("accept a conn failure ,the errno is %d\n",errno);
                    continue;
                }
                printf("a new conn\n");
                addfd(epoll_fd,connfd);
            }else if(events[i].events & EPOLLIN){
                char buf[TCP_BUFFER_SIZE];
                int length =0;
                printf("coming a message\n");
                
                ret = recv(sockfd,&length,sizeof(length),0);
                if(ret < 0){
                    if(errno!=EAGAIN && errno!=EWOULDBLOCK){
                        close(sockfd);
                    }
                    break;
                }
                memset(buf,'\0',TCP_BUFFER_SIZE);
                while(1){                    
                    // 先读有多少字节

                    ret = recv(sockfd,buf,length,0);

                    if(ret < 0){
                        if(errno==EAGAIN || errno== EWOULDBLOCK){
                            break;
                        }
                        close(sockfd);
                        break;
                    }
                }
                printf("recv _ %d kb :%s",length,buf);
            }else{
                printf("something else happened\n");
            }
        }
    }

    close(listen_fd);
    return 0;
}
