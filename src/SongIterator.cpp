/* This file is part of SilkyStrings 
 * Copyright (C) 2006  Olli Salli, Tuomas Per�l�, Ville Virkkala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "SongIterator.h"

#include <boost/filesystem/path.hpp>
#include <string>


Launcher::SongIterator::SongIterator(boost::filesystem::path path)
: iter(boost::filesystem::directory_iterator(path)) {
			
	while(!checkValidity() && iter != boost::filesystem::directory_iterator())
		iter++;
}

Launcher::SongIterator::SongIterator()
: iter(boost::filesystem::directory_iterator()) {};


bool Launcher::SongIterator::operator!=(const SongIterator &iter){
	return this->iter != iter.iter;
}

bool Launcher::SongIterator::operator==(const SongIterator &iter){
	return this->iter == iter.iter;
}

std::string Launcher::SongIterator::operator*(){
	//iter->leaf();
	return iter->path().string();
	//return iter->path().filename();
}

Launcher::SongIterator Launcher::SongIterator::operator++(){
	do{
		iter++;
	}while(iter != boost::filesystem::directory_iterator() && !checkValidity());

	return *this;
}

bool Launcher::SongIterator::checkValidity(){
	return  exists(*iter/"guitar.ogg") &&
			exists(*iter/"notes.mid") &&
			exists(*iter/"song.ini") &&
			exists(*iter/"song.ogg");
}
