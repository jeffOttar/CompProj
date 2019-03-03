#pragma once
#include "State.h"
#include <vector>


	class Warehouse :
		public GEX::State
	{
	public:
		class CommandQueue;
		Warehouse(GEX::StateStack& stack, Context context);

		void						draw() override;
		bool						update(sf::Time dt) override;
		bool						handleEvent(const sf::Event& event)override;

	private:

		sf::Sprite					_backgroundSprite;
		std::vector<sf::Text>		_options;
		std::size_t					_optionsIndex;
		double						_money;
	};



