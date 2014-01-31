/* This file is part of SilkyStrings 
 * Copyright (C) 2006  Olli Salli, Tuomas Perälä, Ville Virkkala
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

#include "midiStream.h"



MidiStream::MidiStream(std::string file):fin(file.c_str()){}
MidiStream::~MidiStream(){

  this->fin.close();
}

std::string MidiStream::getChunkName(){

  std::string header="";
  char ch;
  int i=0;

  while(i<4){

    this->fin.get(ch);
    header.push_back(ch);
    i++;
  }

  return header;
}

unsigned long MidiStream::getChunkLength(){

  unsigned long num=0;
  char c;


  for(int i=0;i<4;i++){
    this->fin.get(c);
    num+=(unsigned char)c;
    if(i!=3)
      num=num<<8;
  }

  return num;  
  
}

  
unsigned short MidiStream::getFormat(){

  unsigned short num=0;
  char c;
  

  for(int i=0;i<2;i++){
    this->fin.get(c);
    num+=(unsigned char)c;
    if(i!=1)
      num=num<<8;
  }

  return num;  
  
}


unsigned short MidiStream::getNTracks(){

  unsigned short num=0;
  char c;


  for(int i=0;i<2;i++){
    this->fin.get(c);
    num+=(unsigned char)c;
    if(i!=1)
      num=num<<8;
  }

  return num;  
  
}
short MidiStream::getDivision(){

  short num=0;
  char c;


  for(int i=0;i<2;i++){
    this->fin.get(c);
    num+=(unsigned char)c;
    if(i!=1)
      num=num<<8;
  }

  return num;  
  
}

unsigned long MidiStream::readVarLen()
{
  unsigned long value;
  char c;

  this->fin.get(c);
 
  if ((value=(unsigned char)c) & 0x80 )
    {
      value &= 0x7F;
      do
	{
	  this->fin.get(c);
	  value = (value << 7) + (((unsigned char) c) & 0x7F);
	} while (c & 0x80);
    }
  
  return value;
}

unsigned long MidiStream::readVarLen(int &i){

  unsigned long value;
  char c;
  int a=0;
  i++;
  this->fin.get(c);
 
  if ((value=(unsigned char)c) & 0x80 )
    {
      value &= 0x7F;
      do
	{
	  i++;
	  a++;
	  this->fin.get(c);
	  value = (value << 7) + (((unsigned char) c) & 0x7F);
	} while ((c & 0x80)&&a<4);
    }
  
  return value;
}



unsigned char MidiStream::getEvent(){

  char c;
  this->fin.get(c);
  return (unsigned char)c;
}

unsigned char MidiStream::getType(){

  char c;

  this->fin.get(c);
  return (unsigned char)c;
}

unsigned char *MidiStream::getData(int bytes){

  char c;
  unsigned char *data=NULL;
  data=new unsigned char[bytes];

  for(int i=0;i<bytes;i++){
    if(!this->fin.get(c)){
      delete []data;
      return NULL;
    }else
      data[i]=(unsigned char)c;
    
  }
    
  return data;
}
void MidiStream::putback(unsigned char c){

  this->fin.putback(c);
}
