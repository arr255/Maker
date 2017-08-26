  #include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include "pitches.h"
//接线
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define id 0x9341

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define TS_MINX 230
#define TS_MINY 150
#define TS_MAXX 918
#define TS_MAXY 900
//颜色值：采取16位，RGB分别为5位，6位，5位
#define  BLACK   0xFFFF
#define BLUE    0xFFE0
#define RED     0x07FF
#define GREEN   0xF81F
#define WHITE   0x0000
Adafruit_TFTLCD tft(LCD_CS,LCD_CD,LCD_WR,LCD_RD,LCD_RESET);
TouchScreen ts=TouchScreen(XP,YP,XM,YM,300);
TSPoint t;
int tonePin=10;
int homeButton=0;//home界面按钮选择
static bool funAlready=0;//选择界面
int homeScreen=0;//主界面
int buttonAlready=0;//文字正常显示
static String a="";//数学运算；
static int flag=-1;//数学运算；
static double aToNum=0;//数学运算
static double result=0;//数学运算
static int start=0;//time
static int s,m,h=0;//time
unsigned long stTime,stopTime,loopTime;//time
int basicTone[][2]={
  '5',NOTE_G3,'6',NOTE_A3,'7',NOTE_B3,'1',NOTE_C4,'2',NOTE_D4,'3',NOTE_E4,'4',
  NOTE_F4,'5',NOTE_G4,'6',NOTE_A4,'7',NOTE_B4,'1',NOTE_C5,'2',NOTE_D5};
int toneTable[][2]={-5,NOTE_G3,-6,NOTE_A3,-7,NOTE_B3,0,0,1,NOTE_C4,2,NOTE_D4,3,NOTE_E4,4,NOTE_F4,
                   5,NOTE_G4,6,NOTE_A4,7,NOTE_B4,11,NOTE_C5,12,NOTE_D5,13,NOTE_E5,14,NOTE_F5,
                   15,NOTE_G5,16,NOTE_A5,17,NOTE_B5};
void setHomeButton(String );
int color2048[]={tft.color565(49,63,77),tft.color565(17,27,37),tft.color565(19,31,55),tft.color565(13,78,134),
                 tft.color565(10,106,156),tft.color565(10,131,160),tft.color565(9,162,196),tft.color565(18,49,142),
                 tft.color565(18,51,158),tft.color565(19,55,175),tft.color565(18,58,192),tft.color565(17,61,209)};
int att2048[16]={0};//每个块的值大小
int mDirect;//移动方向
int toneLength;
String musicName[10];
int startMusic=-1;
int musicNumber=4;
int simpleTone[65];
int changedTone[65];
int noteDurations[65];
class Piano
{
  private:
    int width;
    int height;
    int x;
    int y;
    int color;
    int pressColor;
    int note;
   public:
    void setKey(int x,int y,int width,int height,int color,int pressColor,int note,char charKey)
    {
      TSPoint t=ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);     
      t.x=map(t.x,230,TS_MAXX,0,320);
      t.y=map(t.y,150,TS_MAXY,0,240);
      tft.drawChar(x,y+height/2,charKey,WHITE,BLACK,3);
      if(t.x>y && t.x<y+height  && t.y>x && t.y<x+width){
        tft.fillRect(x,y,width,height,pressColor);
        tone(tonePin,note,100);
        tft.fillRect(x,y,width,height,color);
        }
      else if(buttonAlready==0){
        tft.fillRect(x,y,width,height,color);
     }
    }
     void defaultSet(){
        if(funAlready==0){
          tft.fillScreen(WHITE);
          funAlready=1;
        }
          for(int i=0;i<4;i++){
               setKey(65*i,80,50,70,BLACK,GREEN,basicTone[i][1],basicTone[i][0]);
          }
          for(int i=4;i<8;i++){
               setKey(65*(i-4),160,50,70,BLACK,GREEN,basicTone[i][1],basicTone[i][0]);
          }
          for(int i=8;i<12;i++){
              setKey(65*(i-8),240,50,70,BLACK,GREEN,basicTone[i][1],basicTone[i][0]);
          }
          setHomeButton("Piano");
          buttonAlready=1;
     }
};
class Calculator{
  public:
    int key;//按键序号；    
    void setKey(int x,int y,int width,int height,int color,int pressColor,String s)
    {
      String keys[]={"c","R","%","/","7","8","9","x","4","5","6","-","1","2","3","+","0","!",".","="};   
      TSPoint t=ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);     
      t.x=map(t.x,230,TS_MAXX,0,320);
      t.y=map(t.y,150,TS_MAXY,0,240);
      tft.drawString(x+20,y+height/2,s,BLACK,color,3);
      if(t.x>y && t.x<y+height  && t.y>x && t.y<x+width){
      tft.fillRect(x,y,width,height,pressColor);
      tft.fillRect(x,y,width,height,color);
      tft.drawFastHLine(x,y,width,BLACK);
      tft.drawFastVLine(x,y,height,BLACK);
        for(int m=0;m<4;m++){
          for(int n=0;n<5;n++){
            if(t.y>60*m && t.y<60*(m+1)&& t.x>48*n+80 &&t.x<48*(n+1)+80){
              key=4*n+m;
              tft.fillRect(0,0,160,80,tft.color565(165,42,42 )); 
              break;
            }
          }
        }
        if(key==4||key==5||key==6||key==8||key==9||key==10||key==12||key==13||key==14||key==16||key==18){
         if(flag==5){
            flag=-1;
            a="";
            result=0;
          }
          if(a==""){
                if(key==16){
                   a="";
                }
                else{
                  a=keys[key];
                }
          }
          else{
            a+=keys[key];
          }
          tft.setCursor(130-10*a.length(),45);
          tft.setTextSize(2);
          tft.print(a);
        }
      if(key==0){
        a="";
        result=0;
        flag=-1;
        tft.setCursor(130-10*a.length(),45);
        tft.setTextSize(2);
        tft.print(a);
      }
      if(key==3||key==7||key==11||key==15||key==19){
        aToNum=atof(a.c_str());
        switch(flag){
        case 1:{
            if(aToNum==0){
            tft.setCursor(70,45);
            tft.setTextSize(2);
            tft.print("error!");
            Calculator cal;
            cal.defaultSet();
          }
          else{
            result/=aToNum;
            break;
          }
          }
        case 2:{
          result*=aToNum;
          break;
        }
        case 3:{
          result-=aToNum;
          break;
        }
        case 4:{
          result+=aToNum;
          break;
        }
        }
        a="";
        if(flag==-1){
        result=aToNum;
        }
        if(key==3){
          flag=1;
        }
        if(key==7){
          flag=2;
        }
        if(key==11){
          flag=3;
        }
        if(key==15){
          flag=4;
        }
            String buff;
            buff=(String) result;
            tft.setCursor(130-10*buff.length(),45);
            tft.setTextSize(2);
            tft.print(result);
        if(key==19){
          flag=5;
        }
      }
      if(key==2){
        result/=100;
        String buff;
        buff=(String) result;
        tft.setCursor(130-10*buff.length(),45);
        tft.setTextSize(2);
        tft.print(result);
      }
      if(key==17){
        aToNum=atof(a.c_str());
        int r=aToNum;
        if(aToNum!=0){
          for(int i=r-1;i>0;i--){
            r*=i;
          }
        }
        else{
          r=1;
        }
        String buff;
        buff=(String) r;
        tft.setCursor(130-10*buff.length(),45);
        tft.setTextSize(2);
        tft.print(r);
        result=r;
        flag=5;
      }
      if(key==1){
        result=random(1000);
        String buff;
        buff=(String) result;
        tft.setCursor(130-10*buff.length(),45);
        tft.setTextSize(2);
        tft.print(result);
      }
      }
      else if(buttonAlready==0){
        tft.fillRect(x,y,width,height,color);
        tft.drawFastHLine(x,y,width,BLACK);
        tft.drawFastVLine(x,y,height,BLACK);
     }
    }
  void defaultSet()
  {
      if(funAlready==0){
          tft.fillScreen(WHITE);
          funAlready=1;
      }
        Calculator cal;
        String keys[]={"c","R","%","/","7","8","9","x","4","5","6","-","1","2","3","+","0","!",".","="};
        for(int i=0;i<3;i++){
          cal.setKey(60*i,80,60,48,tft.color565(55,55,55),tft.color565(211,211,211),keys[i]);
        }
        for(int i=4;i<7;i++){
          cal.setKey(60*(i-4),128,60,48,tft.color565(55,55,55),tft.color565(211,211,211),keys[i]);
        }
        for(int i=8;i<11;i++){
          cal.setKey(60*(i-8),176,60,48,tft.color565(55,55,55),tft.color565(211,211,211),keys[i]);
        }
        for(int i=12;i<15;i++){
         cal.setKey(60*(i-12),224,60,48,tft.color565(55,55,55),tft.color565(211,211,211),keys[i]);
        }
        for(int i=16;i<19;i++){
          cal.setKey(60*(i-16),272,60,48,tft.color565(55,55,55),tft.color565(211,211,211),keys[i]);
        }
        for(int i=0;i<5;i++){
          cal.setKey(180,80+48*i,60,48,tft.color565(0,96,133),tft.color565(211,211,211),keys[4*i+3]);
        }
        setHomeButton("Caculator");
        buttonAlready=1;
  }
};
class Home
{
  public :
  int onclick()
  {
      TSPoint t=ts.getPoint();
       pinMode(XM, OUTPUT);
       pinMode(YP, OUTPUT);     
      t.x=map(t.x,230,TS_MAXX,0,320);
      t.y=map(t.y,150,TS_MAXY,0,240);
      if(t.x>80 && t.x<160  && t.y>0 && t.y<80)
      {
           homeButton=1;
           buttonAlready=0;
      }
      if(t.x>80 &&t.x<160 && t.y>80 &&t.y<160)
      {
          homeButton=2;
          buttonAlready=0;
      }
      if(t.x>80 &&t.x<160 &&t.y>160&&t.y<240){
        homeButton=3;
        buttonAlready=0;
      }
      if(t.x>160 && t.x<240  && t.y>0 && t.y<80){
        homeButton=4;
        buttonAlready=0;
      }
      if(t.x>160 && t.x<240 && t.y>80&&t.y<160){
        homeButton=5;
        buttonAlready=0;
      }
      return homeButton;
  }
  void setTile(int x,int y,String s)
  {
    tft.fillRect(x*80,y*80+80,78,78,tft.color565(95,158,160));
    tft.drawString(x*80,y*80+39+80,s,BLACK,tft.color565(95,158,160),2);
  }
  void defaultSet()
  {
    if(funAlready==0){
          tft.fillScreen(WHITE);
          funAlready=1;
        }
    tft.fillRect(0,0,160,80,tft.color565(165,42,42 ));
    tft.drawString(60,40,"Maker",RED,tft.color565(165,42,42),2);
    tft.fillRect(160,0,80,80,GREEN);
    tft.drawString(180,30,"Home",BLACK,GREEN,2);
    Home hm;
    hm.setTile(0,0,"piano");
    hm.setTile(1,0,"Math");
    hm.setTile(2,0,"Time");
    hm.setTile(0,1,"2048");
    hm.setTile(1,1,"Music");
    homeScreen=0;
    funAlready=0;
    homeButton=0;
    buttonAlready=0;
    h=0;s=0;m=0;start=0;
    for(int i=0;i<16;i++){
       att2048[i]=0;
         }
    startMusic=-1;
  }
};
class Time{
  public:
  void setTime(){
    tft.drawRect(0,80,240,120,BLACK);
    tft.setTextSize(3);
    tft.fillRect(20,90,200,100,WHITE);
    tft.setCursor(80,120);
    tft.print(h);
    tft.print(":");
    tft.print(m);
    tft.print(":");
    tft.print(s);
  }
  void defaultSet(){
    if(funAlready==0){
      tft.fillScreen(WHITE);
      funAlready=1;
        }
    startTime();
    setButton();
    setTime();
    setHomeButton("Time");
    buttonAlready=1;
  }
  void startTime(){
    stTime=millis();
    loopTime=stTime-stopTime;
    if(start==1&&loopTime>1000){
        if(s<59){
          s++;
        }
        else{
          if(m<59){
            s=0;
            m++;
          }
          else{
            s=0;m=0;h++;
          }
        }
        stopTime=stTime; 
    }
  }
  void setButton(){ 
      TSPoint t=ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);     
      t.x=map(t.x,230,TS_MAXX,0,320);
      t.y=map(t.y,150,TS_MAXY,0,240);
      if(t.x>240 && t.x<280  && t.y>130 && t.y<220){
          tft.fillRect(130,240,90,40,tft.color565(44,44,44));
          tft.fillCircle(175,260,10,BLACK);
          start=0;h=0;s=0;m=0;
          tft.fillRect(130,240,90,40,tft.color565(30,100,30));
          tft.fillCircle(175,260,10,BLACK);
          tft.fillRect(20,240,90,40,RED);
          tft.fillTriangle(60,250,60,270,80,260,BLACK);
        }
      if(t.x>240 && t.x<280  && t.y>20 && t.y<120){
        if(start==1){
          start=0;
          delay(10);
          tft.fillRect(20,240,90,40,RED);
          tft.fillTriangle(60,250,60,270,80,260,BLACK);
        }
        else{
          tft.fillRect(20,240,90,40,GREEN);
          tft.fillRect(60,250,20,20,BLACK);
          start=1;
          delay(10);
        }
        }
      if(buttonAlready==0){
        if(start==0){
            tft.fillRect(20,240,90,40,RED);
            tft.fillTriangle(60,250,60,270,80,260,BLACK);
        }
        else{
            tft.fillRect(20,240,90,40,GREEN);
        }
            tft.fillRect(130,240,90,40,tft.color565(30,100,30));
            tft.fillCircle(175,260,10,BLACK);
     }
  }
};
class Game2048{
  public:
     bool fail;
     int hmKey;
     Game2048(){
      hmKey=0;
     }
     void setTile(int x,int y,int color,int number)
    {
        tft.setTextSize(2);
        tft.setTextColor(BLACK);
        tft.setCursor(60*x+20,60*y+110);
        tft.fillRect(60*x,60*y+80,59,59,color);
        tft.print(number);
        att2048[4*x+y]=number;
        }
      void newTile(){
        int tSeq;//初始化的块序列号
        int f=0;
        while(f==0){
          tSeq=random(16);
          if(att2048[tSeq]==0){
              int row=tSeq/4;//行
              int col=tSeq%4;//列
              int number=(random(2)>0.8? 2 :4);//数值大小
              int nSeq=log(number)/log(2);//数值排序，以便于利用颜色
              setTile(row,col,color2048[nSeq-1],number);//设置块
              att2048[tSeq]=number;
              f=1;
          }
          }
      }
      int getDirections(){
        TSPoint t=ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);     
        t.x=map(t.x,230,TS_MAXX,0,320);
        t.y=map(t.y,150,TS_MAXY,0,240);
        if(t.x>80 && t.x<140 && t.y >60 && t.y< 180)
            return 0;//top
        if(t.x>140 && t.x<260 && t.y >180 && t.y< 240)
            return 1;//right
        if(t.x>260 && t.x<320 && t.y >60 && t.y< 180)
            return 2;//bottom
        if(t.x>140 && t.x<260 && t.y >0 && t.y< 60)
            return 3;//left
        if(t.x>0 && t.x<80  && t.y>160 && t.y<240 &&t.z>0)
        {
               hmKey=1;
               return -2;
          }
            return -1;
      }
      void merge(int preTile,int currTile){
            int preRow=preTile/4;
            int preCol=preTile%4;
            int currRow=currTile/4;
            int currCol=currTile%4;
            int currColor=log(att2048[currTile])/log(2);
            int preColor=log(att2048[preTile])/log(2);
            if(att2048[currTile]!=0){
              if(att2048[preTile]==0){
                att2048[preTile]=att2048[currTile];
                att2048[currTile]=0;
                setTile(preRow,preCol,color2048[currColor-1],att2048[preTile]);
                tft.fillRect(60*currRow,60*currCol+80,59,59,tft.color565(33,71,120));
                fail=0;
              }
              else if(att2048[preTile]==att2048[currTile]){
                att2048[preTile]=att2048[preTile]*2;
                att2048[currTile]=0;
                setTile(preRow,preCol,color2048[currColor],att2048[preTile]);
                tft.fillRect(60*currRow,60*currCol+80,59,59,tft.color565(33,71,120));   
                if(att2048[preTile]==2048)
                {
                  tft.fillRect(0,140,240,120,WHITE);
                  tft.drawString(60,170,"You win!",RED,WHITE,2);
                }
                fail=0;
              }
          }
      }
      void move2048(){
        fail=1;
        int direct=getDirections();
        while(direct==-1){
          direct=getDirections();     
        }
        int j;
        switch(direct){
          case 3:
                for(int i=4;i<16;i++){
                  j=i;
                  while(j>=4){
                    merge(j-4,j);
                    j-=4;
                  }
                }
                break;
            case 2:
                  for(int i=14;i>=0;i--){
                    j=i;
                    while(j%4!=3){
                      merge(j+1,j);
                      j+=1;
                    }
                  }
                break;
            case 1:
                  for(int i=11;i>=0;i--){
                    j=i;
                    while(j<=11){
                      merge(j+4,j);
                      j+=4;
                    }
                  }
                break;
            case 0:
                  for(int i=1;i<16;i++){
                      j=i;
                      while(j%4!=0){
                        merge(j-1,j);
                        j-=1;
                      }
                  }
                 break;
        }
        for(int i=0;i<16;i++){
          if(att2048[i]==0){
            fail=0;
          }
        }
        if(fail){
              tft.fillRect(0,140,240,120,WHITE);
              tft.drawString(60,170,"You fail!",RED,WHITE,2);
        }
        if(!fail){
          newTile();
        }
      }
      void defaultSet(){
         if(funAlready==0){
          tft.fillScreen(WHITE);
          for(int i=0;i<16;i++){
            int row=i/4;
            int col=i%4;
            tft.drawRect(60*row,60*col+80,60,60,tft.color565(130,130,130));
            tft.fillRect(60*row,60*col+80,59,59,tft.color565(33,71,120));
          }
          newTile();
          newTile();
          funAlready=1;
            }
        setHomeButton("2048");
        move2048();
        buttonAlready=1;
        if(hmKey){
             Home hm;
             homeScreen=0;
             funAlready=0;
             homeButton=0;
             buttonAlready=0;
             h=0;s=0;m=0;start=0;
             hm.defaultSet();
             for(int i=0;i<16;i++){
                att2048[i]=0;
            }
        }
        
      }
};
class Music{
  public:

    void setKey(int y)
    {
      TSPoint t=ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);     
      t.x=map(t.x,230,TS_MAXX,0,320);
      t.y=map(t.y,150,TS_MAXY,0,240);
      tft.setCursor(20,110+60*y);
      tft.setTextSize(2);
      if(t.x>60*y+80 && t.x<60*y+140){
        tft.fillRect(0,60*y+80,240,60,GREEN);
        tft.setCursor(20,110+60*y);
        tft.print(y+1);
        tft.print(".");
        tft.print(musicName[y]);
        startMusic=y;
        }
      else if(buttonAlready==0){
        tft.fillRect(0,60*y+80,240,60,tft.color565(120,32,12));
        tft.setCursor(20,110+60*y);  
        tft.print(y+1);
        tft.print(".");
        tft.print(musicName[y]);
     }
    }
   void setTile(){
        tft.fillRect(0,60*startMusic+80,240,60,tft.color565(120,32,12));
        tft.setCursor(20,110+60*startMusic);  
        tft.print(startMusic+1);
        tft.print(".");
        tft.print(musicName[startMusic]);
     }
    void getSrc(int i){
          if(i==0){
            toneLength=29;
            int noteDurationsSrc[]={
              8,8,4,4,4,4,4,
              8,8,4,4,4,4,4,
              8,8,4,4,4,4,2,8,
              8,8,4,4,4,2,4
              };
            int simpleToneSrc[]={
              -5,-5,-6,-5,1,-7,0,
              -5,-5,-6,-5,2,1,0,
              -5,-5,5,3,1,-7,-6,0,
              4,4,3,1,2,1,0
            };
            for(int i=0;i<toneLength;i++){
                simpleTone[i]=simpleToneSrc[i];
                noteDurations[i]=noteDurationsSrc[i];
               }
            toneChange(simpleTone);
          }
          if(i==1){
            int simpleToneSrc[]={1,1,5,5,6,6,5,
                                 4,4,3,3,2,2,1,
                                 5,5,4,4,3,3,2,
                                 5,5,4,4,3,3,2
                                 };
               int noteDurationsSrc[]={4,4,4,4,4,4,2,
                                       4,4,4,4,4,4,2,
                                       4,4,4,4,4,4,2,
                                       4,4,4,4,4,4,2
               };
               toneLength=28;
              for(int i=0;i<toneLength;i++){
                  simpleTone[i]=simpleToneSrc[i];
                  noteDurations[i]=noteDurationsSrc[i];
               }
              toneChange(simpleTone);
           }
           if(i==2){
            int simpleToneSrc[]={3,3,4,5,5,4,3,2,1,1,2,3,3,2,2,
                                 3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,
                                 2,2,3,2,2,3,4,3,1,2,3,4,3,2,1,2,-5,
                                 3,3,3,4,5,5,4,3,4,2,1,1,2,3,2,1,1
                                 
                                 };
               int noteDurationsSrc[]={4,4,4,4,4,4,4,4,4,4,4,4,3,8,2,
                                       4,4,4,4,4,4,4,4,4,4,4,4,3,8,2,
                                       4,4,4,4,4,8,8,4,4,4,8,8,4,4,4,4,4,
                                       4,4,4,4,4,4,4,4,8,8,4,4,4,4,3,8,2
               };
               toneLength=64;
              for(int i=0;i<toneLength;i++){
                  simpleTone[i]=simpleToneSrc[i];
                  noteDurations[i]=noteDurationsSrc[i];
               }
              toneChange(simpleTone);
           }
         if(i==3){
            int simpleToneSrc[]={
                                 3,3,3,3,3,3,3,5,1,2,3,0,
                                 4,4,4,4,4,3,3,3,3,3,2,2,1,2,5
                                 };
               int noteDurationsSrc[]={
                                  8,8,4,8,8,4,8,8,6,16,3,8,
                                  8,8,6,16,8,8,8,16,16,8,8,8,8,4,4
               };
               toneLength=27;
              for(int i=0;i<toneLength;i++){
                  simpleTone[i]=simpleToneSrc[i];
                  noteDurations[i]=noteDurationsSrc[i];
               }
              toneChange(simpleTone);
           }
    }
    void toneChange(int arr[]){
      for(int i=0;i<toneLength;i++){
        for(int j=0;j<19;j++){
          if(arr[i]==toneTable[j][0]){
             changedTone[i]=toneTable[j][1];
             break;
          }
        }
      }
    }
    void playMusic(){
        for(int i=0;i<toneLength;i++) {
            int noteDuration=1000/noteDurations[i];
            tone(tonePin,changedTone[i],2*noteDuration);
            int pause=noteDuration*1.3;
            delay(pause);
            noTone(10);
            Serial.println(changedTone[i]);
          }
          setTile();
          startMusic=-1;
    }
    void defaultSet(){
       if(funAlready==0){
        tft.fillScreen(WHITE);
        funAlready=1;
        musicName[0]="birthday";
        musicName[1]="Little Star";
        musicName[2]="Happy Song";
        musicName[3]="Jingle Bell";
          }
      for(int i=0;i<musicNumber;i++){
        setKey(i);
        tft.drawRect(0,80+60*i,240,60,BLACK);
      }
      if(startMusic!=-1){
       getSrc(startMusic);
       playMusic();
      }
      setHomeButton("Music");
      buttonAlready=1; 
    }
};
void setHomeButton(String title)
{
  if(buttonAlready==0){
     tft.fillRect(0,0,160,80,tft.color565(165,42,42 )); 
     tft.fillRect(160,0,80,80,GREEN);
     buttonAlready=1;
  }
   if(homeButton!=2){
   tft.drawString(60,40,title,RED,tft.color565(165,42,42),2);
   }
   tft.drawString(180,30,"Home",BLACK,GREEN,2);
   TSPoint t=ts.getPoint();
   pinMode(XM, OUTPUT);
   pinMode(YP, OUTPUT);     
    t.x=map(t.x,230,TS_MAXX,0,320);
    t.y=map(t.y,150,TS_MAXY,0,240);
    if(t.x>0 && t.x<80  && t.y>160 && t.y<240)
    {
         Home hm;
         homeScreen=0;
         funAlready=0;
         homeButton=0;
         buttonAlready=0;
         h=0;s=0;m=0;start=0;
         hm.defaultSet();
         for(int i=0;i<16;i++){
            att2048[i]=0;
         }
    }
}
void setup()
{
  tft.begin(id);
  tft.fillScreen(0x0000);
  Serial.begin(9600);
  tft.drawString(60,120,"welcome",RED,GREEN,3);
  delay(1000);
  tft.fillScreen(0x0000);
  Home hm;
  hm.defaultSet();
  for(int i=0;i<14;i++)
  {
    Serial.print(toneTable[i][0]);
    Serial.print("  :   ");
    Serial.print(toneTable[i][1]);
    Serial.print("\n");
  }
}
void loop()
{
  Home hm;
  if(homeScreen==0){
    hm.onclick();
    funAlready=0;
  }
  switch (homeButton){
    case 1:{
      homeScreen=1;
      Piano piano;
      piano.defaultSet();
      break;
    }
    case 2:{
      homeScreen=1;
      Calculator cal;
      cal.defaultSet();
      break;
    }
    case 3:{
      homeScreen=1;
      Time tm;
      tm.defaultSet();
      break;
    }
    case 4:{
      homeScreen=1;
      Game2048 game2048;
      game2048.defaultSet();
      break;
    }
    case 5:{
      homeScreen=1;
      Music music;
      music.defaultSet();
      break;
    }
  }
}
