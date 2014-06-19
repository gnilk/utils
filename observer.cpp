/*-------------------------------------------------------------------------
 File    : $Archive: $
 Author  : $Author: $
 Version : $Revision: $
 Orginal : 
 Descr   : Simple implemenation of the Observer pattern
 
 
 Modified: $Date: $ by $Author: $
 ---------------------------------------------------------------------------
 TODO: [ -:Not done, +:In progress, !:Completed]
 <pre>
 	- add ability to remove observers
 </pre>
 
 
 \History
 - 23.03.2014, FKling, Implementation 
 
 ---------------------------------------------------------------------------*/
#include "observer.h"

using namespace gnilk;

// TODO: Should add thread saftey to this one..

void Observable::AddObserver(IObserver *pObserver)
{
	// Lock observer list
	this->observers.push_back(pObserver);
	// unlock again
}
void Observable::NotifyObservers(void *pObject, int event)
{
	// Lock observer list
	for(int i=0;i<observers.size();i++) {
		observers[i]->OnEvent(pObject, event);
	}	
	// unlock list
}
