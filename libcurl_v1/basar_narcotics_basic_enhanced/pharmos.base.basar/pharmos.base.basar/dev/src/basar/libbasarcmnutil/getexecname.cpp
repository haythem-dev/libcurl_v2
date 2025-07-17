#ifdef _AIX

#include "getexecname.h"

#include <cassert>
#include <string>
#include <procinfo.h>

const std::string getexecname()
{

	struct procentry64 processInfo;
	const  pid_t       thisPid = getpid();
	       pid_t       retPid  = pid_t();

	char argsBuffer[1024];
	memset (argsBuffer, 0, sizeof(argsBuffer));

    int ret = -1;

	while (getprocs64(&processInfo, sizeof (processInfo), 
                     0, 0, 
                     &retPid, 1) > 0)
	{
		if (static_cast<pid_t>(processInfo.pi_pid) == thisPid)
		{
            ret = getargs (&processInfo  , sizeof(processInfo), 
                           &argsBuffer[0], sizeof(argsBuffer ));
            break;
        }
	}

	const std::string exeName = (ret == 0 ) ? argsBuffer : "";
	assert (exeName.size() < sizeof(argsBuffer));

	return exeName;
}

#endif

