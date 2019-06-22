/*
 * MOTORS_WBM.c
 *
 *  Created on: Jun 06, 2019
 *      Author: Lorena B. Zacharewicz
 */

#include "../inc/LCD_I2C_PCF8574.h"
#include "../inc/KEYBRD_5C.h"
#include "../inc/MOTORS_WBM.h"
#include "sapi.h"               // <= sAPI header
#include "sapi_delay.h"         /* <= sAPI Delay header */


//#include <math.h>

void setupMOTORS_WBM()
{
	cleanInstrucciones();
}

void cleanInstrucciones()
{
	nInst = 0;

	for ( int i = 0; i < N_INST_MAX; i++ )
	{
		inst [ i ].movimiento = PARAR;
		inst [ i ].valor      = 0;
	}
}

void pushInstMOTORS_WBM(mov_t mov, double valor)
{
	inst [ nInst ].movimiento = mov;
	inst [ nInst ].valor      = valor;

	nInst++;
}

bool_t servicioDeMOTORES_WBM()
{
	printf ( "\nGrabando y Ejecutando WBM\n\n");

	for ( int i = 0; i <= nInst ; i++ )
		switch (inst[ i ].movimiento )
		{
		case PARAR:
			printf ( "Motores parados\n" );
			 //motorAvanzar( 0 );
			 //motorRotar  ( 0 );
			 //motorGirar  ( 0 );
			 break;

		case AVANZAR:
			 motorAvanzar( inst[ i ] .valor );
			 break;

		case ROTAR:
			 motorRotar( inst[ i ] .valor );
			 break;

		case GIRAR:
			 motorGirar( inst[ i ] .valor );
			 break;
		}
}


bool_t motorAvanzar(double parametro)
{
	printf ( "Avanzando: %2.2f\n", parametro );
}

bool_t motorRotar(double parametro)
{
	printf ( "Rotando: %2.2f\n", parametro );
}

bool_t motorGirar(double parametro)
{
	printf ( "Girando: %2.2f\n", parametro );
}

