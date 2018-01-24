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
 * @retval SM_STATE_IDLE : State machine is in a IDLE state (sm_idle_state).
 * @retval SM_STATE_PROCESSING : State machine is in a not IDLE state.
 * @retval SM_STATE_COMPLETED : State machine walk all states and back to IDLE state.
 */
static smstate_t sm_manager ( state_machine_t *sm, const void *argin, void *argout )
{
	void* state;
	smstate_t result = SM_STATE_PROCESSING;

	do {
		state = sm->current_state(argin, argout);

		if(state == SM_HOLD) {
			if(state == &sm_idle_state) {
				result = SM_STATE_IDLE;
			}
		} else if (state == SM_RESET) {
			sm->current_state = &sm_idle_state;
			result = SM_STATE_COMPLETED;
		} else {
			sm->current_state = state;
		}

	} while( (state != SM_HOLD && state != SM_RESET) && !(sm->flags & SM_FLAG_STEPMODE));

	return result;
}

smstate_t sm_start(state_machine_t *sm, const void *argin, void *argout)
{
	return sm_manager(sm, argin, argout);
}

smstate_t sm_force_start(state_machine_t *sm, const void *argin, void *argout)
{
	sm_wakeup(sm);
	return sm_manager(sm, argin, argout);
}

void sm_wakeup ( state_machine_t *sm )
{
	if( sm->current_state == &sm_idle_state ) {
		sm->current_state = sm->initial_state;
	}
}
