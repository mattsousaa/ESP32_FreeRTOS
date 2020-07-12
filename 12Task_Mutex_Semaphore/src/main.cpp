/*********************************************************
* Example to demonstrate the use of MUTEX to access resources
* and inversion of priority
* By: Mateus Sousa
*********************************************************/

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define LED 2

SemaphoreHandle_t mutex;

TaskHandle_t task1Handle, task2Handle;

void vTask1(void * pvParameters);
void vTask2(void * pvParameters);

void enviaInformacao(int i){
  xSemaphoreTake(mutex,portMAX_DELAY);
  Serial.println("Sending task information " + String(i)); // exclusive feature for both tasks
  //delay(1000);
  xSemaphoreGive(mutex);
}

void setup(){

  Serial.begin(9600);
  pinMode(LED,OUTPUT);

  mutex = xSemaphoreCreateMutex();

  xTaskCreate(vTask1,"Task1",configMINIMAL_STACK_SIZE + 1024,NULL,1,&task1Handle);
  xTaskCreate(vTask2,"Task2",configMINIMAL_STACK_SIZE + 1024,NULL,4,&task2Handle); //highest priority
  
}

void loop(){
  digitalWrite(LED,HIGH);
  vTaskDelay(pdMS_TO_TICKS(100)); 
  digitalWrite(LED,LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void *pvParameters){
  while(1){
    enviaInformacao(1);
    vTaskDelay(10);
  }
}

void vTask2(void * pvParameters){
  while(1){
    enviaInformacao(2);
    // If no delay here, starvation occurs (only task 2 will be executed because has high priority)
    vTaskDelay(10); // This delay is mandatory here because the CPU needs time to switching context
  }
}