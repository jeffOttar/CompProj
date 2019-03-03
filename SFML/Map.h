#pragma once
#include "State.h"


class Map :
	public GEX::State
{
public:
	class CommandQueue;
	Map(GEX::StateStack& stack, Context context);

	void						draw() override;
	bool						update(sf::Time dt) override;
	bool						handleEvent(const sf::Event& event)override;

	void						updateOptionSelect();
private:
	enum LocationNames
	{
		Shop = 0,//when using an unscoped enum it uses numbers to identify such as 0 here
		Warehouse,
		Marco
	};

	sf::Sprite					_backgroundSprite;
	std::vector<sf::Sprite>		_options;
	std::size_t					_optionsIndex;
	double						_money;
};



