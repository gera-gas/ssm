/**
 * @file   sm_demo_c.c
 * @author Gerasimov A.S.
 * @brief  Example of state machine mechanism.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "../src/state_machine.h"

/*
 * Example work of two state machine objects.
 */
clock_t timer_foo;
clock_t timer_bar;

/*
 ************************************************************
 * FOO state machine object.
 ************************************************************
 */
static SM_STATE(foo_state_0);
static SM_STATE(foo_state_1);

SM_CREATE(sm_foo, SM_FLAG_NONE, foo_state_0);

static SM_STATE( foo_state_0 )
{
	/* delay 1000 ms */
	timer_foo = 1000 + clock();

	/* goto foo_state_1 */
	return SM_JUMP(foo_state_1);
}

static SM_STATE( foo_state_1 )
{
	if(timer_foo > clock() ) {
		/* break next actions and stay in foo_state_1 */
		return SM_HOLD;
	}

	printf( "FOO: working...\n");

	return SM_RESET;
}

/*
 ************************************************************
 * BAR state machine object.
 ************************************************************
 */
static SM_STATE(bar_state_0);
static SM_STATE(bar_state_1);

SM_CREATE(sm_bar, SM_FLAG_NONE, bar_state_0);

static SM_STATE(bar_state_0)
{
	/* delay 1000 ms */
	timer_bar = 3000 + clock();

	/* goto foo_state_1 */
	return SM_JUMP(bar_state_1);
}

static SM_STATE(bar_state_1)
{
	if (timer_bar > clock()) {
		/* break next actions and stay in foo_state_1 */
		return SM_HOLD;
	}

	printf("BAR: working...\n");

	return SM_RESET;
}

/*
 ************************************************************
 * main
 ************************************************************
 */
int main ( void )
{
	printf( "*** Example work of two state machine objects ***\n");

	while( true ) {
		SM_FORCE_START( sm_foo );
		SM_FORCE_START( sm_bar );
	}

	return 0;
}
