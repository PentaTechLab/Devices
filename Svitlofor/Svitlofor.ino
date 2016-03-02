// ---------------------------------------------------------------------------
#include <NewPing.h>

#define SONAR_NUM     6 // Number or sensors.
#define MAX_DISTANCE 600 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 277// Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
int count = 0;                      // Count of cars

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(22, 23, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(24, 25, MAX_DISTANCE),
  NewPing(26, 27, MAX_DISTANCE),
  NewPing(28, 29, MAX_DISTANCE),
  NewPing(30, 31, MAX_DISTANCE),
  NewPing(32, 33, MAX_DISTANCE)
  
};

int led[SONAR_NUM] = {36,37,38,39,40,41};
int rel[SONAR_NUM] = {2,3,4,5,6,7};

int n =0;
bool car[SONAR_NUM];
void detect(int i)
{ car[i]=true;
  count=count+1; 
  Serial.println("Place is occupied");
  digitalWrite(led[i],LOW);
  digitalWrite(rel[i],LOW); 

}

void free(int i)
{ car[i]=false;
  Serial.println("Place is free");  
  digitalWrite(led[i],HIGH); 
  digitalWrite(rel[i],HIGH); 
}
void setup() {
  Serial.begin(9600);
  Serial.println("START");
  pingTimer[0] = millis() + 100;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
    
    for (n=0; n<SONAR_NUM; n++) 
    {
      pinMode(led[n], OUTPUT);
      pinMode(rel[n], OUTPUT);
      free(n);
    }

}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // The rest of your code would go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {

    
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]); 
    Serial.println("cm ");
    
    if (i == 0 || i == 5)
    {
    if (cm[i] > 33)
    {
     if (car[i] == false)
     { if (cm[i]<=377) detect(i); } 
     else 
     { if (cm[i]>377) free(i);}
     }
     else if (cm[i] == 0) free(i); 
    }
    else 
    {
      if (cm[i] > 33)
    {
     if (car[i] == false)
     { if (cm[i]<=333) detect(i); } 
     else 
     { if (cm[i]>333) free(i);}
     }      
    }
  }
  Serial.println();
}
