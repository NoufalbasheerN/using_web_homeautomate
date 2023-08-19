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



void app_main() {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
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

       
        // Read
        printf("Reading string from NVS ... ");
        struct NVS_Data {
          
            char ssid[32];
            char password[64]
        };
        struct NVS_Data nvs_struct;
        char wifi_ssid[] = "MachDatum_2.4";
        char wifi_password[] = "MachDatum.1";
        size_t required_size;
        err = nvs_get_blob(my_handle, "nvs_struct", NULL, &required_size );
        err = nvs_get_blob(my_handle, "nvs_struct", (void *)&nvs_struct, &required_size);
        switch (err) {
            case ESP_OK:
                required_size = sizeof(nvs_struct);
                printf("Done\n\n");
                
                printf("ssid = %s\n\n", nvs_struct.ssid);
                printf("password = %s\n\n", nvs_struct.password);
                memset(nvs_struct.ssid, 0, sizeof(nvs_struct.ssid));
                memset(nvs_struct.password, 0, sizeof(nvs_struct.password));
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                required_size = sizeof(nvs_struct);
                memset(nvs_struct.ssid, 0, sizeof(nvs_struct.ssid));
               
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
       

        // Write
        printf("Adding text to NVS Struct... ");
        
        strncat(nvs_struct.ssid,(const char*)wifi_ssid,(const char*)wifi_ssid);
        strncat(nvs_struct.password,(const char*)wifi_password,(const char*)wifi_password);
       
        err = nvs_set_blob(my_handle, "nvs_struct", (const void*)&nvs_struct, required_size);
       
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        
        printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }

    
}