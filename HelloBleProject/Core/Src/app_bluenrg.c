/*
 * app_bluenrg.c
 *
 *  Created on: Jan 7, 2023
 *      Author: Possardt
 */
#include <stdio.h>

#include "bluenrg_conf.h"
#include "hci.h"
#include "bluenrg1_types.h"
#include "bluenrg1_gap.h"
#include "bluenrg1_aci.h"
#include "bluenrg1_hci_le.h"
#include "app_bluenrg.h"



#define BDADDR_SIZE 6

uint8_t SERVER_BDARR[]={0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

void bluenrg_init(void)
{

	tBleStatus ret;
	uint8_t bdaddr[BDADDR_SIZE];
	const char *name ="Trix";
	uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

	BLUENRG_memcpy(bdaddr, SERVER_BDARR, sizeof(SERVER_BDARR));

	/* Initialize HCI*/
	hci_init(NULL, NULL);
	printf("Initialization successful...\n\r");

	/* Reset HCI */
	hci_reset();

	/*Wait a bit*/
	HAL_Delay(100);

	/*Configure device address*/
	ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN,bdaddr);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("aci_hal_write_config_data : FAILED !! \n\r");
	}

	/* Initialize GATT server */
	ret = aci_gatt_init();
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("aci_gatt_init : FAILED !! \n\r");
	}

	/* Initialize GAP service */
	ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("aci_gap_init : FAILED !! \n\r");
	}

	/* Update device name characteristics */
	ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, strlen(name), (uint8_t *) name);
	if(ret != BLE_STATUS_SUCCESS)
	{
			printf("aci_gatt_update_char_value : FAILED !! \n\r");
	}

	/* Add custom service */

}

void bluenrg_process()
{
	tBleStatus ret;

	uint8_t local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME, 'B','L','E','-','B','O','S','C','H','-','R','E','U','T','L','I','N','G','E','N'};

	/*Set device in General Discoverable mode*/

	/*tBleStatus aci_gap_set_limited_discoverable(uint8_t Advertising_Type,
                                            uint16_t Advertising_Interval_Min,
                                            uint16_t Advertising_Interval_Max,
                                            uint8_t Own_Address_Type,
                                            uint8_t Advertising_Filter_Policy,
                                            uint8_t Local_Name_Length,
                                            uint8_t Local_Name[],
                                            uint8_t Service_Uuid_length,
                                            uint8_t Service_Uuid_List[],
                                            uint16_t Slave_Conn_Interval_Min,
                                            uint16_t Slave_Conn_Interval_Max);
	 * */

	ret = aci_gap_set_discoverable(ADV_IND,0,0,PUBLIC_ADDR, NO_WHITE_LIST_USE, sizeof(local_name), &local_name[0], 0, NULL,0,0);
	if(ret != BLE_STATUS_SUCCESS)
		{
			printf("aci_gap_set_discoverable : FAILED !! \n\r");
		}
}
