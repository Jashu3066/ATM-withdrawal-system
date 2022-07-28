/*******************************************************************
    A telegram bot for your ESP32 that controls the 
    onboard LED. The LED in this example is active low.

    Parts:
    ESP32 D1 Mini stlye Dev board* - http://s.click.aliexpress.com/e/C6ds4my
    (or any ESP32 board)

      = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <HTTPClient.h>
// Wifi network station credentials
#define WIFI_SSID "Jaswanth"
#define WIFI_PASSWORD "Vasu@Jashu"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "2057741416:AAFFrhGoVNSc_c6Phv2T2kvSy8-paJOFYIE"
const char* server = "http://api.thingspeak.com/update";
String apiKey = "DBRUVDHXXBN4AEYQ";     //  Enter your Write API key from ThingSpeak
const unsigned long BOT_MTBS = 1000; // mean time between scan messages
WiFiClient client;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

int bal_amount = 25000;
int notesnum[3] = {5,10,10};  //2000,1000,500 rupees notes
int pins_touch[8] = {4,15,13,12,14,27,33,32};
int vals[8] = {50,35,30,60,35,30,20,20};
void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/login")
    {
      int v;
      
      v = random(10,99);
      bot.sendMessage(chat_id, String(v)+" is your OTP"+"  "+"Enter it", "");
      int c = 0;
      int sum = 0;
      
      while(c<2)
      {
        digitalWrite(2,HIGH);
        delay(5000);
        int i;
        if(touchRead(pins_touch[0])<vals[0])
        {
          i=0;
          c++;
        }
        if(touchRead(pins_touch[1])<vals[1])
        {
          i=1;
          c++;
        }
        if(touchRead(pins_touch[2])<vals[2])
        {
          i=2;
          c++;
        }
        if(touchRead(pins_touch[3])<vals[3])
        {
          i=3;
          c++;
        }
        if(touchRead(pins_touch[4])<vals[4])
        {
          i=4;
          c++;
        }
        if(touchRead(pins_touch[5])<vals[5])
        {
          i=5;
          c++;
        }
        if(touchRead(pins_touch[6])<vals[6])
        {
          i=6;
          c++;
        }
        if(touchRead(pins_touch[7])<vals[7])
        {
          i=7;
          c++;
        }
        if(touchRead(pins_touch[5])<vals[5] && touchRead(pins_touch[6])<vals[6])
        {
          i=8;
          c++;
        }
        if(touchRead(pins_touch[7])<vals[7] && touchRead(pins_touch[6])<vals[6])
        {
          i=9;
          c++;
        }
        Serial.println(i);
        digitalWrite(2,LOW);
        sum = sum*10 + i;
      }
      bot.sendMessage(chat_id, "Verifying.....", "");
      if(sum == v)
      {
        bot.sendMessage(chat_id, "Verified", "");
        bot.sendMessage(chat_id, "/step1", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Wrong OTP, please /login again", "");
      }
    }
    if(text == "/balance")
    {
      Serial.print("Balance = ");
      Serial.println(bal_amount);
      Serial.print("2000 notes remained = ");
      Serial.println(notesnum[0]);
      Serial.print("1000 notes remained = ");
      Serial.println(notesnum[1]);
      Serial.print("500 notes remained = ");
      Serial.println(notesnum[2]);
      bot.sendMessage(chat_id, "Balance = "+String(bal_amount),"");
      bot.sendMessage(chat_id, "2000 notes remained = "+String(notesnum[0]),"");
      bot.sendMessage(chat_id, "1000 notes remained = "+String(notesnum[1]),"");
      bot.sendMessage(chat_id, "500 notes remained = "+String(notesnum[2]),"");
      bot.sendMessage(chat_id, "/step1","");
    }
    if(text == "/WithdrawlMoney")
    {
      bot.sendMessage(chat_id, "Enter the amount required in multiples of 500 or 1000 or 2000 " ,"");
      
      bot.sendMessage(chat_id, "pin 1 5000 \n pin2 10000 \n pin 3 15000 \n pin 4 20000 \n pin 5 500 \n pin 6 1000\n pin 7 2000 \n pin 8 ENTER" ,"");
   
      int sum = 0;
      
      while(touchRead(pins_touch[7])>vals[7])
      {
        digitalWrite(2,HIGH);
        delay(5000);
        int i;
        if(touchRead(pins_touch[0])<vals[0])
        {
          i=5000;
          
        }
        if(touchRead(pins_touch[1])<vals[1])
        {
          i=10000;
          
        }
        if(touchRead(pins_touch[2])<vals[2])
        {
          i=15000;
          
        }
        if(touchRead(pins_touch[3])<vals[3])
        {
          i=20000;
        }
        if(touchRead(pins_touch[4])<vals[4])
        {
          i=500;
          
        }
        if(touchRead(pins_touch[5])<vals[5])
        {
          i=1000;
        }
        if(touchRead(pins_touch[6])<vals[6])
        {
          i=2000;
        }
        
        digitalWrite(2,LOW);
        sum = sum + i;
        Serial.println(i);
        Serial.println("touch pin 8 if completes");
        delay(1000);
      }
      bot.sendMessage(chat_id,"Processing....", "");
      if(bal_amount>=sum)
      {
        int notes[3] = {0,0,0};
        processing(sum,notesnum,notes);
        bal_amount = bal_amount - sum;
        Serial.print("Balance = ");
        Serial.println(bal_amount);
        Serial.print("2000 notes remained = ");
        Serial.println(notesnum[0]);
        Serial.print("1000 notes remained = ");
        Serial.println(notesnum[1]);
        Serial.print("500 notes remained = ");
        Serial.println(notesnum[2]);
        bot.sendMessage(chat_id,"2000 notes = " + String(notes[0]) + "\n" + "1000 notes = " + String(notes[1]) + "\n" + "500 notes = " + String(notes[2]) + "\n", ""); 
        bot.sendMessage(chat_id,"Withdrawl amount = " + String(sum), "");
        bot.sendMessage(chat_id,"Balance = " + String(bal_amount), "");  
        HTTPClient http;
        http.begin(server);
        String DataSent = "api_key=" + apiKey + "&field1=" + String(bal_amount) + "&field2=" + String(notesnum[0]) + "&field3=" + String(notesnum[1]) + "&field4=" + String(notesnum[2]);
        int Response = http.POST(DataSent);  
        http.end();
        client.stop();
        delay(1000);   
        bot.sendMessage(chat_id,"/step1", "");                 
      }
      else
      {
        bot.sendMessage(chat_id,"Exceeded, please /WithdrawlMoney again", "");
      }
    }
    if(text == "/Exit")
    {
      bot.sendMessage(chat_id,"THANK YOU VISIT AGAIN :)", "");
    }
      
  
    if (text == "/step1")
    {
      String welcome = "Welcome to Our Bank, " + from_name + ".\n";
      welcome += "Select your purpose\n";
      welcome += "/balance : to check the balance\n";
      welcome += "/WithdrawlMoney : to withdraw the money\n";
      welcome += "/Exit : exit from services\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void processing(int sum,int notes[],int note[])
{
  int n1,n2,n3;
  n1 = sum/2000;
  if(n1>notes[0])
  {
    n1 = notes[0];
  }
  sum = sum - (n1*2000);
  n2 = sum/1000;
  if(n2>notes[1])
  {
    n2 = notes[1];
  }
  sum = sum - (n2*1000);
  n3 = sum/500;
  if(n3>notes[2])
  {
    n3 = notes[2];
  }
  sum = sum - (n3*500);
  notes[0] = notes[0]-n1;
  notes[1] = notes[1]-n2;
  notes[2] = notes[2]-n3;
  note[0] = n1;
  note[1] = n2;
  note[2] = n3;
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(2,INPUT);
  pinMode(pins_touch[0],INPUT);
  pinMode(pins_touch[1],INPUT);
  pinMode(pins_touch[2],INPUT);
  pinMode(pins_touch[3],INPUT);
  pinMode(pins_touch[4],INPUT);
  pinMode(pins_touch[5],INPUT);
  pinMode(pins_touch[6],INPUT);
  pinMode(pins_touch[7],INPUT);
  
  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  HTTPClient http;
  http.begin(server);
  String DataSent = "api_key=" + apiKey + "&field1=" + String(bal_amount) + "&field2=" + String(notesnum[0]) + "&field3=" + String(notesnum[1]) + "&field4=" + String(notesnum[2]);
  int Response = http.POST(DataSent);  
  http.end();
  client.stop();
  delay(1000);
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
