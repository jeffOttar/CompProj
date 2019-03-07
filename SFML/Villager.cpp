#include "Villager.h"

namespace GEX {
	namespace
	{
		//const is all uppercase and this is for all the aircraft data (it had to be initialized when declared because its const
		const std::map<AircraftType, AircraftData> TABLE = initializeAircraftData();
	}

	Villager::Villager()
	{
	}


	Villager::~Villager()
	{
	}
}