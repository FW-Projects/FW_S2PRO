
#include "beep_handle.h"
#include "FWS2pro_handle.h"

void beep_handle(void)
{
	if (sFWS2_t.general_parameter.speak_state == SPEAKER_OPEN)
		BeepProc(&sbeep);
	else
		sbeep.off();
}
