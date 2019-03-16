#pragma once
#include "Villager.h"

namespace GEX
{
	class CurrentVillager
	{
	public:
		CurrentVillager();
		~CurrentVillager();

		static CurrentVillager& getInstance() { return *_instance; }

		Villager* getCurrentVillager() { return _villager; }
		void setCurrentVillager(Villager* villager) { _villager = villager; }

	private:
		static CurrentVillager* _instance;
		Villager* _villager;
	};
}
