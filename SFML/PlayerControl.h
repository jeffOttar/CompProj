/**
* @file
* @author Jeff Ottar-
* @version 1.0
*
*
* @section DESCRIPTION
* <  >
*
*
* @section LICENSE
*
*
* Copyright 2017
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/
#pragma once

#include <map>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include "Command.h"

namespace GEX {

	//forward declaration
	class CommandQueue;

	enum class Action {
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		RR,
		RL,
		Fire,
		LaunchMissile,
		Interact,
	};

	enum class MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};

	class PlayerControl
	{
	public:
		PlayerControl();

		void										handleEvent(const sf::Event& event, CommandQueue& commands);
		void										handleRealTimeInput(CommandQueue& commands);


		void 										setMissionStatus(MissionStatus status);
		MissionStatus 								getMissionStatus() const;
	private:
		void										initializeActions();
		static bool									isRealtimeAction(Action action);

	private:
		std::map<sf::Keyboard::Key, Action>			_keyBindings;
		std::map<Action, Command>					_actionBindings;
		MissionStatus 								_currentMissionStatus;
	};
}

