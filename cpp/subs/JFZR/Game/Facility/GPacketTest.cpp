#include "stdafx.h"
#include "GPacketTest.h"
#include "../../CrackData/jfzrcalltest/public/JFZRPacket.cpp"
#include "../../CrackData/jfzrcalltest/public/PacketString.cpp"

#ifdef _ENABLE_GAME_PACKET_TEST_
char g_send_packet_buffer_test_[1024 * 4];
#endif