#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
/**
 *  @file     state_machine.h
 *  @brief    Standalone state machine implementation.
 *  @author   Gerasimov A.S.
 *  @date     2014-09-08 10:54:18
 */
#include <stddef.h>
#include <stdbool.h>

/**
 * @enum  smstate_t
 * @brief Various state in SM processing (returned from sm_start).
 */
typedef enum {
	SM_STATE_IDLE,
	SM_STATE_PROCESSING,
	SM_STATE_COMPLETED
} smstate_t;

/**
 * @enum   smflag_t
 * @brief  Flags of state machine object.
 */
typedef enum {
	SM_FLAG_NONE     = 0x0,  /** SM flags absent.                   */
	SM_FLAG_STEPMODE = 0x1,  /** SM doing only one function in step */
} smflag_t;

typedef void* ( *smfunc_t )( const void *, void * );

typedef struct state_machine_st
{
	smflag_t flags;
	smfunc_t initial_state;
	smfunc_t current_state;
} state_machine_t;

/*
 * Possible values returned by state function.
 */
#define SM_JUMP(v)  ((void*)&(v))  /** Jump to other processing state by function name */
#define SM_HOLD     ((void*)(0))   /** Hold current processing state.                  */
#define SM_RESET    ((void*)(-1))  /** Finish processing and back to IDLE state.       */

/**
 *  @def    SM_STATE
 *  @brief  Create a function protoype for state handler.
 *  @param  name : function name.
 */
#define SM_STATE( name )\
	void* name ( const void *argin, void *argout )

/**
 *  @def    SM_CREATE
 *  @brief  Create SM object.
 *
 *  @param  name  : name of SSM object;
 *  @param  flags : see to ssmflag_t;
 *  @param  initial_state : first state of handler functions.
 */
#define SM_CREATE( name, flags, initial_state )\
	state_machine_t name = { flags, (smfunc_t)&initial_state, (smfunc_t)&sm_idle_state }

/*
 * State machine startup macros without arguments.
 */
#define SM_START( sm ) sm_start( &sm, NULL, NULL )
#define SM_FORCE_START( sm ) sm_force_start( &sm, NULL, NULL )

#ifdef __cplusplus
extern "C" {
#endif

SM_STATE( sm_idle_state );

smstate_t sm_start(state_machine_t *sm, const void *argin, void *argout);
smstate_t sm_force_start(state_machine_t *sm, const void *argin, void *argout);
void sm_wakeup( state_machine_t *sm );

#ifdef __cplusplus
}
#endif

#endif /* STATE_MACHINE_H */
