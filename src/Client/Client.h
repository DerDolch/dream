/*
 *  Client.h
 *  Dream
 *
 *  Created by Samuel Williams on 14/08/10.
 *  Copyright 2010 Orion Transfer Ltd. All rights reserved.
 *
 */

#ifndef _DREAM_CLIENT_CLIENT_H
#define _DREAM_CLIENT_CLIENT_H

// http://stackoverflow.com/questions/6649936/c-compiling-on-windows-and-linux-ifdef-switch

#ifdef __APPLE__
#include <TargetConditionals.h>
#elif __linux__
#define TARGET_OS_LINUX 1
#elif _WIN32
#define TARGET_OS_WIN32 1
#endif

namespace Dream
{
	/** The Client namespace is functionality which is specific to actual graphical applications.
	
	This includes such functionality such as graphics, audio, user interface, and associated areas.	
	*/
	namespace Client
	{
	}
}

#endif
