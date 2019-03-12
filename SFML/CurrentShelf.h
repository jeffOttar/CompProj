
#include "Shelf.h"

#pragma once
namespace GEX
{


	class CurrentShelf
	{
	public:
		CurrentShelf();
		~CurrentShelf();

		static CurrentShelf& getInstance() { return *_instance; }

		Shelf* getCurrentShelf() { return _shelf; }
		void setCurrentShelf(Shelf* shelf) { _shelf = shelf; }

	private:
		static CurrentShelf* _instance;
		Shelf* _shelf;//this may need to be an object not a ptr
	};

}