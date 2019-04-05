#pragma once
#include "State.h"
class SellingState :
	public GEX::State
{
public:
	SellingState(GEX::StateStack& stack, Context context);

	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvent(const sf::Event& event) override;

	void						updateOptionText();

private:
	GEX::Player*				_player;
	sf::Sprite					_backgroundSprite;
	GEX::TextureManager*		_textures;
	std::vector<sf::Text>		_amount;
	int _total;
	bool _sold;
	int _incrementer;
};

