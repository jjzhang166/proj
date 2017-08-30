#include "stdafx.h"
#include <AppBase/FrameEvents.h>


DEFINE_LOCAL_EVENT_TYPE(wxEVT_FE_IO_SERVICE)


FE_IoServiceEvent::FE_IoServiceEvent(wxWindowID id) :
	wxNotifyEvent(wxEVT_FE_IO_SERVICE, id)
{

}

FE_IoServiceEvent::FE_IoServiceEvent(const FE_IoServiceEvent& event) : wxNotifyEvent(event)
{

}

wxEvent * FE_IoServiceEvent::Clone() const
{
	return new FE_IoServiceEvent(*this);
}
