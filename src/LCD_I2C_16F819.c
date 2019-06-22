/* Copyright 2019, Lorena B. Zacharewicz.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Date: 2019-05-29 */

/*==================[inclusions]=============================================*/

#include "../inc/LCD_I2C_16F819.h"            /* <= LCD_I2C header   */
#include "../inc/LCD_I2C_PCF8574.h"
#include "sapi.h"               // <= sAPI header
#include "sapi_i2c.h"           /* <= sAPI I2C header */
#include "sapi_delay.h"         /* <= sAPI Delay header */


/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void ejecutar_comando_SLAVE_I2C(uint8_t slv_add, uint8_t slv_cmd, uint8_t X, uint8_t Y, uint8_t Z, uint8_t T){
	uint8_t transmitDataBuffer[5];
	transmitDataBuffer[0] = slv_cmd;
	transmitDataBuffer[1] = X;
	transmitDataBuffer[2] = Y;
	transmitDataBuffer[3] = Z;
	transmitDataBuffer[4] = T;

	i2cWrite(I2C0, slv_add, transmitDataBuffer, 5, TRUE);
	delay(5);
}

void LCD_I2C_CLEAR(uint8_t lcd_add){
	ejecutar_comando_SLAVE_I2C(lcd_add, 0x61, 0, 0, 0, 0);
};
void LCD_I2C_HOME(uint8_t lcd_add){
	ejecutar_comando_SLAVE_I2C(lcd_add, 0x62, 0, 0, 0, 0);
};
void LCD_I2C_SET_ADDRESS_1234(uint8_t lcd_add, uint8_t chr_add){
	ejecutar_comando_SLAVE_I2C(lcd_add, 0x63, chr_add, 0, 0, 0);
};
void LCD_I2C_WRITE_DATA_1234(uint8_t lcd_add, uint8_t chr_val){
	ejecutar_comando_SLAVE_I2C(lcd_add, 0x64, chr_val, 0, 0, 0);
};
void LCD_I2C_PRUEBA(uint8_t lcd_add){
	ejecutar_comando_SLAVE_I2C(lcd_add, 0x84, 0, 0, 0, 0);
};
void LCD_I2C_SETUP(uint8_t lcd_add){
	ejecutar_comando_SLAVE_I2C(lcd_add, 0x85, 0, 0, 0, 0);
};

void LCD_I2C_WRITE_DATA_1234_STRING( uint8_t lcd_add, uint8_t lcd_String[], uint8_t lcd_Str_lng ){
	for ( uint8_t i = 0; i < lcd_Str_lng; i++ ){
		ejecutar_comando_SLAVE_I2C(lcd_add, 0x64, lcd_String[i], 0, 0, 0);
	}
}


