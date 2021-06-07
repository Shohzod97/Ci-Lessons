#include <stdio.h>
#include <stdlib.h>
#include <string.h>   //для strncpy
#include <conio.h>    //для кнопок движения AD
#include <windows.h>  //для фун. GetKeyState, проверяющий нажата ли кнопка или нет; без этой фун. прога работала только после нажатия; а так будет независима
#include <math.h>     //для расчетов полёта по формуле
#include <stdbool.h>

#define width 65    //вообще будет 80 симв., но будет инфа справа
#define height 25

int hitCnt=0;
int maxHitCnt=0;
int death=0;
char mas[height][width+1];  //т.к. дальше будем исп. width, то надо +1 для учета /
char wall[height][width+1];
int zet=0;

///////////////////////////////////
//курсор (для очистки поля)

void hidecur(){                                  //спрячет курсор вообще; иначе моргал бы при перемещении курсора с помощью void setcur
HANDLE  handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

void setcur(int x, int y)     //переместит курсор в нужное место; нужна для очистки пред. поля
{
    COORD coord;  //COORD - это тип, кот. является ст
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord); //1-ое указ. на наше окно, 2-ое указ. коорд., куда нужно переместить курсор

}

///////////////////////////////////
//поле

void init(){

    for(int i=0;i<width;i++)  //верхняя стенка: на 0-ой строке заполняем все столбцы знаком #; благодаря <width \0 не затрагивается
        mas[0][i]='#';
    //strncpy(mas[height-1],mas[0],width); //нижняя стенка; скопирует в mas[i] строку mas[0] кол-во: width; в данном случае не нужна

    mas[1][0]='#';           //левая стенка
    mas[1][width-1]='#';    //правая стенка, width-1,т.к.последняя \0
    for(int i=1;i<width-1;i++)
      mas[1][i]=' ';           //пустота между стенками

    for(int i=2;i<height;i++)
        strncpy(mas[i],mas[1],width); //скопировали 1-ую строку подряд до предпоследней

    if(hitCnt==0 && maxHitCnt==0){
        for(int i=20;i<50;i++)          //добавили рандомную горизонтальную линию
            wall[10][i]='$';
    }

    for(int i=20;i<50;i++)       //следи чтоб сверху (wall) и здесь (mas) одинак. числа были
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
//Ракетка

typedef struct{

    int x,y;      //положение по оси x и y (хотя по y не понадобится)
    int w;        //ширина ракетки

} TRacket;        //создали структуру и назвали как TRacket


TRacket racket;   //по структуре TRacket создали перем. racket


void initRacket(){     //начальные данные о ракетке: положение и ширина

    racket.w=7;
    racket.x=(width-racket.w)/2; //x-левый край ракетки
    racket.y=height-1;           //в самом низу будет
}

void putRacket(){    //Put, а не show, т.к. show-для стат., а put - для динамич.

        for(int i=racket.x;i<racket.x+racket.w;i++) //racket.x+racket.w - это правый край ракетки
            mas[racket.y][i]='@';
}

void moveRacket(int x){    //изменяет положение ракетки

    racket.x=x;

    if(racket.x<1)  //для левого края;
       racket.x=1;
    if(racket.x+racket.w>=width)  //на width - #, а width+1 для \0; поэтому >=
       racket.x=(width-1)-racket.w;

}
///////////////////////////////////
// Шарик

typedef struct{

    float x,y;      //float - т.к. шарик будет перемещаться в зависимости от угла
    int ix,iy;      //для (int)round(ball.y), кот. понадобится в putBall

    float alpha;    //угол
    float speed;    //для расчета движения


} TBall;

TBall ball;      //создали шарик по структуре TBall

void initBall()  //нач. положение
{
    moveBall(2,2);      //сделали так, т.к. в movebBall переводит из float в int
    ball.alpha = -1;    //задали нач. угол
    ball.speed = 0.5;   //задали нач. скорость
}

void putBall()  //рисуем шарик на поле
{
    mas[ball.iy][ball.ix]='*';     //mas[ball.y][ball.x]='*'
}

void moveBall(float x,float y){  //т.к. в структуре x,y - float

    ball.x=x;
    ball.y=y;

    ball.ix=(int)round(ball.x);
    ball.iy=(int)round(ball.y);
}

void autoMoveBall(){

    if(ball.alpha<0) ball.alpha +=2*M_PI;      //M_PI-это пи
    if(ball.alpha>2*M_PI) ball.alpha -=2*M_PI; //это ограничивает угол от 0 до 2Пи


    TBall bl = ball;                          //сохранили все знач. шарика перед перемещением в bl

    moveBall(ball.x+cos(ball.alpha)*ball.speed     //именно в этой формуле alpha превращается в угол
             ,ball.y+sin(ball.alpha)*ball.speed); //нач. пол. x + скорость, кот. делится между cos и sin

    if((mas[ball.iy][ball.ix]=='#') || (mas[ball.iy][ball.ix]=='@') || (mas[ball.iy][ball.ix]=='$')){  //СТОЛКНОВЕНИЯ

        if(mas[ball.iy][ball.ix]=='$')
        {
            hitCnt++;                  //подсчет кол-во отскоков

            wall[ball.iy][ball.ix]=' ';
        }

        if((ball.ix != bl.ix) && (ball.iy != bl.iy))        // если изм. обе коорд.; bl-старое пол.
        {
            if(mas[bl.iy][ball.ix]==mas[ball.iy][bl.ix]) //правая кл. стены равна верхней клетке стены => угол
                bl.alpha=bl.alpha+M_PI;
            else
            {
                if (mas[bl.iy][ball.ix]=='#')             //стена; отраж. по вертикали (потолок)
                    bl.alpha=(2*M_PI - bl.alpha) + M_PI;
                else
                    bl.alpha=(2*M_PI - bl.alpha);        //потолок; отраж. по горизонтали (стена)
            }
        }
        else if(ball.iy==bl.iy)
            bl.alpha=(2*M_PI - bl.alpha) + M_PI;      //потолок
        else
            bl.alpha=(2*M_PI - bl.alpha);             //стена

        ball=bl;      //т.к. столкнулся
        autoMoveBall();
    }
}

///////////////////////////////////

int main()
{
    hidecur();
    bool run=false;  //проверят, запущен ли шарик (до нажатия выстрела)

    initRacket();      //создаем ракету

    initBall();        //создаем шарик


    do{
        setcur(0,0); //очищаем пред. поле; ставим перед показом след. поля

        if (run)
            autoMoveBall();

        if (ball.iy>height-2)   //уход шарика вниз за экран
        {
            run=false;
            maxHitCnt+=hitCnt;  //кол-во очков
            hitCnt=0;
            death++;
        }

        init();       //создаем полеp
        putRacket();  //ставим ракету на это поле
        putBall();

        show();       //показываем
                                                        //GetKeyState - проверяет, нажата ли кнопка
        if (GetKeyState('A')<0) moveRacket(racket.x-1);  //при нажатии передаст, что x изменился на -1; moveRacket поставит ракетку на x-1
        if (GetKeyState('D')<0) moveRacket(racket.x+1); //('D'<0)- надо с большой буквы; <0 значит нажата
        if (GetKeyState('W')<0) run=true;
        if (GetKeyState('R')<0) run=false;

        if (!run) moveBall(racket.x+(racket.w/2),racket.y-1);     //поставили шарик на центр ракетки

        Sleep(10);                                  //delay на 10 мс, чтобы ракетка двигалась чуть медленее

        }while(GetKeyState('P')>=0);            //VK_ESCAPE, VK_LEFT, VK_RIGHT


    return 0;
}
