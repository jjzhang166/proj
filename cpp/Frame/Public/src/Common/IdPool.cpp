#include "IdPool.h"
#include <vector>
#include <algorithm>

int IdDistributeByOrder::GetFreeId()
{
	std::vector<int> ids;
	for ( id_iter_->First(); !id_iter_->IsDone(); id_iter_->Next() )
	{
		ids.push_back(id_iter_->CurItem());
	}

	std::sort(ids.begin(), ids.end());
	int free_id = 0;
	for ( auto cur_id : ids )
	{
		if ( free_id < cur_id )
			break;
		free_id = cur_id + 1;
	}
	return free_id;
}

IdDistributeByOrder::IdDistributeByOrder( IdIter& id_iter )
{
	id_iter_ = &id_iter;
}

