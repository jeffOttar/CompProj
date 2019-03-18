#include "SellingState.h"
#include "CurrentShelf.h"
#include "CurrentVillager.h"
#include "Utility.h"



SellingState::SellingState(GEX::StateStack & stack, Context context)
	: GEX::State(stack, context),
	_player(context.playerBody),
	_backgroundSprite(),
	_textures(context.textures),
	_total(0)
{
	_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::Warehouse));


	sf::Text option;
	option.setString("0");
	centerOrigin(option);
	option.setPosition(context.window->getView().getSize()/ 2.f);
	_amount.push_back(option);
}

void SellingState::draw()
{
	//TODO: DRAW STUFF
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(_backgroundSprite);
	//draw each of the text options
	for (const sf::Text& text : _amount)
	{
		window.draw(text);
	}
}

bool SellingState::update(sf::Time dt)
{
	return false;
}

bool SellingState::handleEvent(const sf::Event & event)
{
	//if the event is not a keypress
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}

	if (event.key.code == sf::Keyboard::Return)//if key press is return
	{
		//update displayed text
		//if (_sold)
		//{
		//	//update all neccessary stuff
		//	requestStackPop();
		//}
		////if not sold update face and display some text
		//updateText();
	}
	else if (event.key.code == sf::Keyboard::Right)
	{

		if (_total <= 99999)
		{
			_total++;
		}
		else//if it reaches the top then reset to the 0 indexed item
		{
			_total = 0;
		}

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Left)
	{

		if (_total >= 99999)
		{
			_total--;
		}
		else//if it reaches the top then reset to the 0 indexed item
		{
			_total = 0;
		}

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}

	return true;
}

void SellingState::updateOptionText()
{

	//update option text
	_amount[0].setString(std::to_string(_total));

}
