/*
 * MOTORS_WBM.h
 *
 *  Created on: Jun 20, 2019
 *      Author: pablo
 */

#ifndef WIRE_BENDING_INTERFAZ_INC_MOTORS_WBM_H_
#define WIRE_BENDING_INTERFAZ_INC_MOTORS_WBM_H_

#define N_INST_MAX  20

typedef enum
{
	PARAR   = 0,
	AVANZAR = 1,
	ROTAR   = 2,
	GIRAR   = 3
} mov_t;

typedef struct
{
	mov_t movimiento;
	double valor;
} instrucion_t;


static instrucion_t inst[ N_INST_MAX ];

static uint8_t nInst = 0;


void setupMOTORS_WBM( void );
void cleanInstrucciones( void );
void pushInstMOTORS_WBM( mov_t mov, double valor );
bool_t servicioDeMOTORES_WBM( void );
bool_t motorGirar( double parametro );
bool_t motorAvanzar( double parametro );
bool_t motorRotar( double parametro );


#endif /* EXAMPLES_C_SAPI_I2C_WIRE_BENDING_INTERFAZ_INC_MOTORS_WBM_H_ */
