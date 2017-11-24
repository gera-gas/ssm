/**
 *  @file     ssm.c
 *  @brief    Standalone state machine implementation.
 *  @author   Gerasimov A.S.
 *  @date     2014-09-08 10:54:18
 */
#include "assert.h"
#include "ssm.h"


/**
 * @brief
 * Manager of SSM (Executing and controling SSM objects).
 *
 * @param ssm    : [in]  address of SSM object.
 * @param argin  : [in]  memory address of input parameters for state handlers.
 * @param argout : [out] memory address of output parameters for state handlers.
 *
 * @retval SSM_STATE_ATWORK : SSM is not finish.
 * @retval SSM_STATE_FINISH : SSM finish (all states is walked or reset).
 */
ssmres_t ssm_start ( ssm_t *ssm, const void *argin, void *argout )
{
	  register size_t index = 0;
	register ssmres_t result = SSM_STATE_ATWORK;
	

	assert( ssm->numb < SSM_RESET );
	/*
	 * Main cycle of state handling.
	 */
	do {
		index = (*ssm->state)( argin, argout );
		/*
		 * SSM of forcibly completed.
		 */
		if( index == SSM_RESET )
		{
			goto ssm_finish;
		}
		
		/*
		 * Assertion control.
		 * Check to next state index on out of range.
		 */
		assert( ((size_t)(ssm->state - ssm->init) + index) <= ssm->numb );
		
		ssm->state += index;
		/*
		 * Walk all states in functions array.
		 * SSM in success finished.
		 */
		if( *ssm->state == 0 )
		{
		ssm_finish:
			ssm->state = ssm->init;
			result = SSM_STATE_FINISH;
			break;
		}
	} while( index && !(ssm->flags & SSM_FLAG_STEPMODE) );

	return  result;
}
