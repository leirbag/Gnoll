/*
 * main.c
 * Copyright (C) Paf 2007 <paf@tuxfamily.org>
 * 
 * main.cpp is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * main.cpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with main.cpp.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */


#include <iostream>
#include <iomanip>
#include <fstream>

#include <glibmm/ustring.h>
#include <boost/shared_ptr.hpp>

#include "../include/persistentobjectmanager.h"
#include "../include/persistentobject.h"
#include "../include/cpersistentobjectproxy.h"
#include "../include/basemanager.h"

#include "../include/scalar.h"
#include "../include/integer.h"
#include "../include/float.h"
#include "../include/string.h"
#include "../include/list.h"
#include "../include/double.h"
#include "../include/inherits.h"

#include "../include/sourcefile.h"
#include "../include/istream.h"


using namespace std;
using namespace Gnoll::Core;





class PJ : public CPersistentObjectProxy
{

	public:
	
		PJ(string _instanceName) : CPersistentObjectProxy(_instanceName)
		{
		
		}
		
		~PJ()
		{
		}
		
		void computeAge()
		{
			cout << "Age is : " << *(m_self->getAttribute<Integer>("age")) << endl;
		}

};




int main() {


	cout << "Hello world !" << endl;



	shared_ptr<ISource> loadChannel(new SourceFile(".", false));
	shared_ptr<ISource> saveChannel(new SourceFile(".", true));

	PersistentObjectManager *pom = PersistentObjectManager::getInstancePtr();

	pom->addLoadSource(loadChannel);
	pom->addSaveSource(saveChannel);
/*
	shared_ptr<PersistentObject> Zelda = pom->load("zelda");

	shared_ptr<Integer> ageZelda = Zelda->getAttribute< Integer > ("age");
	shared_ptr<Integer> newAge(new Integer((*ageZelda)() + 1));
	Zelda->setAttribute("age2", newAge);
*/
	shared_ptr<Integer> newAge(new Integer(1));

	PJ pj("zelda32");
	pj.computeAge();
	pj.setAttribute("age2", newAge);
	pj.save();
	return 0;
}



