#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>  
#include <arpa/inet.h>
#include <sys/stat.h>    
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include "config/Config.h"
#include "MemoryInfo.h"
#include "CpuInfo.h"
#include "IoInfo.h"
#include "NetInfo.h"

#include <iostream>
using namespace std;
#define MAXBUFSIZE 1024


//mysystem(cmd_date, info_buff, 100);
//printf("%s",info_buff);
//mysystem(cmd_cpu_info_head, info_buff, 100);
//printf("%s",info_buff);
//mysystem(cmd_cpu_info, info_buff, 100);
//printf("子进程获取%s",info_buff);
int mysystem(char* cmdstring, char* buf, int len)
{
      int   fd[2];
      pid_t pid;
      int   n, count;
      memset(buf, 0, len);
      if (pipe(fd) < 0)
          return -1;
      if ((pid = fork()) < 0)
          return -1;
      else if (pid > 0)     /* parent process */
      {
          close(fd[1]);     /* close write end */
          count = 0;
          while ((n = read(fd[0], buf + count, len)) > 0 && count > len)
              count += n;
          close(fd[0]);
          if (waitpid(pid, NULL, 0) > 0)
              return -1;
      }
      else                  /* child process */
      {
          close(fd[0]);     /* close read end */
          if (fd[1] != STDOUT_FILENO)
          {
              if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
              {
                  return -1;
              }
              close(fd[1]);
          }
          if (execl("/bin/sh", "sh", "-c", cmdstring, (char*)0) == -1){
              return -1;
          }
              return -1;
      }
      return 0;
}

static char* cmd_date =(char *)"date +\"[%F %T\"] | tr '\n' ' '>> temp.his";
static char* cmd_cpu_info_head =(char *)"vmstat -a | tail -3 |head -n 2 >> temp.his";
// static char* cmd_cpu_info =(char *)"vmstat -a | tail -1 >> temp.his";
static char* cmd_cpu_info =(char *)"vmstat -a | tail -1 ";


void getCurrentTime( char * retMessage,int msgLen){
    memset(retMessage,'\0',msgLen);
    time_t now = time(NULL);
    strftime(retMessage, msgLen, "[%Y-%m-%d %H:%M]：\n", localtime(&now));

}


const char * logfile_path = "pc_info.log";




int main(int argc ,char* argv[]){
    int log_fd = open(logfile_path,O_RDWR|O_APPEND);
    char retMessage[300];

    MemoryInfo *mem_info = new MemoryInfo();
    IoInfo *io_info = new IoInfo();
    CpuInfo *cpu_info =new CpuInfo();
    NetInfo *net_info =new NetInfo();


    printf("-----------\n");

    const char *ip =argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address,sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sockfd = socket(PF_INET ,SOCK_STREAM,0);
    assert(sockfd>=0);
    printf("-----------\n");

    if(connect(sockfd,(struct sockaddr *)&server_address,sizeof(server_address))<0){
        printf("client connection failed \n");
        close(sockfd);
        return 1;
    }
    
    
    int pipefd[2];
    int ret = pipe(pipefd);
    int j=1;


    while(j++<6){
        // mem_info->getInfoString(retMessage,300);
        // write(log_fd,retMessage,strlen(retMessage));

        // io_info->getInfoString(retMessage,300);
        // write(log_fd,retMessage,strlen(retMessage));

        // cpu_info->getCpuRateInfoString(retMessage,300);
        // write(log_fd,retMessage,strlen(retMessage));

        // cpu_info->getInfoString(retMessage,300);
        // write(log_fd,retMessage,strlen(retMessage));

        net_info->getInfoString(retMessage,300,1);
        write(log_fd,retMessage,strlen(retMessage));
        int messageLen = sizeof(retMessage);
        send(sockfd,&messageLen,sizeof(messageLen),0);
        send(sockfd,retMessage,messageLen,0);
        
        
        sleep(5);
    }


    // int pid_ = fork();
    // if(pid_==-1)
    // {
    //     delete mem_info;
    //     close(log_fd);
    //     return -1;
    // }
    // if(pid_==0){
    //     close(pipefd[0]);
    //     // child
    //     while(i<=10){
    //         i++;
    //         i%=10;
            
    //         mem_info->getInfoString(retMessage,300);
    //         write(log_fd,retMessage,strlen(retMessage));

    //         io_info->getInfoString(retMessage,300);
    //         write(log_fd,retMessage,strlen(retMessage));

    //         sleep(3);
            
    //     }    
    // }else{
    //     // parent
    //     while( 1){
    //         close(pipefd[1]);
    //         read(pipefd[0],retMessage,sizeof(retMessage));
    //         printf("父进程管道: %s",retMessage);
    //     }
    // }
    delete io_info;
    delete mem_info;
    delete cpu_info;
    delete net_info;
    close(log_fd);
    return 0;
}


// int main(){
//     int port;  
//     std::string ipAddress;  
//     std::string username;  
//     std::string password;  
//     const char ConfigFile[]= "config.ini";   
//     Config configSettings(ConfigFile);  
      
//     port = configSettings.Read("email", 0);  
//     ipAddress = configSettings.Read("max_cpu_rate", ipAddress);  
//     username = configSettings.Read("max_swap_mem_rate", username);  
//     password = configSettings.Read("password", string("nms"));  
//     std::cout<<"port:"<<port<<std::endl;  
//     std::cout<<"ipAddress:"<<ipAddress<<std::endl;  
//     std::cout<<"username:"<<username<<std::endl;  
//     std::cout<<"password:"<<password<<std::endl;  
      
//     return 0;  
// }