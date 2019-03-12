#include "CurrentShelf.h"


namespace GEX {
	CurrentShelf* CurrentShelf::_instance = nullptr;

	CurrentShelf::CurrentShelf()
	{
		_instance = this;
	}


	CurrentShelf::~CurrentShelf()
	{
	}
}

