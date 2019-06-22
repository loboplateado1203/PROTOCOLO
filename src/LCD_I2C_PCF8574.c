//
// \file LCD_I2C_PCF8574.c
// \LCD library with PCF8574 I2C adapter.
//
// \Refurbished para usarla en c con la CIAA
//    c++      ---> c
//    wire.lib ---> No usa wire.lib
//    Arduino  ---> CIAA
//
// \Author: Lorena B. Zacharewicz
// \Date  : 01/Junio/2019
//
// \Basado en la siguiente librería:

// \file LiquidCrystal_PCF8574.cpp
// \brief LiquidCrystal (LCD) library with PCF8574 I2C adapter.
//
// \Original author Matthias Hertel, http://www.mathertel.de
// \copyright Copyright (c) 2014 by Matthias Hertel.\n
// This work is licensed under a BSD style license.\n
// See http://www.mathertel.de/License.aspx
//
// \details
// This is a library for driving LiquidCrystal displays (LCD) by using the I2C bus and an PCF8574 I2C adapter.
// This library is derived from the original Arduino LiquidCrystal library and uses the original Wire library for communication.
//
// More documentation and source code is available at http://www.mathertel.de/Arduino
//
// ChangeLog see: LiquidCrystal_PCF8574.h

// Definitions on how the PCF8574 is connected to the LCD

#include "../inc/LCD_I2C_PCF8574.h"
#include "sapi.h"               /* <= sAPI header       */
#include "sapi_i2c.h"           /* <= sAPI I2C header   */
#include "sapi_delay.h"         /* <= sAPI Delay header */

// These are Bit-Masks for the special signals and background light
#define PCF_RS  0x01
#define PCF_RW  0x02
#define PCF_EN  0x04
#define PCF_BACKLIGHT 0x08

// Definitions on how the PCF8574 is connected to the LCD
// These are Bit-Masks for the special signals and Background
#define RSMODE_CMD  0
#define RSMODE_DATA 1

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

// modification:
// don't use ports from Arduino, but use ports from Wire

// a nibble is a half Byte

// NEW: http://playground.arduino.cc//Code/LCDAPI
// NEW: setBacklight

// El ex-creador de la clase . . . .
/*
void setup_LCD_I2C_PCF8574(uint8_t addr)
{
  _Addr = addr;
  _backlight = 0;
} // LiquidCrystal_PCF8574
*/
/*============================================================================================*/

void setup_LCD_I2C_PCF8574(uint8_t addr, uint8_t lines, uint8_t cols, uint8_t dotsize) {

	_Addr = addr;
	_backlight = 0;

	_numLines = lines;
	_numCols = cols;   //USADO POR LAS FUNCIONES EXTRAS

	_displayfunction = 0;

	if (lines > 1) {
	_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) {
	_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befor 4.5V so we'll wait 50
	//Wire.begin();

	// initializing the display
	_write2Wire(0x00, LOW, FALSE);
	//delayMicroseconds(50000);
	delay(50); //en mSeg

	// put the LCD into 4 bit mode according to the
	// hitachi HD44780 datasheet figure 26, pg 47
	_sendNibble(0x03, RSMODE_CMD);
	//delayMicroseconds(4500);
	delay(5);
	_sendNibble(0x03, RSMODE_CMD);
	//delayMicroseconds(4500);
	delay(5);
	_sendNibble(0x03, RSMODE_CMD);
	//delayMicroseconds(150);
	delay(5);
	// finally, set to 4-bit interface
	_sendNibble(0x02, RSMODE_CMD);

	// finally, set # lines, font size, etc.
	_command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	_command(LCD_ENTRYMODESET | _displaymode);

	cursor(); //Enciendo el cursor. . .

	caratula_LCD();
}

/*============================================================================================*/


void caratula_LCD(void){
	//PREPARADO PARA LA WBM LBZ
	clearLinea( 0 );
	setCursor( 1, 0 );
	print_LCD2( "Wire Bending Machine;");//, 20);
	setCursor( 2, 0 );
	print_LCD2( " Zacharewicz Tools;");//, 18);
	clearLinea( 3 );
	//setCursor( 3, 0 );
}

/************************* high level commands, for the user! **********************************/
void clear()
{
  _command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  //delayMicroseconds(2000);  // this command takes a long time!
  delay(2);

  for( int i = 0; i < _numCols; i++){
	  for( int J = 0; J < _numLines; J++){
  		  LCD_Texto[J][i] = ' ';
	  }
  }
}

/*============================================================================================*/

void home()
{
  _command(LCD_RETURNHOME);  // set cursor position to zero
  //delayMicroseconds(2000);  // this command takes a long time!
  delay(2);
  posc.linea = 0;
  posc.columna = 0;
}


// *****************************Turn the display on/off (quickly)
void noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void display() {
  _displaycontrol |= LCD_DISPLAYON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void cursor() {
  _displaycontrol |= LCD_CURSORON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void blink() {
  _displaycontrol |= LCD_BLINKON;
  _command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
  _command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void) {
  _command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  _command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  _command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  _command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  _command(LCD_ENTRYMODESET | _displaymode);
}

/*============================================================================================*/

/// Setting the brightness of the background display light.
/// The backlight can be switched on and off.
/// The current brightness is stored in the private _backlight variable 
/// to have it available for further data transfers.
void setBacklight(uint8_t brightness) {
  _backlight = brightness;
  // send no data but set the background-pin right;
  _write2Wire(0x00, RSMODE_DATA, FALSE);
} // setBacklight


//-------------Luego implemento esta funcionalidad
// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  _command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*============================================================================================*/

//Se la llama para incrementar el cursor y automáticamente setear la linea correcta
void incrementarPosc(){
	posc.columna++;
	if ( posc.columna >= _numCols ){
		posc.columna = 0;
		posc.linea++;
		if ( posc.linea >= _numLines ){
			posc.linea = 0;
		}
	}
}

/*============================================================================================*/

// Coloca el cursor en una nueva posicion
void setCursor(uint8_t linea, uint8_t columna)
{
  //Corrijo todo:
	uint8_t col; uint8_t row;
	if ( columna >= _numCols ) columna = _numCols - 1;
	if ( columna < 0  ) columna = 0;
	if ( linea >= _numLines ) linea = _numLines - 1;
	if ( linea < 0 ) linea = 0;

    posc.linea = linea;
	posc.columna = columna;

  //Ahora sigue el código original

  int linea_offsets[] = { 0x00, 0x40, 0x14, 0x54 }; //{ 0, 40, 20, 60}

  _command(LCD_SETDDRAMADDR | (columna + linea_offsets[linea]));
}

/*============================================================================================*/


//write retornaba 1 pero no tiene sentido porque no se interroga la LCD
//corrige automaticamente los saltos de lineas
void write( uint8_t value ) {
	LCD_Texto[ posc.linea ][ posc.columna ] = value; //Guardo en el LCD texto

    _send(value, RSMODE_DATA); //envío al LCD, esto incrementa la posición del cursor pero MAL porque salta del renglón 1 al 3 y del 2 al 4

    incrementarPosc();         //Incremento BIEN la Posc
    if ( posc.columna == 0 ) setCursor(posc.linea, posc.columna); //solo si se paso de renglón seteo el cursor
}

//El reemplazo al viejo print
void print_LCD( uint8_t * texto, uint8_t largo ){
	for ( uint8_t i = 0; i < largo; i++ ) {
		write( texto[i] );
	}
}

//El reemplazo al viejo print pero corta cuando encuentra un ;
void print_LCD2( uint8_t * texto ){
	for ( uint8_t i = 0; i < 81; i++ ) {
		if ( texto[i] == ';' ) break;
		write( texto[i] );
	}
}

//borra la linea indicada
void clearLinea( uint8_t linea ){
	setCursor(linea, 0 );
	for( int i = 0; i < _numCols; i++){
		write( ' ' );
		LCD_Texto[ linea ][i] = ' ';
	}
	setCursor(linea, 0 ); //vuelvo a dejar al cursor al principio de la linea
}

//Escribe sobre la linea indicada
void writeLinea( uint8_t linea, uint8_t * texto, uint8_t largo ){
	clearLinea( linea );
	if ( largo > 20 ) largo= 20;		//solo 20 de máximo
	for( int i = 0; i < largo; i++ ){
		write( texto[ i ] );
		LCD_Texto[ linea - 1 ][ i ] = texto[i];
	}
}

//Elevo todas las lineas un renglón y borro la última
void pullUpLineas(){
	for( int i = 0; i < _numLines-1; i++ ){   //copio los renglones hacia arriba
		for( int j = 0; j < _numCols; j++ ){
			LCD_Texto[ i ][ j ] = LCD_Texto[ i+1 ][ j ];
		}
	}
	setCursor( 0, 0 );
	for( int i = 0; i < _numLines-1; i++ ){   //vuelvo a escribir los renglones
		for( int j = 0; j < _numCols; j++ ){
			write( LCD_Texto[ i ][ j ] );
		}
	}

	clearLinea( _numLines - 1 );              //borro la última linea
}

/* ----- low level functions ----- */

inline void _command(uint8_t value) {
  _send(value, RSMODE_CMD);
} // _command()


// write either command or data
void _send(uint8_t value, uint8_t mode) {
  // separate the 4 value-nibbles
  uint8_t valueLo = value    & 0x0F;
  uint8_t valueHi = value>>4 & 0x0F;

  _sendNibble(valueHi, mode);
  _sendNibble(valueLo, mode);
} // _send()


// write a nibble / halfByte with handshake
void _sendNibble(uint8_t halfByte, uint8_t mode) {
  _write2Wire(halfByte, mode, TRUE);
  //delayMicroseconds(1);    // enable pulse must be >450ns
  delay(1);
  _write2Wire(halfByte, mode, FALSE);
  //delayMicroseconds(37);   // commands need > 37us to settle
  delay(1);
} // _sendNibble


// private function to change the PCF8674 pins to the given value
void _write2Wire(uint8_t halfByte, uint8_t mode, uint8_t enable) {
  // map the given values to the hardware of the I2C schema
  uint8_t i2cData = halfByte << 4;
  if (mode > 0) i2cData |= PCF_RS;
  // PCF_RW is never used.
  if (enable > 0) i2cData |= PCF_EN;
  if (_backlight > 0) i2cData |= PCF_BACKLIGHT;


  i2cWrite(I2C0, _Addr, &i2cData, 1, TRUE);

} // write2Wire


// The End.
