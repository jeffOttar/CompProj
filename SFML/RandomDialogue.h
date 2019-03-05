#pragma once
#include "State.h"
class RandomDialogue :
	public GEX::State
{
public:
	RandomDialogue(GEX::StateStack& stack, Context context);

	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvent(const sf::Event& event) override;
	void					updateText();

private:
	sf::Sprite					_backgroundSprite;
	std::vector<std::string>	_text;
	std::size_t					_index;
	sf::Text				_displayedText1;
	sf::Text				_displayedText2;
	sf::Text				_displayedText3;
	std::vector<sf::Text*>	_displayedTexts;
	bool _end;
	//have a map of villager type and text for dialogue
};

