/**
 * @file gizwits.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "gizwits.h"
#include "gizwits_product.h"
#include "stdio.h"


void InitGizwits(void){

    timerInit();
	uartInit();

	userInit();
	gizwitsInit();
	//keyInit();
	GIZWITS_LOG("Gizwits initialization Success \n");
}
