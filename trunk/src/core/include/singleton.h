/***************************************************************************
 *   Copyright (C) 2006 by Puzzle Team                                     *
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


/*------------------------------singleton.h--------------------------------*\
|   A singleton template                                                    |
|                                                                           |
|   Changelog :                                                             |
|               06/23/2006 - Paf - Initial release                          |
|               13/07/2007 - Yellow - 'Singleton' ambiguous symbol, change  |
|               to '::Singleton'                                            |
|                                                                           |
\*-------------------------------------------------------------------------*/



#ifndef __SINGLETON_H__
#define __SINGLETON_H__



/**
 *	Interface of all game modules. 
 */ 
template <typename T> class Singleton
{
	private:

		static T* m_instance;
	

	public:

		/**
		 * A constructor
		 */
		Singleton() {}

		/**
		 * A virtual destructor
		 */
		virtual ~Singleton() {};

		/**
		 * This returns a pointer to the singleton
		 */
		static T* getInstancePtr() 
		{ 
			if (m_instance == 0)
				m_instance = new T;

			return m_instance;
		}

		/**
		 * This returns the singleton by reference
		 */
		static T& getInstance() 
		{ 
			if (m_instance == 0)
				m_instance = new T;

			return *m_instance;
		}


		/**
		 * This destroys the singleton
		 */
		static void destroy()
		{
			
			if (m_instance != 0)
			{
				delete m_instance;
				m_instance = 0;
			}			
		
		}

};

template <typename T> T *::Singleton<T>::m_instance = 0;


#endif // __SINGLETON_H__
