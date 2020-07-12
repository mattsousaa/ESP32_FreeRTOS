/*********************************************************
* Example for demonstration or use of binary seminar within
* an ISR
* By: Mateus Sousa
*********************************************************/

/* Use of semaphores: Tasks synchronization, Tasks and interrupts synchronization, resource constraint
 * - Binary semaphore - interrupt treatment (the interrupt releases the semaphore)
 * - Mutex - peripheral usage like I2C, SPI.. (the task releases the semaphore)
 * - Counting semaphore - when an event occurs then one task will process the first event; While this is happening another 
 *  insterrupts can occur; this interrupts can wait until the task process them in order one by one.
 * 
 */

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define LED 2

TaskHandle_t xTaskADCHandle;

SemaphoreHandle_t xSemaphore;

void vTaskADC(void * pvParameters);

void setup(){

  Serial.begin(9600);
  pinMode(LED,OUTPUT);

  xSemaphore = xSemaphoreCreateBinary();
  
  if(xSemaphore == NULL){
   Serial.println("The semaphore could not be created!");
   while(1);
  }

  xTaskCreate(vTaskADC,"TaskADC",configMINIMAL_STACK_SIZE+1024, NULL,1,&xTaskADCHandle);
  
}

void loop(){
  digitalWrite(LED,!digitalRead(LED));
  Serial.println("LED Value: " + String(digitalRead(LED)));
  vTaskDelay(pdMS_TO_TICKS(3000)); // Wait 3 sec
  xSemaphoreGive(xSemaphore); // Release semaphore after above configuration
  
}

void vTaskADC(void * pvParameters){

  int adcValue;

  while(1){
   xSemaphoreTake(xSemaphore,portMAX_DELAY);
   adcValue = analogRead(34);
   Serial.println("ADC Value: " + String(adcValue));
   Serial.println(" ");
  }
}