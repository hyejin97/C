#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int pfd[2]; // 파이프를 위한 파일 디스크립터를 두개 선언
    
    if (pipe(pfd) == -1)    perror("pipe"); // 파이프 작동, 오류 메세지 출력
    // 첫 번째 child
    switch (fork()){
        case -1 :   perror("fork");
        case 0 :
            if(close(1) == -1)    perror("close"); //표준 출력 close
            if(dup(pfd[1]) != 0)    perror("dup"); //dup : new file descriptor return(pfd[1])
            if(close(pfd[0]) == -1 || close(pfd[1]) == -1)    perror("close2"); //pfd[0]과 pfd[1] close
            execlp("ls", "ls", NULL);
            perror("execlp");
    }
    // 두 번째 child
    switch(fork())
    {
        case -1 : perror ("fork");
        case 0 :
            if(close(0) == -1)    perror("close3");
            if(dup(pfd[0]) != 0)    perror("dup2");
            if(close(pfd[0]) == -1 || close(pfd[1]) == -1)    perror("close4");
            execlp("more", "more", NULL);
            perror("execlp");
    }
    
    if(close(pfd[0]) == -1 || close(pfd[1]) == -1)    perror("close5");
    while(wait(NULL) != -1);
    return 0;
}

