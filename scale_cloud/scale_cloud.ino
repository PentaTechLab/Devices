
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#undef int
#undef abs
#undef double
#undef float
#undef round



///////////////////////////////
///      EDIT THIS STUFF     //
///////////////////////////////

byte mac[] = {0x02, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };  //Replace with your Ethernet shield MAC
IPAddress ip(192,168,0,177);    // Your Arduino device IP address

char devid[] = "vC15EDDF304A9192";  // THIS IS THE DEVICE ID FROM PUSHINGBOX

int del=3000;  // Amount of seconds delay between posting to google docs.

///////////////////////////////
//       DONE EDITING        //
///////////////////////////////


 

char postmsg[100];
int k=0;
int temp_av = 0;
char server[] = "api.pushingbox.com";
EthernetClient client;
int data=777;
int i = 1;
void setup()
{
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("connecting...");
  
    // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
}
 
void loop(){
 
      Serial.print("Loop ");
      Serial.println(i);
      i+=1;
  
  // average temp reading for 'del' time.........................................
// 
//  for(int j=0; j<del;j++)
//  {
//    // Read local temp........................................
//    int chk = DHT11.read(DHT11PIN);
//    int temp = Fahrenheit(DHT11.temperature);
//    temp_av=temp_av+temp;
//    delay(1000);
//  }
//  

//  temp_av=0;
//    
//    
    
  client.connect(server, 80);
  delay(500);
 // Post to Google Form.............................................
  if (client.connected()) 
  {
    Serial.println("connected to server");
    sprintf(postmsg,"GET /pushingbox?devid=vC15EDDF304A9192&status=%d HTTP/1.1",data);  // NOTE** In this line of code you can see where the temperature value is inserted into the wed address. It follows 'status=' Change that value to whatever you want to post.
    client.println(postmsg);
    client.println("Host: api.pushingbox.com");
    client.println("Connection: close");
    client.println();

    Serial.println(postmsg);
    Serial.println("Host: api.pushingbox.com");
    Serial.println("Connection: close");
    Serial.println();
    data+=10;
     delay(1000);
     client.stop();
  }
  delay(1000);
  Serial.println("client not connected");
  if (!client.connected()) 
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    return;
  }
  
 
}

//
//double Fahrenheit(double celsius) // Function to convert to Fahrenheit
//{
//	return 1.8 * celsius + 32;
//}

