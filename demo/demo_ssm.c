/**
 * @file   main.c
 * @author Gerasimov A.S.
 * @brief  Example of SSM mechanism.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "../src/ssm.h"

/*
 * Example work of two SSM objects.
 */
clock_t timer_foo;
clock_t timer_bar;

/*
 ************************************************************
 * FOO SSM object.
 ************************************************************
 */
SSM_STATE( foo_state_0 )
{
	/* delay 1000 ms */
	timer_foo = 1000 + clock();

	/* goto foo_state_1 */
	return SSM_NEXT;
}

SSM_STATE( foo_state_1 )
{
	if(timer_foo > clock() ) {
		/* break next actions and stay in foo_state_1 */
		return SSM_HOLD;
	}

	printf( "FOO: working...\n");

	return SSM_NEXT;
}

SSM_CREATE( ssm_foo, SSM_FLAG_NONE,
	foo_state_0,
	foo_state_1 );

/*
 ************************************************************
 * BAR PT object.
 ************************************************************
 */
SSM_STATE(bar_state_0)
{
	/* delay 1000 ms */
	timer_bar = 3000 + clock();

	/* goto foo_state_1 */
	return SSM_NEXT;
}

SSM_STATE(bar_state_1)
{
	if (timer_bar > clock()) {
		/* break next actions and stay in foo_state_1 */
		return SSM_HOLD;
	}

	printf("BAR: working...\n");

	return SSM_NEXT;
}

SSM_CREATE( ssm_bar, SSM_FLAG_NONE,
	bar_state_0,
	bar_state_1 );

/*
 ************************************************************
 * main
 ************************************************************
 */
int main ( void )
{
	printf( "*** Example work of two SSM object ***\n");

	while( true ) {
		SSM( ssm_foo );
		SSM( ssm_bar );
	}

	return 0;
}
