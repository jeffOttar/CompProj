#include "SellingState.h"
#include "CurrentShelf.h"
#include "CurrentVillager.h"
#include "Utility.h"
#include "FontManager.h"



SellingState::SellingState(GEX::StateStack & stack, Context context)
	: GEX::State(stack, context),
	_player(context.playerBody),
	_backgroundSprite(),
	_textures(context.textures),
	_total(0),
	_sold(false),
	_incrementer(1)
{
	_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::Warehouse));


	sf::Text option;
	option.setString("0");
	option.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	centerOrigin(option);
	option.setPosition(context.window->getView().getSize()/ 2.f);
	_amount.push_back(option);

	sf::Text item;
	auto item2 = GEX::CurrentShelf::getInstance().getCurrentShelf()->getItem().getType();
	auto s = GEX::CurrentShelf::getInstance().getCurrentShelf()->getItem().getItemName(item2);
	item.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	item.setString(s);
	centerOrigin(item);
	item.setPosition((context.window->getView().getSize() / 2.f) - sf::Vector2f(150.f,0.f));
	_amount.push_back(item);
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
		if (_sold)
		{
			//update all neccessary stuff
			_player->setMoney((_player->getMoney()+_total));
			GEX::CurrentShelf::getInstance().getCurrentShelf()->removeItemOnShelf();
			requestStackPop();
		}
		//if not sold update face and display some text
		auto v = GEX::CurrentVillager::getInstance().getCurrentVillager();
		auto item = (GEX::CurrentShelf::getInstance().getCurrentShelf()->getItem().getType());
		int willingToPay = v->getValue(item);
		
		if (_total <= willingToPay)//if amount less than amount willing to pay
		{
			_sold = true;
			//say farewell or thanks in text
		}
		else
		{
			//say that they dont want the item at that price
		}

		//updateText();
	}
	else if (event.key.code == sf::Keyboard::Right)
	{

		if (_total <= 99999)
		{
			_total += _incrementer;
		}
		else//if it reaches the top then reset to the 0 indexed item
		{
			_total = 0;
		}

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Left)
	{

		if (_total >= 1 && (!((_total / _incrementer) < 1)))
		{
			_total -= _incrementer;
		}
		else//if it reaches the bottom then reset to the 0 indexed item
		{
			_total = 0;
		}

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}
	else if (event.key.code == sf::Keyboard::Num1)
	{
		_incrementer = 1;
	}
	else if (event.key.code == sf::Keyboard::Num2)
	{
		_incrementer = 5;
	}
	else if (event.key.code == sf::Keyboard::Num3)
	{
		_incrementer = 10;
	}
	else if (event.key.code == sf::Keyboard::Num4)
	{
		_incrementer = 50;
	}
	else if (event.key.code == sf::Keyboard::Num5)
	{
		_incrementer = 100;
	}
	else if (event.key.code == sf::Keyboard::Num6)
	{
		_incrementer = 500;
	}
	else if (event.key.code == sf::Keyboard::Num7)
	{
		_incrementer = 1000;
	}

	return true;
}

void SellingState::updateOptionText()
{

	//update option text
	_amount[0].setString(std::to_string(_total));

}
