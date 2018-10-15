#include<ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
ESP8266WebServer server(80);

FirebaseArduino FirebaseStream;

#define FIREBASE_AUTH "9eQbDkxJ6wbPAZpi5us5S3XnsgbLluwMywA5HvEo"
#define button 14// button for wifi settings
#define rst 13//button for uid
#define remote 4
#define device_id "Device 002"


int f0=0;
int f1=0;
int f2=0;
int f3=0;
int f4=0;
int f5=0;
int f6=0;
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);


const char* ssid = "Myhome1237";
const char* passphrase = "Myhome1237";
String content;
String st;
String path="";
String FIREBASE_HOST="mobile-control-home.firebaseio.com";
int statusCode;

void setup() {

Serial.begin(115200);
Serial.println("1");
        pinMode(0,OUTPUT);
        Serial.println("updated1");
        pinMode(2,OUTPUT);
        Serial.println("updated2");
        pinMode(16,OUTPUT);
        Serial.println("updated3");
        pinMode(3,OUTPUT);
        Serial.println("updated4");
        pinMode(15,INPUT);
        Serial.println("updated5");
        pinMode(5,INPUT);
        Serial.println("updated6");
        pinMode(12,INPUT);
        Serial.println("updated7");
        pinMode(2,INPUT);
        Serial.println("updated8");
  EEPROM.begin(512);
  delay(10);
  char esid[32];
Serial.println("s2");
     
       EEPROM.get(0,esid);
    delay(500);
Serial.println("s3");
     
    char epass[64];
  EEPROM.get(32,epass);
  delay(500);
Serial.println("s4");
     
     char uid[64];
  EEPROM.get(96,uid);
Serial.println("s5");
delay(500);     
    
        pinMode(button,INPUT);
        pinMode(rst,INPUT);
     
   
Serial.println("2");
    if ( sizeof(esid) !=0 && sizeof(uid)!=0) {
      
Serial.println("3");
      Serial.println(esid);
      Serial.println(uid);
      WiFi.begin(esid, epass);
      
Serial.println("4");
      Serial.println("wifi");
      
Serial.println("5");
      while((WiFi.status() != WL_CONNECTED))
      { 
Serial.println("6");
     
        if(digitalRead(button)==HIGH)
        break;
      yield();
      Serial.print(".");
      delay(100);
      }
      delay(1000);
     if (WiFi.status() == WL_CONNECTED) {
         
Serial.println("7");
     
        //FIREBASE_HOST=FIREBASE_HOST+"/ers/"+uid+"/"+device_id;
        Serial.println(FIREBASE_HOST);
        Serial.println("hi");
        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
        FirebaseStream.begin(FIREBASE_HOST, FIREBASE_AUTH);
        Serial.println("hrllo");
        
        path=path+"/users/"+uid+"/"+device_id;
        FirebaseStream.stream(path);
        String test="";
        test=path+"/test";
        
        Firebase.set(test,1);
            Serial.println("updated");
       
 
Serial.println("8");
        
     } 
    }
    if(digitalRead(button)==HIGH) 
        {
        setupAP(); 
Serial.println("9");
        }

}

bool testWifi(void)
{
  Serial.println("Waiting for Wifi to connect");
while(WiFi.status() != WL_CONNECTED)
{
  Serial.print(".");
  if(digitalRead(button)==HIGH)
  return false;
  delay(500);
}
Serial.println("true");
return true;
  
  
}
void setupAP(void)
{ 
Serial.println("10");
     
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
   
Serial.println("11");
     
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
       
Serial.println("12");
     
     }
  }
  Serial.println(""); 
  st = "<ol>";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</li>";
    }
  st += "</ol>";
  delay(100);
   
Serial.println("13");
     
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
  WiFi.softAP(ssid, passphrase, 6);
  Serial.println("softap");
   
Serial.println("14");
     
  launchWeb(1);
  Serial.println("over");
   
Serial.println("15");
     
  }
void launchWeb(int webtype) {
  Serial.println("16");
  Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
 
Serial.println("17");
     
  createWebServer(webtype);
  // Start the server 
Serial.println("18");
     
  server.begin();
  Serial.println("Server started");

}
void createWebServer(int webtype)
{ 
Serial.println("19");
     
  if ( webtype == 1 ) {
    server.on("/", []() {
       IPAddress ip = WiFi.softAPIP();
       String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
        content += ipStr;
        content += "<p>";
        content += st;
        content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64>";
        if(digitalRead(rst)==1)
        {content+="<label>UserID: </label><input name='uid' length=28>";
          }
        content += "<input type='submit'></form></html>";
        server.send(200, "text/html", content);  
      Serial.println("20");
    });
    server.on("/setting", []() {
        String qsid = server.arg("ssid");
        String qpass = server.arg("pass");
        //if(digitalRead(rst)==1)
        String uid=server.arg("uid");
        Serial.println("21");
        if (qsid.length() > 0 && qpass.length() > 0) {
          Serial.println("clearing eeprom");
          for (int i = 0; i < 96; ++i) { delay(100);EEPROM.write(i, 0); }
          if(digitalRead(rst)==1)
          for(int i=96;i<160;i++){delay(100);EEPROM.write(i,0);}
          Serial.println(qsid);
          Serial.println("");
          Serial.println(qpass);
          Serial.println("");
          Serial.println("writing eeprom ssid:");
           
Serial.println("22");
     
          for (int i = 0; i < qsid.length(); ++i)
            {
              delay(100);
              EEPROM.write(i, qsid[i]);
              Serial.print("Wrote: ");
              Serial.println(qsid[i]); 
            }
          Serial.println("23");
          Serial.println("writing eeprom pass:"); 
          for (int i = 0; i < qpass.length(); ++i)
            {
              delay(100);
              EEPROM.write(32+i, qpass[i]);
              Serial.print("Wrote: ");
              Serial.println(qpass[i]); 
            }
             
Serial.println("24");
     
            if(digitalRead(rst)==1){
            for (int i = 0; i < uid.length(); ++i)
            {
              delay(100);
              EEPROM.write(96+i, uid[i]);
              Serial.print("Wrote: ");
              Serial.println(uid[i]); 
            }    
            }
             
Serial.println("25");
     
          EEPROM.commit();
          content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
          statusCode = 200;
        }
        else {
          content = "{\"Error\":\"404 not found\"}";
          statusCode = 404;
          Serial.println("Sending 404");
        }
        server.send(statusCode, "application/json", content);
        }
        );
  
}
}

void loop(){
  Serial.println("26");
  if(digitalRead(button)==0)
  { 
    String Switch="";
    int val;
    while(WiFi.status()==WL_CONNECTED)
  
  {
     if (FirebaseStream.available()) {
    FirebaseObject event = FirebaseStream.readEvent();
    Serial.print("event: ");
    Serial.println(event.getString("event"));
    Serial.print("path: ");
    Switch=event.getString("path");
    Serial.println(event.getString("path"));
    Serial.print("data: ");
    val=event.getInt("data");
    Serial.println(event.getInt("data"));
    switch Switch:
    case "Switch 1":
    
  }
  }
Serial.println("27");
     
  while(WiFi.status()==WL_CONNECTED)
  {
  /*int a0=Firebase.getInt("fan0");
  if(Firebase.failed())
  break;
  int a2=Firebase.getInt("fan2");
  if(Firebase.failed())
  break;
  */
   
Serial.println("28");
     
  int a4=Firebase.getInt("Switch 1");
  if(Firebase.failed())
  break;
   
Serial.println("29");
     
  int a5=Firebase.getInt("Switch 2");
  if(Firebase.failed())
  break;
   
Serial.println("30");
     
  int a6=Firebase.getInt("Switch 3");
  if(Firebase.failed())
  break;
   
Serial.println("31");
     
  int a7=Firebase.getInt("Switch 4");
  if(Firebase.failed())
  break;
   
Serial.println("32");
     
  
  /*if(a0==1)
  {
  digitalWrite(0,LOW);
  }
  else if(a0==0)
  {
  digitalWrite(0,HIGH);
  }
  if(a2==1)
  {
  digitalWrite(2,LOW);
  }
  else if(a2==0)
  {
  digitalWrite(2,HIGH);
  }

  
  if(a4==1)
  {
  digitalWrite(4,LOW);
  }
  else if(a4==0)
  {
  digitalWrite(4,HIGH);
  }
  */
  if(a5==1)
  {
  digitalWrite(5,LOW);
  Serial.println("LOW");
  }
  else if(a5==0)
  {
  digitalWrite(5,HIGH);
  Serial.println("HIGH");
  }
  if(a6==1)
  {
  digitalWrite(6,LOW);
  }
  else if(a6==0)
  {
  digitalWrite(6,HIGH);
  }
  if(a7==1)
  {
  digitalWrite(7,LOW);
  
  }
  else if(a7==0)
  {
  digitalWrite(7,HIGH);
  }
  
  
  
  if(digitalRead(8)!=f0)
  {
  Firebase.setInt("feed0",!(digitalRead(8)));
  f0=digitalRead(8);
  }
  if(digitalRead(9)!=f1)
  {
  Firebase.setInt("feed1",!(digitalRead(9)));
  f1=digitalRead(9);
  }
  if(digitalRead(11)!=f3)
  {
  Firebase.setInt("feed3",!(digitalRead(11)));
  f3=digitalRead(11);
  }
  if(digitalRead(12)!=f4)
  {
  Firebase.setInt("feed4",!(digitalRead(12)));
  f4=digitalRead(12);
  }
  if(digitalRead(15)!=f5)
  {
  Firebase.setInt("feed5",!(digitalRead(15)));
  f5=digitalRead(15);
  }
  }
  }
 else if(digitalRead(button)==HIGH)
 {
   
Serial.println("33");
     
    server.handleClient();
    Serial.println("34");
 }
  }
