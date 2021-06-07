#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>

#define width 65
#define height 25

int hitCnt=0;
int maxHitCnt=0;
int death=0;

int lvlNow=1;
int lvlEnd=2;

char mas[height][width+1];
char wall[height][width+1];      //для учета разрушаемой стенки


///////////////////////////////////
//курсор (для очистки поля)

void hidecur(){
HANDLE  handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

///////////////////////////////////
//поле

void init(int lvl){            //на входе level

    for(int i=0;i<width;i++)
        mas[0][i]='#';

    mas[1][0]='#';
    mas[1][width-1]='#';
    for(int i=1;i<width-1;i++)
      mas[1][i]=' ';

    for(int i=2;i<height;i++)
        strncpy(mas[i],mas[1],width);
    //////////////////////////////

        if(lvl==1)
    {
        if(hitCnt==0 && maxHitCnt==0)
        {
            for(int i=20;i<50;i++)
                wall[10][i]='$';        //добавили стенку, которая будет разрушаться
        }

    for(int i=20;i<50;i++)         //следи чтоб сверху (wall) и здесь (mas) одинак. числа были
        mas[10][i]=wall[10][i];   //вставили разрушаемую стенку в поле; при каждом проходе цикла в int main массив wall будет менять и копироваться в поле
    }

    if(lvl==2)
    {
    if(hitCnt==0 && maxHitCnt==0)
        {
            for(int i=20;i<50;i++)
                wall[10][i]='$';        //добавили стенку, которая будет разрушаться
        }

    for(int i=20;i<50;i++)         //следи чтоб сверху (wall) и здесь (mas) одинак. числа были
        mas[10][i]=wall[10][i];   //вставили разрушаемую стенку в поле; при каждом проходе цикла в int main массив wall будет менять и копироваться в поле
    //////////////

    if(hitCnt==0 && maxHitCnt==0)
        {
            for(int i=20;i<50;i++)
                wall[5][i]='$';        //добавили стенку, которая будет разрушаться
        }

    for(int i=20;i<50;i++)         //следи чтоб сверху (wall) и здесь (mas) одинак. числа были
        mas[5][i]=wall[5][i];

    }
    //////////////

    if(lvl==3)
    {
        if(hitCnt==0 && maxHitCnt==0)
        {
        for(int j=1;j<10;j++)
            for (int i=3;i<64;i+=7)
                wall[j][i]='$';
        }

        for(int j=1;j<10;j++)
            for (int i=3;i<64;i+=7)
                mas[j][i]=wall[j][i];
    }

}

void show(){

    for(int i=0;i<height;i++)
    {
        printf("%s",mas[i]);
        if(i==1) printf(" Level: %i ", lvlNow);
        if(i==3) printf(" hit: %i ", hitCnt);
        if(i==4) printf(" max: %i ", maxHitCnt);
        if(i==5) printf(" death: %i ", death);

        if(i<height-1) printf("\n");
    }
}

void showPreview(){

    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t  LEVEL %d", lvlNow);
    Sleep(1000);
    system("cls");
}

void showWon(){

    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t  YOU WON!!!");
    Sleep(100000);
    system("cls");
}

///////////////////////////////////
//Ракетка

typedef struct{

    int x,y;
    int w;

} TRacket;


TRacket racket;


void initRacket(){

    racket.w=7;
    racket.x=(width-racket.w)/2;
    racket.y=height-1;
}

void putRacket(){

        for(int i=racket.x;i<racket.x+racket.w;i++)
            mas[racket.y][i]='@';
}

void moveRacket(int x){

    racket.x=x;

    if(racket.x<1)
       racket.x=1;
    if(racket.x+racket.w>=width)
       racket.x=(width-1)-racket.w;

}
///////////////////////////////////
// Шарик

typedef struct{

    float x,y;
    int ix,iy;

    float alpha;
    float speed;


} TBall;

TBall ball;

void initBall()
{
    moveBall(2,2);
    ball.alpha = -1;
    ball.speed = 0.5;
}

void putBall()
{
    mas[ball.iy][ball.ix]='*';
}

void moveBall(float x,float y){

    ball.x=x;
    ball.y=y;

    ball.ix=(int)round(ball.x);
    ball.iy=(int)round(ball.y);
}

void autoMoveBall(){

    if(ball.alpha<0) ball.alpha +=2*M_PI;
    if(ball.alpha>2*M_PI) ball.alpha -=2*M_PI;


    TBall bl = ball;

    moveBall(ball.x+cos(ball.alpha)*ball.speed
             ,ball.y+sin(ball.alpha)*ball.speed);

    if((mas[ball.iy][ball.ix]=='#') || (mas[ball.iy][ball.ix]=='@') || (mas[ball.iy][ball.ix]=='$')){ //отраж. от разрушамеой $

        if(mas[ball.iy][ball.ix]=='$')          //при столкновении с $
        {
            hitCnt++;

            wall[ball.iy][ball.ix]=' ';        //заменили на пробел
        }


        if((ball.ix != bl.ix) && (ball.iy != bl.iy))
        {
            if(mas[bl.iy][ball.ix]==mas[ball.iy][bl.ix])
                bl.alpha=bl.alpha+M_PI;
            else
            {
                if (mas[bl.iy][ball.ix]=='#')
                    bl.alpha=(2*M_PI - bl.alpha) + M_PI;
                else
                    bl.alpha=(2*M_PI - bl.alpha);
            }
        }
        else if(ball.iy==bl.iy)
            bl.alpha=(2*M_PI - bl.alpha) + M_PI;
        else
            bl.alpha=(2*M_PI - bl.alpha);

        ball=bl;
        autoMoveBall();
    }
}

///////////////////////////////////

int main()
{
    hidecur();
    bool run=false;

    initRacket();

    initBall();

    showPreview();

    do{
        setcur(0,0);

        if (run)
            autoMoveBall();

        if (ball.iy>height-2)
        {
            run=false;
            maxHitCnt+=hitCnt;
            hitCnt=0;
            death++;
        }


            if(maxHitCnt>=3)
            {
                lvlNow++;
                run=false;    //сбросили игру; нарисуется новое поле
                maxHitCnt=0;  //сбросили счетчики
                hitCnt=0;
                if(lvlNow!=lvlEnd)
                    showPreview();
                else showWon();
            }


        init(lvlNow);

        putRacket();
        putBall();

        show();

        if (GetKeyState('A')<0) moveRacket(racket.x-1);
        if (GetKeyState('D')<0) moveRacket(racket.x+1);
        if (GetKeyState('W')<0) run=true;
        if (GetKeyState('R')<0) run=false;

        if (!run) moveBall(racket.x+(racket.w/2),racket.y-1);

        Sleep(8);

        }while(GetKeyState('P')>=0);


    return 0;
}
