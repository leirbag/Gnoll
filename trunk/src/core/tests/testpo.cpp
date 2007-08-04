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
#include <fstream>

#include <glibmm/ustring.h>
#include <boost/shared_ptr.hpp>

#include "../include/persistentobject.h"
#include "../include/scalar.h"

#include "../include/sourcefile.h"
#include "../include/istream.h"


using namespace std;

int main() {


	cout << "Hello world !" << endl;

	PersistentObject toto = PersistentObject("toto");	

	shared_ptr<Scalar<float> > pi( new Scalar<float>(3.145) );
	shared_ptr<Scalar<Glib::ustring> > phrase ( new Scalar<Glib::ustring>("c'etait un cafard qui vivait dans le noir") );

	toto.setAttribute( Glib::ustring("pi"), pi);
	toto.setAttribute( "phrase", phrase);

	cout << (*pi)() << endl;
	cout << (*phrase)() << endl;

	cout << "Toto has attribute 'pi' : " << toto.hasAttribute("pi") << endl;
	cout << "Toto has attribute 'phrase' : " << toto.hasAttribute("phrase") << endl;
	cout << "Toto has attribute 'tournoyant' : " << toto.hasAttribute("tournoyant") << endl;

	const Glib::ustring pi_name("pi");


	shared_ptr<Scalar<float> > pi2 = toto.getAttribute< Scalar<float> > (pi_name);
	shared_ptr<Scalar<Glib::ustring> > phrase2 = toto.getAttribute< Scalar<Glib::ustring> > ("phrase");


	cout << "Get attribute 'pi' : " << (*pi2)() << endl;
	cout << "Get attribute 'phrase' : " << (*phrase2)() << endl;
	
	try 
	{
		shared_ptr<Scalar<Glib::ustring> > phrase3 = toto.getAttribute< Scalar<Glib::ustring> > ("aioli");
	}
	catch(Glib::ustring e)
	{
		cout << e << endl;
	}


	cout << endl << "Serializationed (I'am sure it exists :) toto : " << endl;

	shared_ptr<xmlpp::Document> output = toto.serializeXML();
	cout << output->write_to_string_formatted()  << endl;


	SourceFile sf("./");

	shared_ptr<IStream> stream = sf.load("testpo.cpp");
	unsigned int bytesRead = 0;
	while (!stream->eof())
	{
		char buffer[256];
		unsigned int nbRead = stream->read(buffer, 255);
		buffer[nbRead] = 0;
		cout << buffer;
		bytesRead += nbRead;
	}
	stream->close();

	cout << endl << "Bytes Read : " << bytesRead << endl;

	sf.setOverWrite(true);

	shared_ptr<IStream> streamW = sf.load("testpo.cppW");

	unsigned int bytesWritten = streamW->write("Jesuislorsdelajusticeetdel'emeriteca\nvalier", 40);
	streamW->close();

	cout << endl << "Bytes Written : " << bytesWritten << endl;
	

	return 0;
}



