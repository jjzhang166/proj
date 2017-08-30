#include "stdafx.h"
#include "CfgStructC.h"
#include <strstream>
#include <wx/wxprec.h>

stPlayersData::PlayerData::PlayerData()
{
	spid_ = 0;
	player_ability_ = 0;
}

bool stPlayersData::Load()
{
	auto players_data = LoadRcdataRes(wxGetInstance(), _T("players_data"));
	if ( !players_data.first || players_data.second <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "stPlayersData load error";
		return false;
	}
	
	//XmlCfg::Restore(*this, "F:\\proj\\FIFA\\Game\\res\\players_data.xml");
	//return true;

	std::istrstream ost((char*)players_data.first, players_data.second);
	XmlCfg::Restore(*this, ost);
	return true;
}

stPlayersData::stPlayersData()
{
}

stPlayersData stPlayersData::singleton_players_data;
