#pragma once
/*
@author zhp
@date 2016/10/29 15:09
@purpose wxWidgets custom events
FE : Frame Event
*/
#include <wx/event.h>


BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_CORE, wxEVT_FE_IO_SERVICE, 0)
END_DECLARE_EVENT_TYPES()

class FE_IoServiceEvent : public wxNotifyEvent{
public:
	FE_IoServiceEvent(wxWindowID id);
	FE_IoServiceEvent(const FE_IoServiceEvent& event);
	wxEvent* Clone() const override;
};


typedef void (wxEvtHandler::*wxFE_IoServiceEventFunction)(FE_IoServiceEvent&);

#define EVT_FE_IO_SERVICE(id, fn) wx__DECLARE_EVT1(wxEVT_FE_IO_SERVICE, id, (wxObjectEventFunction)wxStaticCastEvent(wxFE_IoServiceEventFunction, &fn))

