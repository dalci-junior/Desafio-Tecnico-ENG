#include <Arduino_FreeRTOS.h>
#include <WiFi.h>

int led = 2;

int adcIN = 34;
int adcValue = 0;

#define SSID "*****";
#define PASSWD "*******";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer sockServ(8001);

void toggleLED(void * parameter){
  for(;;){
    digitalWrite(led, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void lerADC(void * parameter){
  for(;;){
    adcValue = analogRead(adcIN);
  }
 }

 void serverTCP(void * parameter){
  for(;;){
    WiFiClient client = sockServ.available();
    if (client) {
        while (client.connected()) {
            while (client.available()>0) {
                char c = client.read();
                client.write(c);
            }
            delay(10);
        }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(SSID, PASSWD);
  sockServ.begin();

  xTaskCreate(
    toggleLED,    // Função a ser chamada
    "Toggle LED",   // Nome da tarefa
    1000,            // Tamanho (bytes)
    NULL,            // Parametro a ser passado
    1,               // Prioridade da Tarefa
    NULL             // Task handle
  );
  
  xTaskCreate(
    serverTCP,        
    "Server TCP",     
    1000,             
    NULL,             
    1,               
    NULL               
  );
  
  xTaskCreate(
    lerADC,    
    "Ler ADC",     
    1000,            
    NULL,            
    1,                 
    NULL             
  );
}

void loop() {
  // put your main code here, to run repeatedly:

}
