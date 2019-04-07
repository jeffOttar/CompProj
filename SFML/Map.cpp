#include "Map.h"
#include "Utility.h"
#include "FontManager.h"
#include "ResourceIdentifiers.h"

//#include "Player.h"


	Map::Map(GEX::StateStack & stack, Context context):
		State(stack, context),
		_options(),
		_optionsIndex(0)
	{
		_money = context.playerBody->getMoney();

		sf::Texture& texture = context.textures->get(GEX::TextureID::Map);

		_backgroundSprite.setTexture(texture);
		_backgroundSprite.scale(1.5, 1.5);

		sf::Sprite shopOption;
		shopOption.setTexture(context.textures->get(GEX::TextureID::ShopIcon));
		//shopOption.setTexture = context.textures->get(GEX::TextureID::ShopIcon);
		centerOrigin(shopOption);
		shopOption.setPosition(context.window->getView().getSize().y / 1.2f, context.window->getView().getSize().x / 2.5f);
		_options.push_back(shopOption);//add the option to the list

		sf::Sprite warehouseOption;
		//warehouseOption.setTexture = context.textures->get(GEX::TextureID::WarehouseIcon);
		warehouseOption.setTexture(context.textures->get(GEX::TextureID::WarehouseIcon));
		centerOrigin(warehouseOption);
		warehouseOption.setPosition(context.window->getView().getSize().y / 5.f, context.window->getView().getSize().x / 2.f);
		_options.push_back(warehouseOption);//add the option to the list

		sf::Sprite marcoOption;
		//marcoOption.setTexture = context.textures->get(GEX::TextureID::MarcoMoney);
		marcoOption.setTexture(context.textures->get(GEX::TextureID::MarcoMoney));
		centerOrigin(marcoOption);
		marcoOption.setPosition(context.window->getView().getSize().y / 1.5f, context.window->getView().getSize().x / 12.f);
		_options.push_back(marcoOption);//add the option to the list
		
		
		//set up the menu 
		//the play option settings
		//sf::Text playOption;
		//playOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		//playOption.setString("Play");
		//centerOrigin(playOption);
		//playOption.setPosition(context.window->getView().getSize() / 2.f);
		//_options.push_back(playOption);//add the option to the list

		////exit option settings
		//sf::Text exitOption;
		//exitOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
		//exitOption.setString("Exit");
		//centerOrigin(exitOption);
		//exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
		//_options.push_back(exitOption);//add the option to the list

		//this updates the display to show current selection
		updateOptionSelect();
	}
	void Map::draw()
	{
		sf::RenderWindow& window = *getContext().window;
		window.setView(window.getDefaultView());

		//draw objects
		window.draw(_backgroundSprite);
		for (const sf::Sprite& icon : _options)
		{
			window.draw(icon);
		}
	}
	bool Map::update(sf::Time dt)
	{
		return false;
	}
	bool Map::handleEvent(const sf::Event & event)
	{
		//if the event is not a keypress
		if (event.type != sf::Event::KeyPressed)
		{
			return false;
		}

		if (event.key.code == sf::Keyboard::Return)//if key press is return
		{
			if (_optionsIndex == Warehouse)// if currently selected option is warehouse
			{
				requestStackPop();
				requestStackPush(GEX::StateID::Warehouse);
			}
			else if (_optionsIndex == Marco)
			{
				if (_money >= 100000000)
				/*if (_money >= 1)*/
				{
					requestStackPop();
					requestStackPush(GEX::StateID::MarcoLand);
				}
			}
			else if (_optionsIndex == Shop)
			{
				//remove the item from the stack so it is empty (pop will empty stack because the menu has nothing under it)
				requestStackPop();
			}
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

			updateOptionSelect();
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

			updateOptionSelect();
		}

		return false;
	}
	void Map::updateOptionSelect()
	{
		if (_options.empty())
		{
			return;
		}

		for (sf::Sprite& icon : _options)//set all icons to original
		{
			icon.setScale(1.f, 1.f);
		}

		//change the selected icon scale
		_options[_optionsIndex].scale(2.f,2.f);
	}
