#include <stdlib.h>

static SM_STATE(graph_example_state_0);
static SM_STATE(graph_example_state_1);
static SM_STATE(graph_example_state_2);

SM_CREATE(sm_graph_example, SM_FLAG_NONE, graph_example_state_0);

char *msg;

static SM_STATE( graph_example_state_0 )
{
	if(strlen(msg) == 0) {
		return SM_HOLD;
	}

	return SM_JUMP(graph_example_state_1); /* Receive message. */
}

static SM_STATE( graph_example_state_1 )
{
	if(strcmp(msg, "reset") == 0) {
		return SM_RESET; /* Receive 'reset' message. */
	}

	return SM_JUMP(graph_example_state_2); /* Go to message processing. */
}

static SM_STATE( graph_example_state_2 )
{
	/* Something do with 'msg' */

	return SM_RESET; /* Processing finish. */
}
