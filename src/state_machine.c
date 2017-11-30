/**
 *  @file     state_machine.c
 *  @brief    Standalone state machine implementation.
 *  @author   Gerasimov A.S.
 *  @date     2014-09-08 10:54:18
 */
#include "assert.h"
#include "state_machine.h"

SM_STATE( sm_idle_state )
{
	return SM_HOLD;
}

/**
 * @brief
 * Main manager of state machine.
 *
 * @param sm     : [in]  address of SM object.
 * @param argin  : [in]  memory address of input parameters for state handlers.
 * @param argout : [out] memory address of output parameters for state handlers.
 *
 * @return.
 */
static void sm_manager ( state_machine_t *sm, const void *argin, void *argout )
{
	void* result;

	do {
		result = sm->current_state(argin, argout);

		if(result == SM_HOLD) {
			;
		} else if (result == SM_RESET) {
			sm->current_state = sm->initial_state;
		} else {
			sm->current_state = result;
		}

	} while( (result != SM_HOLD && result != SM_RESET) && !(sm->flags & SM_FLAG_STEPMODE));
}

void sm_start(state_machine_t *sm, const void *argin, void *argout)
{
	sm_manager(sm, argin, argout);
}

void sm_force_start(state_machine_t *sm, const void *argin, void *argout)
{
	sm_wakeup(sm);
	sm_manager(sm, argin, argout);
}

void sm_wakeup ( state_machine_t *sm )
{
	if( sm->current_state == &sm_idle_state ) {
		sm->current_state = sm->initial_state;
	}
}

bool sm_is_processing ( state_machine_t *sm )
{
	return sm->current_state != &sm_idle_state;
}
