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

#ifndef __ABSTRACTSOURCE_H__
#define __ABSTRACTSOURCE_H__

#include <boost/shared_ptr.hpp>
#include <libxml++/libxml++.h>

#include "abstractstream.h"

using namespace std;
using namespace boost;

namespace Gnoll
{
	namespace Core
	{
		/**
		 * This is the interface of all Sources.</br>
		 * A Source is an abstraction that can return a stream from a url.</br>
		 * So through this interface one can get a stream from a local file,</br>
		 * a file located on a ftp server, or procedurally generated...
		 */
		class AbstractSource
		{
			public:
				/**
				* This is a constructor.
				*/
				AbstractSource(unsigned int priority) : m_priority(priority) {};

				/**
				* This is a destructor.
				*/
				virtual ~AbstractSource() {};

				/**
				 * This loads a stream from a given URL
				 * @param _url URL to load
				 * @return Stream based on this URL
				 */
				virtual shared_ptr<AbstractStream> loadStream(const string url) = 0;

				/**
				 * This loads a stream with written permissions to a given URL
				 * @param _url URL to save to
				 * @return True if the operation is successful. False otherwise
				 */
				virtual shared_ptr<AbstractStream> saveStream(const string url) = 0;

				/**
				 *	This methods tells if a readable stream can be built from a given URL
				 *	@param _url URL to be tested
				 *	@param True if a readable stream can be built from this URL
				 */
				virtual bool isFetchable(const string url) = 0;

				/**
				 *	This methods tells if a writable stream can be built from a given URL
				 *	@param _url URL to be tested
				 *	@param True if a writable stream can be built from this URL
				 */
				virtual bool isWritable(const string url) = 0;

				/**
				 * Comparison operator to compare two sources based on their priorities
				 * @param op Source to compare to
				 * @return True if this source has a smaller priority
				 */
				bool operator<(shared_ptr<AbstractSource> op) { return this->m_priority < op->m_priority; }

			private:
				/**
				 * This is the priority of a Source. This is
				 * Example : A local source should have a higher priority than a remote source.
				 */
				unsigned int m_priority;
		};
	}
}

#endif 
