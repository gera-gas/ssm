#ifndef SSM_H
#define SSM_H
/**
 *  @file     essm.h
 *  @brief    Standalone state machine implementation.
 *  @author   Gerasimov A.S.
 *  @date     2014-09-08 10:54:18
 */
#include <stddef.h>

/**
 * @enum   ssmflag_t
 * @brief  Flags of SSM object.
 */
typedef enum {
	SSM_FLAG_NONE     = 0x0,  /** SSM flags absent.                   */
	SSM_FLAG_STEPMODE = 0x1,  /** SSM doing only one function in step */
} ssmflag_t;

/**
 * @enum   ssmres_t
 * @brief  Return types of SSM manager process.
 */
typedef enum {
	SSM_STATE_ATWORK, /** SSM manager still processing of states. */
	SSM_STATE_FINISH  /** SSM manager finish (reset) processing.  */
} ssmres_t;

typedef size_t ( *ssmfunc_t )( const void *, void * );

/**
 * @struct  ssm_t
 * @brief   Structure with main contex of SSM object.
 */
typedef struct ssm_st {
         char *name;
    ssmflag_t  flags;
       size_t  numb;
    ssmfunc_t *init;
    ssmfunc_t *state;
} ssm_t;

/*
 * Possible values returned by state function.
 */
#define SSM_HOLD      (size_t)(0)                 /** Hold in current state.       */
#define SSM_NEXT      (size_t)(1)                 /** Go to next state of SSM.     */
#define SSM_BACK      (size_t)(-1)                /** Back to previous state.      */
#define SSM_JUMP( i ) (size_t)(i)                 /** Jump to any state.           */
#define SSM_RESET     (size_t)((size_t)(-1) / 2)  /** Complete process forcibly.   */

/**
 *  @def    SSM_STATE
 *  @brief  Create a function protoype for state handler.
 *  @param  name : function name.
 */
#define SSM_STATE( name )\
	size_t name ( const void *argin, void *argout )

/**
 *  @def    SSM_CREATE
 *  @brief  Create SSM object.
 *
 *  @param  name  : name of SSM object;
 *  @param  flags : see to ssmflag_t;
 *  @param  ...   : state handler functions.
 */
#define SSM_CREATE( name, flags, ... )\
	ssmfunc_t name##_states_[] = { __VA_ARGS__, NULL };\
	ssm_t name = { #name, flags, (sizeof(name##_states_)/sizeof(name##_states_[0])) - 1, &name##_states_[0], &name##_states_[0] };

/**
 * Starting SSM without arguments.
 */
#define SSM( ssm ) ssm_start( &ssm, NULL, NULL )

/**
 * Starting SSM and wait until they will be finished.
 */
#define SSM_WAIT( ... ) while( ssm_start( &__VA_ARGS__ ) != SSM_STATE_FINISH )

#ifdef __cplusplus
extern "C" {
#endif

ssmres_t ssm_start ( ssm_t *ssm, const void *argin, void *argout );

#ifdef __cplusplus
}
#endif

#endif /* SSM_H */
