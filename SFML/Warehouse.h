#pragma once
#include "State.h"
#include <vector>
#include "Item.h"
#include "TextureManager.h"
#include <map>
#include "Player.h"

	class Warehouse :
		public GEX::State
	{
	public:
		class CommandQueue;
		Warehouse(GEX::StateStack& stack, Context context);

		void						draw() override;
		bool						update(sf::Time dt) override;
		bool						handleEvent(const sf::Event& event)override;

		void						updateOptionText();

	private:

		sf::Sprite					_backgroundSprite;
		std::vector<sf::Text>		_options;
		std::vector<GEX::Item>		_items;
		std::size_t					_optionsIndex;
		double						_money;
		GEX::TextureManager*				_textures;
		int _itemCount;
		std::map<int,std::string> _optionString;
		std::map<int, std::string> _optionCount;
		GEX::Player*			_player;

	};



