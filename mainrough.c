/*
 *  GattLib - GATT Library
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "gattlib.h"
#include <wiringPi.h>

#define LedPin 0
#define LedPin2 2

#define NUS_CHARACTERISTIC_TX_UUID	"f3641401-00b0-4240-ba50-05ca45bf8abc"   //Temp UUID   1                       
#define NUS_CHARACTERISTIC_RX_UUID	"f3641402-00b0-4240-ba50-05ca45bf8abc"   //Humid UUID  1  

#define NUS_CHARACTERISTIC_TX_UUID2	"00001525-1212-efde-1523-785feabcd123"   //Temp UUID   2
#define NUS_CHARACTERISTIC_RX_UUID2	"00001524-1212-efde-1523-785feabcd123"   //Humid UUID  2




gatt_connection_t* m_connection;
gatt_connection_t* m_connection2;



 int count,count2;
 int hold1,hold2;
 unsigned status=1;
 unsigned status2=1;
 
 

void notification_cb(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data) {
	int i;
	for(i = 0; i < data_length; i++) {
		printf("%c", data[i]);
	}
}


void int_handler(int dummy) {
	gattlib_disconnect(m_connection);
	gattlib_disconnect(m_connection2);			
	exit(0);
}

int main(int argc, char *argv[]) {

//    int testInteger,testInteger2 ;
    int add,add2,add3,add4;
	int i, ret,ret2,ret3,ret4;
	size_t len;
	size_t len2,len3,len4;

	
	uuid_t nus_characteristic_tx_uuid;
	uuid_t nus_characteristic_rx_uuid;

	uuid_t nus_characteristic_tx_uuid2;
	uuid_t nus_characteristic_rx_uuid2;	

			

//C9:2C:CD:C5:11:EC
   while(status){
	m_connection = gattlib_connect(NULL, "C9:2C:CD:C5:11:EC",
				       GATTLIB_CONNECTION_OPTIONS_LEGACY_BDADDR_LE_RANDOM |
 				      GATTLIB_CONNECTION_OPTIONS_LEGACY_BT_SEC_LOW);
  				      				    	
	   hold1 = m_connection; 				    	
 	   if(hold1 < 1000){status = 1; printf("not connected \n"); }else{ printf("connected \n"); status=0; break;} 	   
 	   
      }				      
 				      
//------------------------------------------------------------------
// CB:D1:0D:53:FC:14
// F8:96:04:19:82:6A 

    while(status2){ 				      
	m_connection2 = gattlib_connect(NULL, "F8:96:04:19:82:6A",
				       GATTLIB_CONNECTION_OPTIONS_LEGACY_BDADDR_LE_RANDOM |
 				      GATTLIB_CONNECTION_OPTIONS_LEGACY_BT_SEC_LOW);
 				      
 	   hold2 = m_connection2; 				   	
 	   if(hold2 < 1000){status2 = 1; printf("not connected \n"); }else{ printf("connected \n"); status2=0; break;} 	   
 	   
      } 				      					      			
 				      					      			      
//------------------------------------------------------------------------------------				      	
 				      
	if (m_connection == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		return 1;
	}

	if (m_connection2 == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		return 1;
	}		

	// Convert characteristics to their respective UUIDs
	ret = gattlib_string_to_uuid(NUS_CHARACTERISTIC_TX_UUID, strlen(NUS_CHARACTERISTIC_TX_UUID) + 1, &nus_characteristic_tx_uuid);
	if (ret) {
		fprintf(stderr, "Fail to convert characteristic TX to UUID.\n");
		return 1;
	}
	ret = gattlib_string_to_uuid(NUS_CHARACTERISTIC_RX_UUID, strlen(NUS_CHARACTERISTIC_RX_UUID) + 1, &nus_characteristic_rx_uuid);
	if (ret) {
		fprintf(stderr, "Fail to convert characteristic RX to UUID.\n");
		return 1;
	}
	
//----------------------------------------------------------------------

 	ret2 = gattlib_string_to_uuid(NUS_CHARACTERISTIC_TX_UUID2, strlen(NUS_CHARACTERISTIC_TX_UUID2) + 1, &nus_characteristic_tx_uuid2);
	if (ret) {
		fprintf(stderr, "Fail to convert characteristic TX 2 to UUID.\n");
		return 1;
	}
	ret2 = gattlib_string_to_uuid(NUS_CHARACTERISTIC_RX_UUID2, strlen(NUS_CHARACTERISTIC_RX_UUID2) + 1, &nus_characteristic_rx_uuid2);
	if (ret) {
		fprintf(stderr, "Fail to convert characteristic RX 2 to UUID.\n");
		return 1;
	}

	
//----------------------------------------------------------------------

	// Look for handle for NUS_CHARACTERISTIC_TX_UUID
	gattlib_characteristic_t* characteristics;
	int characteristic_count;
	
//------

	gattlib_characteristic_t* characteristics2;
	int characteristic_count2;
	
//-----
	
	ret = gattlib_discover_char(m_connection, &characteristics, &characteristic_count);
	if (ret) {
		fprintf(stderr, "Fail to discover characteristic.\n");
		return 1;
	}

	uint16_t tx_handle = 0, rx_handle = 0;
	for (i = 0; i < characteristic_count; i++) {
		if (gattlib_uuid_cmp(&characteristics[i].uuid, &nus_characteristic_tx_uuid) == 0) {
			tx_handle = characteristics[i].value_handle;
		} else if (gattlib_uuid_cmp(&characteristics[i].uuid, &nus_characteristic_rx_uuid) == 0) {
			rx_handle = characteristics[i].value_handle;
		}
	}
	if (tx_handle == 0) {
		fprintf(stderr, "Fail to find NUS TX characteristic.\n");
		return 1;
	} else if (rx_handle == 0) {
		fprintf(stderr, "Fail to find NUS RX characteristic.\n");
		return 1;
	}
	free(characteristics);
	
	
//----------------------------------------------------------------------

	ret2 = gattlib_discover_char(m_connection2, &characteristics2, &characteristic_count2);
	if (ret) {
		fprintf(stderr, "Fail to discover characteristic2.\n");
		return 1;
	}

	uint16_t tx_handle2 = 0, rx_handle2 = 0;
	for (i = 0; i < characteristic_count2; i++) {
		if (gattlib_uuid_cmp(&characteristics2[i].uuid, &nus_characteristic_tx_uuid2) == 0) {
			tx_handle2 = characteristics2[i].value_handle;
		} else if (gattlib_uuid_cmp(&characteristics2[i].uuid, &nus_characteristic_rx_uuid2) == 0) {
			rx_handle2 = characteristics2[i].value_handle;
		}
	}
	if (tx_handle2 == 0) {
		fprintf(stderr, "Fail to find NUS TX characteristic2.\n");
		return 1;
	} else if (rx_handle2 == 0) {
		fprintf(stderr, "Fail to find NUS RX characteristic2.\n");
		return 1;
	}
	free(characteristics2);
	
//----------------------------------------------------------------------

     if(wiringPiSetup() == -1) 
        { 
           printf("setup wiringPi failed !\n");
            return -1;
        }	
	
	pinMode(LedPin, OUTPUT);
	pinMode(LedPin2, OUTPUT);	
	
//----------------------------------------------------------------------


// Register notification handler
	gattlib_register_notification(m_connection, notification_cb, NULL);
	gattlib_register_notification(m_connection2, notification_cb, NULL);	
	
	ret = gattlib_notification_start(m_connection, &nus_characteristic_rx_uuid);
	if (ret) {
		fprintf(stderr, "Fail to start notification.\n");
		return 2;
	}
	
//--------------
	
	ret2 = gattlib_notification_start(m_connection2, &nus_characteristic_rx_uuid2);
	if (ret) {
		fprintf(stderr, "Fail to start notification.\n");
		return 2;
	}

//--------------
			

	// Register handler to catch Ctrl+C
	signal(SIGINT, int_handler);

//----------------------------------------------------------------------           

        

//----------------------------------------------------------------------
        printf("\n");
	printf("PRESS & HOLD THE NRF BUTTON 1 TO SEND THE SIGNAL - For 5 sec \n");
     
	    
		uint8_t *buffer = NULL;
		uint8_t *buffer2 = NULL;	
		uint8_t *buffer3 = NULL;
		uint8_t *buffer4 = NULL;	
		

while(1) 
{

		ret = gattlib_read_char_by_uuid(m_connection, &nus_characteristic_rx_uuid, (void **)&buffer, &len);
		ret2 = gattlib_read_char_by_uuid(m_connection2, &nus_characteristic_rx_uuid2, (void **)&buffer2, &len2);
		ret3 = gattlib_read_char_by_uuid(m_connection2, &nus_characteristic_tx_uuid2, (void **)&buffer3, &len3);		
		ret4 = gattlib_read_char_by_uuid(m_connection2, &nus_characteristic_tx_uuid2, (void **)&buffer4, &len4);		

				
//----------		
		if (ret != GATTLIB_SUCCESS) {
			char uuid_str[MAX_LEN_UUID_STR + 1];

		gattlib_uuid_to_string(&nus_characteristic_rx_uuid, uuid_str, sizeof(uuid_str));

			if (ret == GATTLIB_NOT_FOUND) {
				fprintf(stderr, "Could not find GATT Characteristic with UUID . "
					"You might call the program with '--gatt-discovery'.\n");
			} else {
				fprintf(stderr, "Error while reading GATT Characteristic with UUID (ret:)\n");
			}
			
		}
//-------------------------------------------		
		if (ret2 != GATTLIB_SUCCESS) {
			char uuid_str2[MAX_LEN_UUID_STR + 1];

		gattlib_uuid_to_string(&nus_characteristic_rx_uuid2, uuid_str2, sizeof(uuid_str2));

			if (ret2 == GATTLIB_NOT_FOUND) {
				fprintf(stderr, "Could not find GATT Characteristic with UUID . "
					"You might call the program with '--gatt-discovery'.\n");
			} else {
				fprintf(stderr, "Error while reading GATT Characteristic with UUID (ret:)\n");
			}
			
		}
		
//-------------------------------------------		
		if (ret3 != GATTLIB_SUCCESS) {
			char uuid_str2[MAX_LEN_UUID_STR + 1];

		gattlib_uuid_to_string(&nus_characteristic_tx_uuid2, uuid_str2, sizeof(uuid_str2));

			if (ret2 == GATTLIB_NOT_FOUND) {
				fprintf(stderr, "Could not find GATT Characteristic with UUID . "
					"You might call the program with '--gatt-discovery'.\n");
			} else {
				fprintf(stderr, "Error while reading GATT Characteristic with UUID (ret:)\n");
			}
			
		}

//-------------------------------------------		
		if (ret4 != GATTLIB_SUCCESS) {
			char uuid_str2[MAX_LEN_UUID_STR + 1];

		gattlib_uuid_to_string(&nus_characteristic_tx_uuid2, uuid_str2, sizeof(uuid_str2));

			if (ret2 == GATTLIB_NOT_FOUND) {
				fprintf(stderr, "Could not find GATT Characteristic with UUID . "
					"You might call the program with '--gatt-discovery'.\n");
			} else {
				fprintf(stderr, "Error while reading GATT Characteristic with UUID (ret:)\n");
			}
			
		}		
//-------------------------------------------		
//----------------------------------------------------------------------		
	add = 0;
	add2 = 0;			
//----------------------------------------------------------------------		
		
		printf("\n");				

		printf("Read UUID completed 1: ");
		for (i = 0; i < len; i++) {
			printf("%02x ", buffer[i]);

		       count = (buffer[i] +1);
			add = 1;
			
		}
		printf("\n");
		
//----------------------------------------------------------------------		
add3 = 0;
add4 = 0;
//----------------------------------------------------------------------
		
		printf("Read UUID completed 2: ");
		for (i = 0; i < len2; i++) {
			printf("%02x ", buffer2[i]);

		       count2 = (buffer2[i] +1);
		       add3 = 1;
			
		}
		printf("\n");	


//----------------------------------------------------------------------

//----------------------------------------------------------------------
		
		printf("Read UUID completed 3: ");
		for (i = 0; i < len3; i++) {
			printf("%02x ", buffer3[i]);

		       count2 = (buffer3[i] +1);
		       add3 = 1;
			
		}
		printf("\n");	


//----------------------------------------------------------------------
//----------------------------------------------------------------------
		
		printf("Read UUID completed 4: ");
		for (i = 0; i < len4; i++) {
			printf("%02x ", buffer4[i]);

		       count2 = (buffer4[i] +1);
		       add3 = 1;
			
		}
		printf("\n");	


//----------------------------------------------------------------------
		
	//	if(count == 1){ digitalWrite(LedPin, HIGH);  printf("led1 off- NO Signal Received\n");  }   //led off		
	//	if(count == 2){ digitalWrite(LedPin, LOW);   printf("led1 on-  Signal Received\n"); }    //led on
	
		
//----------------------------------------------------------------------
		
	//	if(count2 == 1){ digitalWrite(LedPin2, HIGH);  printf("led2 off- NO Signal Received\n");  }   //led off	
	//	if(count2 == 2){ digitalWrite(LedPin2, LOW);   printf("led2 on-  Signal Received\n"); }    //led on
		
//----------------------------------------------------------------------

	while(1){ 
		
	if(add  == add2){
	status=1;	
	printf(" nrf board 1 C9:2C:CD:C5:11:EC is disconnected or off \n");
	printf(" searching for C9:2C:CD:C5:11:EC again \n");
	
   while(status){	 
	m_connection = gattlib_connect(NULL, "C9:2C:CD:C5:11:EC",
				       GATTLIB_CONNECTION_OPTIONS_LEGACY_BDADDR_LE_RANDOM |
 				      GATTLIB_CONNECTION_OPTIONS_LEGACY_BT_SEC_LOW);
 				      printf("connection established\n");
  				      				    	
	   hold1 = m_connection; 				    	
 	   if(hold1 < 1000){status = 1; printf("not connected \n"); }else{ printf("connected \n"); status=0; break;} 	   
 	   
      } 				      
 				      
 				        
		}
			
		
	
	if(add3  == add4){	
	status2=1;	
	printf(" nrf board 2 F8:96:04:19:82:6A is disconnected or off \n");
	printf(" searching for F8:96:04:19:82:6A again \n");
	
    while(status2){ 	 		
	m_connection2 = gattlib_connect(NULL, "F8:96:04:19:82:6A",
				       GATTLIB_CONNECTION_OPTIONS_LEGACY_BDADDR_LE_RANDOM |
 				      GATTLIB_CONNECTION_OPTIONS_LEGACY_BT_SEC_LOW);
 				      printf("connection established\n");
 		
 	   hold2 = m_connection2; 				   	
 	   if(hold2 < 1000){status2 = 1; printf("not connected \n"); }else{ printf("connected \n"); status2=0; break;} 	   
 	   
      }				      
 		}		
 		
				      
 		 				      
	 if (! ret2 ){
		         
		   break;
	            }		 
	     	
 			}	      
 				      
 				         
//	    }       
 
//----------------------------------------------------------------------                         
}  
            
            
                        
            
            
	return 0;
}
