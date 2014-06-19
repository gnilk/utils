/*-------------------------------------------------------------------------
File    : $Archive: ioutils.h $
Author  : $Author: Fkling $
Version : $Revision: 1 $
Orginal : 2009-10-17, 15:50
Descr   : Defines the implementation classes.

Modified: $Date: $ by $Author: Fkling $
---------------------------------------------------------------------------
TODO: [ -:Not done, +:In progress, !:Completed]
<pre>
</pre>


\History
- 14.03.14, FKling, published on github
- 10.11.09, FKling, Refactored out of ySystem_internal.h

---------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <map>

namespace gnilk
{
	class URLParser
	{
	protected:
		char *service;
		char *path;

		bool DoParseURL(const char *url);
	public:
		URLParser(const char *url);
		virtual ~URLParser();

		static URLParser *ParseURL(const char *url);

		const char *GetService();
		const char *GetPath();

		char *GetServiceCopy(char *dst, int maxlen);
		char *GetPathCopy(char *dst, int maxlen);
	};

}
