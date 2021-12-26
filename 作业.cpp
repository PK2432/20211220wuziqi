#include<termio.h>
#include <locale.h>
#define OUTTYPE "%s "
int getch(void);  
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cstring>  
#define CHESS 5                                     //宏定义棋子数
#define SLEN 10                                     //宏定义棋盘大小
#define BOARD (SLEN*2+1)
void gezi(void);
void xiaqi(void);
int guize(void);
int panding(void);
void jieguo(int); 
void gotoxy(int,int);
const char* board[BOARD][BOARD]={0};  
int check[SLEN][SLEN]={0};
int flag=-1;                    // -1==white  1==black 
int row=0, col=0,  con; 
int count=0;
const char * BLACK="○",  * WHITE ="●",   * POS  ="¤";// UTF-8 棋子棋盘
const char * SIDE="|", * TOP ="-";
const char * SPACE=" ";
int main(void)
 {
    int O;        
    gezi(); 
    O=guize();  
    jieguo(O); 
    return 0;
 }

void gezi(void)
{
for(int j=0;j<BOARD;j++)
	for(int k=0;k<BOARD;k++)
		{
			if(j%2==0)
				board[j][k]=TOP;
			else if (j%2==1||k%2==0)
				board[j][k]=SIDE;
			else
				board[j][k]=SPACE;
		}	
}
void xiaqi(void)
{
    system("clear"); 
    for(int j=0;j<BOARD;j++){
        for(int k=0;k<BOARD;k++) 
        {                     
            if((j-1)%2==0 && (k-1)%2==0){ 
                int sam=check[(j-1)/2][(k-1)/2];
                if(sam==0) printf("  ");
                else if(sam==-1) printf(OUTTYPE,WHITE);
                else if (sam==1) printf(OUTTYPE,BLACK);                
            }
            else printf(OUTTYPE,board[j][k]);
        }
        putchar('\n');
    }
    if(flag==1) printf("  %s  Black turn",BLACK);         //回合输出
    else     printf("  %s  White turn",WHITE);
}   

#define LOCATE  check[row][col]       
#define NOTCHESS  LOCATE==0          
#define ISWHITE  LOCATE==-1
#define ISBLACK  LOCATE==1
#define GOTOCURR gotoxy(1+2*(1+2*col),2+2*row);  
#define MOVE  {if(NOTCHESS)	{ LOCATE=3; }else if(ISWHITE)	{ LOCATE=-2; }else if(ISBLACK) 	{ LOCATE=2; }}          
#define PUTDOWN {if(flag==-1) {  LOCATE=flag; } else if(flag==1) { LOCATE=flag; }} 
#define PLACE {PUTDOWN; flag=-flag; count++;}           

int guize(void) 
{
    LOCATE=3;             
    xiaqi();               
    system("stty -echo");  
    while(1)
    {   
            while((con=getch())!='`')               //退出按键
            {
                if(con=='w'||con=='s'||con=='a'||con=='d'||con==' '){
                    switch(con){              
                        case 'w': if(row>0)      row--;
				 else row=SLEN-1;MOVE; break;
                        case 's': if(row<SLEN-1) row++;
				 else row=0;     MOVE; break;
                        case 'a': if(col>0)      col--;
				 else col=SLEN-1;MOVE; break;
                        case 'd': if(col<SLEN-1) col++;
				 else col=0;     MOVE; break;
                        case ' ': if(LOCATE==3)  PLACE; break;
                        default : break;
                    }
                }
                if(count==SLEN*SLEN) return 0;       //平局
                if(panding()) return 1;     //返回分出胜负
                break;
            }
            if(con=='`') return 2;        
	}
}

int panding(void)          
{
    bool judge;                
    int  pres;                
    for(int j=0;j<SLEN;j++)       
        for(int k=0;k<SLEN;k++)
            if(check[j][k]!=0){
                pres=check[j][k];
                judge=true;
                if(j<=SLEN-CHESS && k<=SLEN-CHESS){
                    for(int l=1;l<CHESS;l++)
                        if(check[j+l][k+l]!=pres) judge=false;          
                    if(judge==true) return 1;   //正对角
                }
                judge=true;
                if(j<=SLEN-CHESS && k>=CHESS-1){
                    for(int l=1;l<CHESS;l++)
                        if(check[j+l][k-l]!=pres) judge=false;          
                    if(judge==true) return 1;    //反对角
                }
                judge=true;
                if(k<=SLEN-CHESS){
                    for(int l=1;l<CHESS;l++)
                        if(check[j][k+l]!=pres) judge=false;
                    if(judge==true) return 1;     //水平
                }
                judge=true;
                if(j<=SLEN-CHESS){
                    for(int l=1;l<CHESS;l++)
                        if(check[j+l][k]!=pres) judge=false; 
                    if(judge==true) return 1;    //竖直
                }
            }
    return 0;
}
void jieguo(int o)            
{
    gotoxy(0,BOARD+2);             
    if(o){
        if(o==2) printf("Force exit\n");
        else if(flag==1) printf("\t-----%s White Won-----\n",WHITE);
        else        printf("\t-----%s Black Won-----\n",BLACK);
    }
    else printf("\t-----KO-----\n");
}

void gotoxy(int x,int y) //移动光标
  {  printf("%c[%d;%df",0x1b,y,x);   }

int getch(void)                             //获取键盘输入
{
    struct termios tm,tm_old;
    int fd=0,ch;
    if(tcgetattr(fd,&tm)<0) return -1;
    tm_old=tm;
    cfmakeraw(&tm);
    if(tcsetattr(fd,TCSANOW,&tm)<0) return -1;
    ch=getchar();
    if(tcsetattr(fd,TCSANOW,&tm_old)<0) return -1;
    return ch;
}
