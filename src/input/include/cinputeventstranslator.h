/***************************************************************************
 *   Copyright (C) 2008 by Paf                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __CINPUTEVENTSTRANSLATOR_H__
#define __CUNPUTEVENTSTRANSLATOR_H__

#include <boost/shared_ptr.hpp>

#include "../../core/include/module.h"
#include "../../core/include/singleton.h"
#include "../../core/messages/include/listener.h"

using namespace boost;
using namespace Gnoll::Core;

namespace Gnoll
{
	namespace Input
	{
		/**
		 *	Interface of all game modules.
		 */
		class CInputEventsTranslator: public Module, public Gnoll::Core::Singleton<CInputEventsTranslator>
		{
			private:

				/**
				 * Translators for keyboard generated messages
				 */
				shared_ptr<Messages::Listener> keyboardEventsTranslator;
				shared_ptr<Messages::Listener> keyboardStateTranslator;


				/**
				 * Translator for keyboard generated messages
				 */
				shared_ptr<Messages::Listener> keyboardEventsTrigger;


				/**
				 * How often keyboard events will be triggered
				 */
				shared_ptr<boost::any> m_periodData;

				/**
				 * Translator for mouse motion generated messages
				 */
				shared_ptr<Messages::Listener> mouseMotionEventsTranslator;
				shared_ptr<Messages::Listener> mouseMotionStateTranslator;


				/**
				 * Translators for mouse button generated messages
				 */
				shared_ptr<Messages::Listener> mouseButtonEventsTranslator;
				shared_ptr<Messages::Listener> mouseButtonStateTranslator;

				/**
				 * Translator for mouse generated messages
				 */
				shared_ptr<Messages::Listener> mouseButtonEventsTrigger;

				/**
				 * Translators for joystick axis generated messages
				 */
				shared_ptr<Messages::Listener> joystickAxisEventsTranslator;
				shared_ptr<Messages::Listener> joystickAxisStateTranslator;


				/**
				 * Rule manager
				 */
				shared_ptr<Messages::Listener> ruleManager;

			public:
				/**
				 * @copydoc Module::init
				 */
				virtual void init();

				/**
				 * @copydoc Module::process
				 */
				virtual void process();

				/**
				 * @copydoc Module::exit
				 */
				virtual void exit();

				/**
				 * @copydoc Module::~Module
				 */
				virtual ~CInputEventsTranslator();

				/**
				 * @copydoc Module::Module
				 */
				CInputEventsTranslator();

				/**
				 * Activate keyboard events translation
				 */
				void activateKeyboardTranslation();

				/**
				 * Deactivate keyboard events translation
				 */
				void deactivateKeyboardTranslation();

				/**
				 * Activate mouse motion events translation
				 */
				void activateMouseMotionTranslation();

				/**
				 * Deactivate mouse motion events translation
				 */
				void deactivateMouseMotionTranslation();

				/**
				 * Activate mouse button events translation
				 */
				void activateMouseButtonTranslation();

				/**
				 * Deactivate mouse button events translation
				 */
				void deactivateMouseButtonTranslation();

				/**
				 * Activate mouse button events translation
				 */
				void activateJoystickAxisTranslation();

				/**
				 * Deactivate mouse button events translation
				 */
				void deactivateJoystickAxisTranslation();


				/**
				 * Activate rule manager
				 */
				void activateRuleManager();

				/**
				 * Deactivate rule manager
				 */
				void deactivateRuleManager();
		};

	}
}

#endif // __CINPUTEVENTSTRANSLATOR_H__


