
#define WIFI_SSID "Chahal"
#define WIFI_PASSWORD "84841906"


#define totalStudents 5

char tags[totalStudents][3][20] = {
  //TagID, Name, RollNo
  {"02005CF1812E", "Alfa", "1706700"},
  {"18008DA1DFEB", "Beta", "1706701"},
  {"21007BC1C853", "Gamma", "1706702"},
  {"21007B868D51", "Theta", "1706703"},
  {"02005DE8883F", "Nita", "1706704"},
};

#include <WiFi.h>
#include <HTTPClient.h>
HTTPClient http;
char input[12];        // A variable to store the Tag ID being presented
int count = 0;         // A counter variable to navigate through the input[] character array
int match = -1;

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT); //status LED
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}
void loop()
{
  if ((WiFi.status() != WL_CONNECTED)) {
    Serial.println("reconnecting...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }
  else {
    if (Serial.available()) // Check if there is incoming data in the RFID Reader Serial Buffer.
    {
      count = 0;
      while (Serial.available() && count < 12)
      {
        input[count] = Serial.read(); // Read 1 Byte of data and store it in the input[] variable
        count++;
        delay(5);
      }
      if (count == 12)
      {
        for (int i = 0; i < totalStudents; i++) {
          match = -1;
          if (theseAreSame(input, tags[i][0]))
          {
            match = i;
            break;
          }
        }
        if (match == -1)
        {
          Serial.println("You are not the student of this class!");
        }
        else
        {
          Serial.println("Name:" + String(tags[match][1]) + " || RollNo:" + String(tags[match][2]) + " || TagID:" + String(tags[match][0]));

          digitalWrite(2, HIGH);
          http.begin("https://maker.ifttt.com/trigger/bme280_readings/with/key/bDJn3jlqZeR6xjh5Uu-aJNqLdc6ruEZj0EIm05PSLUr?value1=" + String(tags[match][2]) + "&value2=" + String(tags[match][1]));
          int httpCode = http.GET();
          if (httpCode > 0) {
            String payload = http.getString();
            Serial.println(payload);
          }
          else Serial.println("ERROR: failed to mark attendence");
          http.end();
          digitalWrite(2, LOW);

        }
        for (int i = 0; i < 12; i++)
        {
          input[i] = 'F';
        }
      }
    }
  }
}

boolean theseAreSame(char input1[], char input2[]) { //Only compares 12 char arrays
  for (int i = 0; i < 12; i++)
  {
    if (input1[i] != input2[i])return false;
  }
  return true;
}
