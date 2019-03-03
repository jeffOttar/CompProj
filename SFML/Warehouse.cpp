#include "Warehouse.h"
#include "Utility.h"
#include "FontManager.h"
#include "CommandQueue.h"
#include "Item.h"
#include <math.h>


	Warehouse::Warehouse(GEX::StateStack & stack, Context context):
		State(stack, context),
		_backgroundSprite(),
		_textures(context.textures),
		_itemCount(0)
	{
		_money = context.playerBody->getMoney();
		//set the background face texture
		_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::Warehouse));
		//centerOrigin(_backgroundSprite);
		//_backgroundSprite.setColor(sf::Color(255, 255, 255, 100));//make image transparent

		//make loop creating a scrollable list of text for the items

		GEX::Item tmp(GEX::Item::Type::Bread, *_textures);
		std::vector<GEX::Item::Type> itemTypes = tmp.getAllTypes();

		for (int i = 0; i < itemTypes.size(); i++)
		{
			auto item = new GEX::Item(itemTypes.at(i), *_textures);
			_items.push_back(*item);
		}
		for (int i = 0; i < itemTypes.size(); i++)
		{
			sf::Text option;
			option.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
			option.setString((tmp.getItemName(itemTypes[i]) + std::to_string(_items.at(i).getPrice()) + std::to_string(_itemCount)));
			centerOrigin(option);
			option.setPosition(context.window->getView().getSize() + sf::Vector2f(0.f, (30.f + 30.f * i)));
			_options.push_back(option);//add the option to the list
		}
	}
	void Warehouse::draw()
	{
		sf::RenderWindow& window = *getContext().window;
		window.setView(window.getDefaultView());

		//sf::RectangleShape backgroundShape;//the translucent background
		//backgroundShape.setFillColor(sf::Color(255, 0, 0, 100));
		//backgroundShape.setSize(window.getView().getSize());

		//draw objects
		//window.draw(backgroundShape);
		window.draw(_backgroundSprite);

		//draw each of the text options
		for (const sf::Text& text : _options)
		{
			window.draw(text);
		}
	}
	bool Warehouse::update(sf::Time dt)
	{
		return false;
	}
	bool Warehouse::handleEvent(const sf::Event & event)
	{
		//if the event is not a keypress
		if (event.type != sf::Event::KeyPressed)
		{
			return false;
		}

		if (event.key.code == sf::Keyboard::Return)//if key press is return
		{
			//buy the items
		}
		else if (event.key.code == sf::Keyboard::Up)//if key is up
		{
			//if they use the up key move the index in the correct direction for the selections
			if (_optionsIndex > 0)
			{
				_optionsIndex--;
			}
			else
			{
				_optionsIndex = _options.size() - 1;
			}

			updateOptionText();
		}
		else if (event.key.code == sf::Keyboard::Down)//if key is down
		{
			//if they use the up key move the index in the correct direction for the selections
			if (_optionsIndex < _options.size() - 1)// if it is less than the max size/ top of the vector
			{
				_optionsIndex++;
			}
			else//if it reaches the top then reset to the 0 indexed item
			{
				_optionsIndex = 0;
			}

			updateOptionText();
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			
			if (_money >= (_items.at(_optionsIndex).getPrice()*_itemCount))
			{
				_itemCount++;
			}
			else//if it reaches the top then reset to the 0 indexed item
			{
				_itemCount = 1;
			}

			updateOptionText();
		}
		else if (event.key.code == sf::Keyboard::Left)
		{

			if (_itemCount>0)
			{
				_itemCount--;
			}
			else//if it reaches the top then reset to the 0 indexed item
			{
				int flooredAmount = std::floor(_money / _items.at(_optionsIndex).getPrice());
				_itemCount = flooredAmount;
			}

			updateOptionText();
		}

		return true;
	}

	void Warehouse::updateOptionText()
	{
		if (_options.empty())
		{
			return;
		}

		for (sf::Text& text : _options)//set all text to white
		{
			text.setFillColor(sf::Color::White);
			text.setOutlineColor(sf::Color::Black);
		}

		//change the selected texts color
		_options[_optionsIndex].setFillColor(sf::Color::Magenta);
	}


