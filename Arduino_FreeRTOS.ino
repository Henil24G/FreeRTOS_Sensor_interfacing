#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino_FreeRTOS.h>

// Include mutex support
#include <semphr.h>

// Pin for Dallas temperature sensor
#define ONE_WIRE_BUS 2

// Data wire is plugged into digital pin 2 on the Arduino
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Task handles
TaskHandle_t temperatureTaskHandle;
TaskHandle_t microphoneTaskHandle;
TaskHandle_t printTaskHandle;

// Semaphore for serial printing
SemaphoreHandle_t serialSemaphore;

// Temperature task
void temperatureTask(void *pvParameters) {
  (void)pvParameters;

  sensors.begin();

  for (;;) {
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);

    // Give the semaphore to allow serial printing
    xSemaphoreGive(serialSemaphore);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Delay for 1 second
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Microphone task
void microphoneTask(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    // Simulating microphone input
    int microphoneValue = analogRead(A0);

    // Give the semaphore to allow serial printing
    xSemaphoreGive(serialSemaphore);

    Serial.print("Microphone value: ");
    Serial.println(microphoneValue);

    // Delay for 100 milliseconds
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// Print task
void printTask(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    // Wait until semaphore is available
    if (xSemaphoreTake(serialSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
      // Print available data on the serial monitor
      Serial.flush();
    }
  }
}

void setup() {
  Serial.begin(9600);

  // Create a semaphore to protect serial printing
  serialSemaphore = xSemaphoreCreateMutex();

  // Create tasks
  xTaskCreate(temperatureTask, "TemperatureTask", configMINIMAL_STACK_SIZE, NULL, 1, &temperatureTaskHandle);
  xTaskCreate(microphoneTask, "MicrophoneTask", configMINIMAL_STACK_SIZE, NULL, 1, &microphoneTaskHandle);
  xTaskCreate(printTask, "PrintTask", configMINIMAL_STACK_SIZE, NULL, 1, &printTaskHandle);

  // Start the scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty loop
}
