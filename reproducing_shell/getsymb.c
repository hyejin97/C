#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
typedef enum
{S_WORD, S_BAR, S_AMP, S_SEMI, S_GT, S_GTGT, S_LT, S_NL, S_EOF} SYMBOL;

typedef enum {NEUTRAL, GTGT, INQUOTE, INWORD} STATUS;

SYMBOL getsymb(char *word){
    STATUS state = NEUTRAL;
    int c;
    char *w;
    w = word;
    
    while((c = getchar()) != EOF){
        switch (state){
            case NEUTRAL:
                switch (c){
                    case ';' : return S_SEMI;
                    case '&' : return S_AMP;
                    case '|' : return S_BAR;
                    case '<' : return S_LT;
                    case '\n' : return S_NL;
                        
                    case ' ' : continue;
                    case '\t' : continue;
                    case '>' :
                        state = GTGT;
                        continue;
                    case '"' :
                        state = INQUOTE;
                        continue;
                    default:
                        state = INWORD;
                        *w++ = c;
                        continue;
         
                }       
            case GTGT:
                        if (c == '>') return S_GTGT;
                        ungetc(c, stdin);
                        return S_GT;            
                
                
            case INQUOTE: //쿼터(") 안에 들어와 있을경우 문자열 입력 상태
                switch(c){
                    case '\\' :             // 문자열에서 \\ 문자가 입력되면
                        *w++ = getchar();    // 문자입력 받고, w 주소증가
                        continue;            // 다시 문자 입력
                        
                    case '"'  :             // INQUOTE 상태에서 " 하나가 더 입력되면
                        *w = '\0';            // 종료문자를 넘기고
                        return S_WORD;        // S_WORD 를 리턴
                        
                    default   :
                        *w++ = c;            // 그외의 문자가 들어올경우에는 w 에 저장후 주소증가
                        continue;            // 문자 입력
                }
            case INWORD: //이어지는 문자들을 위한 상태
                switch(c){

                        // INWORD 상태에서 ; & | < > \n ' ' \t 문자일 경우에는
                    case ';' :
                    case '&' :
                    case '|' :
                    case '<' :
                    case '>' :
                    case '\n':
                    case ' ' :
                    case '\t':
                        ungetc(c, stdin);
                        *w = '\0';                // 종료문자 추가하고
                        return S_WORD;            // S_WORD 리턴
                        
                    default  :
                        *w++ = c;                // 다른 문자인경우에는 문자저장하고 w 주소증가
                        continue;                // 문자 입력으로 계속

                }
        }
    }
    return S_EOF;
}

int main()
{
    char word[200];            // getsymb 함수에서 w 로 장난을 치면 결과는 word 에 저장된다.
    
    while(1)
        switch(getsymb(word))
    {
            // 결과 출력문
        case S_WORD :
            printf("S_WORD<%s>\n", word);
            break;
        case S_BAR :
            printf("S_BAR\n");
            break;
        case S_AMP :
            printf("S_AMP\n");
            break;
        case S_SEMI :
            printf("S_SEMI\n");
            break;
        case S_GT :
            printf("S_GT\n");
            break;
        case S_GTGT :
            printf("S_GTGT\n");
            break;
        case S_LT :
            printf("S_LT\n");
            break;
        case S_NL :
            printf("S_LN\n");
            break;
        case S_EOF :
            printf("S_EOF\n");
            exit(0);
    }
    
    return 0;
}

