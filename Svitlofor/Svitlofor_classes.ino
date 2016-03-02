#include <NewPing.h>
#define maximum 500

// ========================================
bool car = false;

int trig1 = 22; 
int trig2 = 24;
int trig3 = 26; 
int trig4 = 28;
int trig5 = 30; 
int trig6 = 32;

int echo1 = 23;
int echo2 = 25;
int echo3 = 27;
int echo4 = 29;
int echo5 = 31;
int echo6 = 33;


// 
int count = 0;
int usec;
int i;

class Portal : public NewPing   
{
  private:
   int num;
   int green_led;
   int red_led;
   int trig;
  public:
  bool car = false;
   Portal(int n, int trig, int pos, int g, int r):NewPing(trig, pos, maximum)
   {
    num = n; 
    green_led = g; 
    red_led  = r;
    pinMode(green_led, OUTPUT);
    pinMode(red_led, OUTPUT); 
    this->detect();
   }


void locate ()
 {
 usec=this->ping_cm(); //sends ping and estimates the duration of echo in uS
 Serial.print("Sonar" );
 Serial.print(num);
 Serial.print(" ");
 Serial.println(usec);
 
 if (usec <10 || usec==0) return;
 else
 {
 if (car == true)
 { if (usec>50) free(); } 
 else 
 { if (usec<=50) detect();}
 }
 }

void detect()
{ car=true;
  count=count+1; 
  Serial.println("Place is occupied");
  Serial.print("Total cars :");
  Serial.println(count);
  digitalWrite(green_led,LOW);
  digitalWrite(red_led,LOW);

}

void free()
{ car=false;
  Serial.println("Place is free");  
  digitalWrite(red_led,HIGH);  
  digitalWrite(green_led,HIGH);
}

};
//
  Portal P1(1,trig1,echo1,36,37);
  Portal P2(2,trig2,echo2,38,39);
  Portal P3(3,trig3,echo3,40,41);
//  Portal P4(4,trig4,echo4,5,39);
//  Portal P5(5,trig5,echo5,6,40);
//  Portal P6(6,trig6,echo6,7,41);

void setup()
{ 
  Serial.begin(9600);
  Serial.println("START"); 
}
  
void loop()
{ 
  P1.locate();
  delay(500);

  P2.locate();
  delay(500);
}
