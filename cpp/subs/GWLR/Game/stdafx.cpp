// stdafx.cpp : source file that includes just the standard includes
// Game.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


StaticTaskQueueT& GetStaticTaskQueue()
{
	static StaticTaskQueueT g_static_task_queue_;
	return g_static_task_queue_;
}