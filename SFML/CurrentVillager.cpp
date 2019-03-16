#include "CurrentVillager.h"


namespace GEX {
	CurrentVillager* CurrentVillager::_instance = nullptr;

	CurrentVillager::CurrentVillager()
	{
		_instance = this;
	}


	CurrentVillager::~CurrentVillager()
	{
	}
}

