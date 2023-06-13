#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include  <signal.h>
#include <zconf.h>
/*
 *  не продуманный в глубь набросок, покака просто введу структуры rgba и поиграюсь с записью
 *
 */
#define FB_LOC  "/dev/fb0"
#define WB_MODE "w+b"
#define HEIGH 768
#define WIDTH 1024
#define FPS 50

// макрос доступа к нужной позиции в памяти
// fseek - принимает в себя байты сдвига по этому важно умножить на 4 байта rgba
#define POS(row, column)        \
(column + row*WIDTH)*sizeof(rgba_t)


// важно определить переменную time а то пизда
long _time_before, _time_after ;
#define FAT_TIME(src)                                   \
_time_before = micros();                                \
src;                                                    \
_time_after = micros();                                 \
printf("Code on line: %d\n", __LINE__);                 \
printf("Take time: %l", _time_after - _time_before);    \

/*
 * переименования типов
 */

typedef unsigned char byte;
/*
 *      глобальные переменные
 */
FILE *fb;

/*
 *      служебные функции
 */
void init(void){
    system("tput civis");  // выключаю мигание курсора
    fb = fopen(FB_LOC, WB_MODE);
    if (!fb) {
        printf("Error: cannot open framebuffer device.\n");
        exit(1);
    }
}

void finish(void){
    system("tput cnorm"); // включаю мигание курсора
    fclose(fb);
}




/**
 * спиздил с интенета  текущее время в микросекундах
 * для производительности.
 */
long micros(){
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

/*
 *      служебные типы
 */
typedef struct _rgb{
    byte r;
    byte g;
    byte b;
    byte a;
} rgba_t;


void test_write_to_screen(void){
    for(int i=0; i<HEIGH; i++){
        for(int j=0; j<WIDTH; j++){
            fprintf(fb, "%x", 100);
            fprintf(fb, "%x", 100);
            fprintf(fb, "%x", 100);
            fprintf(fb, "%x", 100);
        }
    }
}

void print_pixel(const int row,const int column,  rgba_t const * color){
    fseek(fb,POS(row, column), SEEK_SET);
   // fprintf(fb, "%x", color->r);
   // fprintf(fb, "%x", color->g);
   // fprintf(fb, "%x", color->b);
   // fprintf(fb, "%x", color->a);
   fprintf(fb, "%x%x%x%x", color->a, color->r, color->g, color->b);
}

void     INThandler(int);
void  INThandler(int sig)
{
    char  c;

    signal(sig, SIG_IGN);
    printf("OUCH, did you hit Ctrl-C?\n"
           "Do you really want to quit? [y/n] ");
    c = getchar();
    if (c == 'y' || c == 'Y')
        exit(0);
    else
        signal(SIGINT, INThandler);
}
void delay_until_ctrl_c(){
    signal(SIGINT, INThandler);
    while (1)
        pause();
}

int main() {
    init();
    //print_pixel(100, 100, {24,24,90,0});
    rgba_t color1 = {255,0,0,0};
    rgba_t color2 = {0,255,0,0};
    rgba_t color3 = {0,0,255,0};
    for(int i=0; i<WIDTH; i++){
        for(int j=0; j<20; j++){
            print_pixel(j,i,&color1);
            print_pixel(j+20,i,&color2);
            print_pixel(j+40,i,&color3);
        }
    }
    delay_until_ctrl_c();
    finish();

    return 0;
}
