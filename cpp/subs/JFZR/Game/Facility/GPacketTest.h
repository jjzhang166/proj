#pragma once
/*
@author zhp
@date 2014/11/4 23:22
@purpose for test game packet
*/
#define _ENABLE_GAME_PACKET_TEST_ZHP_
#ifdef _DEBUG
#define _ENABLE_GAME_PACKET_TEST_
#endif

#ifdef _ENABLE_GAME_PACKET_TEST_
#include "../../CrackData/jfzrcalltest/JFZRPacket.h"
extern char g_send_packet_buffer_test_[1024 * 4];
#endif