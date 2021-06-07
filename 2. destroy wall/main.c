#include <stdio.h>
#include <stdlib.h>
#include <string.h>   //��� strncpy
#include <conio.h>    //��� ������ �������� AD
#include <windows.h>  //��� ���. GetKeyState, ����������� ������ �� ������ ��� ���; ��� ���� ���. ����� �������� ������ ����� �������; � ��� ����� ����������
#include <math.h>     //��� �������� ����� �� �������
#include <stdbool.h>

#define width 65    //������ ����� 80 ����., �� ����� ���� ������
#define height 25

int hitCnt=0;
int maxHitCnt=0;
int death=0;
char mas[height][width+1];  //�.�. ������ ����� ���. width, �� ���� +1 ��� ����� /
char wall[height][width+1];
int zet=0;

///////////////////////////////////
//������ (��� ������� ����)

void hidecur(){                                  //������� ������ ������; ����� ������ �� ��� ����������� ������� � ������� void setcur
HANDLE  handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

void setcur(int x, int y)     //���������� ������ � ������ �����; ����� ��� ������� ����. ����
{
    COORD coord;  //COORD - ��� ���, ���. �������� ��
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord); //1-�� ����. �� ���� ����, 2-�� ����. �����., ���� ����� ����������� ������

}

///////////////////////////////////
//����

void init(){

    for(int i=0;i<width;i++)  //������� ������: �� 0-�� ������ ��������� ��� ������� ������ #; ��������� <width \0 �� �������������
        mas[0][i]='#';
    //strncpy(mas[height-1],mas[0],width); //������ ������; ��������� � mas[i] ������ mas[0] ���-��: width; � ������ ������ �� �����

    mas[1][0]='#';           //����� ������
    mas[1][width-1]='#';    //������ ������, width-1,�.�.��������� \0
    for(int i=1;i<width-1;i++)
      mas[1][i]=' ';           //������� ����� ��������

    for(int i=2;i<height;i++)
        strncpy(mas[i],mas[1],width); //����������� 1-�� ������ ������ �� �������������

    if(hitCnt==0 && maxHitCnt==0){
        for(int i=20;i<50;i++)          //�������� ��������� �������������� �����
            wall[10][i]='$';
    }

    for(int i=20;i<50;i++)       //����� ���� ������ (wall) � ����� (mas) ������. ����� ����
        mas[10][i]=wall[10][i];
}


void show(){

    for(int i=0;i<height;i++)
    {
        printf("%s",mas[i]);
        if(i==3) printf(" hit: %i ", hitCnt);
        if(i==4) printf(" max: %i ", maxHitCnt);
        if(i==5) printf(" death: %i ", death);

        if(i<height-1) printf("\n");
    }
}
///////////////////////////////////
//�������

typedef struct{

    int x,y;      //��������� �� ��� x � y (���� �� y �� �����������)
    int w;        //������ �������

} TRacket;        //������� ��������� � ������� ��� TRacket


TRacket racket;   //�� ��������� TRacket ������� �����. racket


void initRacket(){     //��������� ������ � �������: ��������� � ������

    racket.w=7;
    racket.x=(width-racket.w)/2; //x-����� ���� �������
    racket.y=height-1;           //� ����� ���� �����
}

void putRacket(){    //Put, � �� show, �.�. show-��� ����., � put - ��� �������.

        for(int i=racket.x;i<racket.x+racket.w;i++) //racket.x+racket.w - ��� ������ ���� �������
            mas[racket.y][i]='@';
}

void moveRacket(int x){    //�������� ��������� �������

    racket.x=x;

    if(racket.x<1)  //��� ������ ����;
       racket.x=1;
    if(racket.x+racket.w>=width)  //�� width - #, � width+1 ��� \0; ������� >=
       racket.x=(width-1)-racket.w;

}
///////////////////////////////////
// �����

typedef struct{

    float x,y;      //float - �.�. ����� ����� ������������ � ����������� �� ����
    int ix,iy;      //��� (int)round(ball.y), ���. ����������� � putBall

    float alpha;    //����
    float speed;    //��� ������� ��������


} TBall;

TBall ball;      //������� ����� �� ��������� TBall

void initBall()  //���. ���������
{
    moveBall(2,2);      //������� ���, �.�. � movebBall ��������� �� float � int
    ball.alpha = -1;    //������ ���. ����
    ball.speed = 0.5;   //������ ���. ��������
}

void putBall()  //������ ����� �� ����
{
    mas[ball.iy][ball.ix]='*';     //mas[ball.y][ball.x]='*'
}

void moveBall(float x,float y){  //�.�. � ��������� x,y - float

    ball.x=x;
    ball.y=y;

    ball.ix=(int)round(ball.x);
    ball.iy=(int)round(ball.y);
}

void autoMoveBall(){

    if(ball.alpha<0) ball.alpha +=2*M_PI;      //M_PI-��� ��
    if(ball.alpha>2*M_PI) ball.alpha -=2*M_PI; //��� ������������ ���� �� 0 �� 2��


    TBall bl = ball;                          //��������� ��� ����. ������ ����� ������������ � bl

    moveBall(ball.x+cos(ball.alpha)*ball.speed     //������ � ���� ������� alpha ������������ � ����
             ,ball.y+sin(ball.alpha)*ball.speed); //���. ���. x + ��������, ���. ������� ����� cos � sin

    if((mas[ball.iy][ball.ix]=='#') || (mas[ball.iy][ball.ix]=='@') || (mas[ball.iy][ball.ix]=='$')){  //������������

        if(mas[ball.iy][ball.ix]=='$')
        {
            hitCnt++;                  //������� ���-�� ��������

            wall[ball.iy][ball.ix]=' ';
        }

        if((ball.ix != bl.ix) && (ball.iy != bl.iy))        // ���� ���. ��� �����.; bl-������ ���.
        {
            if(mas[bl.iy][ball.ix]==mas[ball.iy][bl.ix]) //������ ��. ����� ����� ������� ������ ����� => ����
                bl.alpha=bl.alpha+M_PI;
            else
            {
                if (mas[bl.iy][ball.ix]=='#')             //�����; �����. �� ��������� (�������)
                    bl.alpha=(2*M_PI - bl.alpha) + M_PI;
                else
                    bl.alpha=(2*M_PI - bl.alpha);        //�������; �����. �� ����������� (�����)
            }
        }
        else if(ball.iy==bl.iy)
            bl.alpha=(2*M_PI - bl.alpha) + M_PI;      //�������
        else
            bl.alpha=(2*M_PI - bl.alpha);             //�����

        ball=bl;      //�.�. ����������
        autoMoveBall();
    }
}

///////////////////////////////////

int main()
{
    hidecur();
    bool run=false;  //��������, ������� �� ����� (�� ������� ��������)

    initRacket();      //������� ������

    initBall();        //������� �����


    do{
        setcur(0,0); //������� ����. ����; ������ ����� ������� ����. ����

        if (run)
            autoMoveBall();

        if (ball.iy>height-2)   //���� ������ ���� �� �����
        {
            run=false;
            maxHitCnt+=hitCnt;  //���-�� �����
            hitCnt=0;
            death++;
        }

        init();       //������� ����p
        putRacket();  //������ ������ �� ��� ����
        putBall();

        show();       //����������
                                                        //GetKeyState - ���������, ������ �� ������
        if (GetKeyState('A')<0) moveRacket(racket.x-1);  //��� ������� ��������, ��� x ��������� �� -1; moveRacket �������� ������� �� x-1
        if (GetKeyState('D')<0) moveRacket(racket.x+1); //('D'<0)- ���� � ������� �����; <0 ������ ������
        if (GetKeyState('W')<0) run=true;
        if (GetKeyState('R')<0) run=false;

        if (!run) moveBall(racket.x+(racket.w/2),racket.y-1);     //��������� ����� �� ����� �������

        Sleep(10);                                  //delay �� 10 ��, ����� ������� ��������� ���� ��������

        }while(GetKeyState('P')>=0);            //VK_ESCAPE, VK_LEFT, VK_RIGHT


    return 0;
}
