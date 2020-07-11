/*********************************************************
* Example that demonstrates how to create a queue, add and read
* queue data between two tasks
* Por: Mateus Sousa
*********************************************************/
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define LED 2

QueueHandle_t xFila;

TaskHandle_t xTask1Handle, xTask2Handle;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup(){

  BaseType_t xReturned;

  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  xFila = xQueueCreate(1, sizeof(int)); 

  if(xFila == NULL){
    Serial.println("Could not create queue");
    while(1);
  }

  xReturned = xTaskCreate(vTask1,"Task1",configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask1Handle);

  if(xReturned == pdFAIL){
    Serial.println("Could not create Task 1");
    while(1);
  }

  xReturned = xTaskCreate(vTask2,"Task2",configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask2Handle);

  if(xReturned == pdFAIL){
    Serial.println("Could not create Task 2");
    while(1);
  }
}

void loop(){
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void *pvParameters){

  int count = 0;

  while(1){
    if(count < 10){
      xQueueSend(xFila,&count,portMAX_DELAY);
      count++;
    } else{
      count = 0;
      vTaskDelay(pdMS_TO_TICKS(5000)); //Count is reset and wait 5 sec
    }
    vTaskDelay(pdMS_TO_TICKS(500)); //Put in the queue at each 500ms (count < 10)
  }
}

void vTask2(void *pvParameters){

  int valorRecebido = 0;

  while(1){
    if(xQueueReceive(xFila,&valorRecebido,pdMS_TO_TICKS(1000)) == pdTRUE){
      Serial.println("Received value: " + String(valorRecebido));
    } else{ // 1sec expired
      Serial.println("TIMEOUT");
    }
  }
}