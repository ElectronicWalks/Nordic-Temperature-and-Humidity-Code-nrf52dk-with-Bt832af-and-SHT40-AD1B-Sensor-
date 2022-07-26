# Nordic-Temperature-and-Humidity-Code-nrf52dk-with-Bt832af-and-SHT40-AD1B-Sensor-
Nordic nRF52 Development Kit – Measuring Temperature and Humidity using Bluetooth Low Energy

SOFTWARE NEEDED:
 Segger embedded studio 5.40b
 SHT40-AD1B-Sensor (temp and humid sensor) Read datasheet online for connection
 

1. Go to this link:- https://drive.google.com/drive/folders/1zoDut_T_UO9TXcxyypc-_A5CZhxZC8g8?usp=sharing
2. Download the Zip File
3. Unzip and Go to path : nRF5_SDK_17.0.2_BLE_template_success_temp_humid\nRF5_SDK_17.0.2_d674dde\examples\ble_peripheral\ble_app_template\pca10040e\s112\ses
4. Open :- ble_app_template_pca10040e_s112 (using segger and compile)

Detials:
   . I have use Ble custom (https://github.com/maidenone/custom_ble_service_example.git)
   to add service and characteristics in ble_app_template
   . After I Uploaded SHT4X Driver file to folders
   . Done Some Code edit to create Temp UUID & & Humid UUID
   . Connected the Temp & Humid sensor to scl and sda pin (read sensor datasheet online)
   
   
   
   *********************** THANK TOU *******************************
   
