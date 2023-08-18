/* Non-Volatile Storage (NVS) Read and Write a Struct - Example

MIT License

Copyright (c) 2021 Michael Campbell

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

#define BLINK_GPIO 2
#define BUFFSIZE 2000

void app_main() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Done\n");

        gpio_reset_pin(BLINK_GPIO);
        /* Set the GPIO as a push/pull output */
        gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
        // Read
        printf("Reading string from NVS ... ");
        struct NVS_Data {
            //    long cap1;
            //    long cap2;
            int number1;
            int number2;
            uint8_t s_led_state;
            char character;
            char buffer[BUFFSIZE];
        };
        struct NVS_Data nvs_struct;
        char test_string[] = "Here is a load of text to test the NVS string storage. ";
        int textsize = 0;
        size_t required_size;
        err = nvs_get_blob(my_handle, "nvs_struct", NULL, &required_size );
        err = nvs_get_blob(my_handle, "nvs_struct", (void *)&nvs_struct, &required_size);
        switch (err) {
            case ESP_OK:
                printf("Done\n\n");
                printf("Buffer = %s\n\n", nvs_struct.buffer);
                printf("Number 1 = %d\n\n", nvs_struct.number1);
                printf("Number 2 = %d\n\n", nvs_struct.number2);
                printf("Character = %c\n\n", nvs_struct.character);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                required_size = sizeof(nvs_struct);
                memset(nvs_struct.buffer, 0, sizeof(nvs_struct.buffer));
                nvs_struct.number1 = 0;
                nvs_struct.number2 = 1000;
                nvs_struct.character = 'a';
                nvs_struct.s_led_state = 0;
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
        textsize = strlen(nvs_struct.buffer);
        printf("Buffer is %d characters in size\n", textsize);
        if (textsize > (BUFFSIZE - 60)) {
            printf("Text is getting too large, so restarting the ESP32!\n\n");
            for (int i = 6; i >= 0; i--) {
                printf("Restarting in %d seconds...\n", i);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
            esp_restart();
        }

        // Write
        printf("Adding text to NVS Struct... ");
        strncat(nvs_struct.buffer, (const char*)test_string, sizeof(test_string));
        nvs_struct.number1++;
        nvs_struct.number2--;
        nvs_struct.character++;
        printf("Turning the LED %s!\n", nvs_struct.s_led_state == true ? "ON" : "OFF");
        gpio_set_level(BLINK_GPIO, nvs_struct.s_led_state);
        nvs_struct.s_led_state = !nvs_struct.s_led_state;
        err = nvs_set_blob(my_handle, "nvs_struct", (const void*)&nvs_struct, required_size);
        //err = nvs_set_str(my_handle, "nvs_struct", (const char*)nvs_struct.buffer);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }

    printf("\n");

    // Restart module
    for (int i = 4; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
