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

#include "midiFileParser.h"
#include <cassert>

MidiFileParser::MidiFileParser(std::string name, Difficulty difficulty)
  : stream(name.c_str ())
{
  switch (difficulty)
  {
    case DIFFICULTY_SUPAEASY:
      this->difficulty = 0x3c;
      break;
    case DIFFICULTY_EASY:
      this->difficulty = 0x48;
      break;
    case DIFFICULTY_MEDIUM:
      this->difficulty = 0x54;
      break;
    case DIFFICULTY_AMAZING:
      this->difficulty = 0x60;
      break;
    default:
      assert (false);
      break;
  }

  this->headername=this->stream.getChunkName();
  this->headerlength=this->stream.getChunkLength();
  this->format=this->stream.getFormat();
  this->ntracks=this->stream.getNTracks();
  this->division=this->stream.getDivision();
  this->readMidiFile();
  this->makeChores();
}

MidiFileParser::~MidiFileParser(){}

std::string MidiFileParser::getHeaderChunkName(){

  return this->headername;
}
unsigned short MidiFileParser::getFormat(){

  return this->format;
}

unsigned short MidiFileParser::getNTracks(){

  return this->ntracks;
}

short MidiFileParser::getDivision(){

  return this->division;
}

unsigned long MidiFileParser::getChunkLength(){

  return this->headerlength;
}

void MidiFileParser::readTrack(){

  std::string chunkheader=this->stream.getChunkName();
  unsigned long length=this->stream.getChunkLength();
  double absolutetime=0;
  int i=0;
  
  while(i<(int)length){

    unsigned long delta=this->stream.readVarLen(i);
    absolutetime+=this->getAbTime((int)delta);
    i++;
    unsigned char event=this->stream.getEvent();
    

    if(!(event&0x80)){
      this->stream.putback(event);
      i--;
      event=this->runningstatus;
    }else
      this->runningstatus=event;

   
      
    if(event==0xFF){
         
      i++;
      unsigned char type=this->stream.getType();
      int datalen=(int)this->stream.readVarLen(i);
      i+=datalen;
      unsigned char *data=this->stream.getData(datalen);
           
      if(type==0x51){
	this->tempo=this->getAmount(data,3);
      }
      delete [] data;
    }else if(event==0xF0){
      int datalen=(int)this->stream.readVarLen(i);
      i+=datalen;
      unsigned char *data=this->stream.getData(datalen);
      delete [] data;
    }else if(event==0xF7){
      int datalen=(int)this->stream.readVarLen(i);
      i+=datalen-1;
      unsigned char *data=this->stream.getData(datalen-1);
      delete [] data;
    }else{
      
      unsigned char c=(event>>4);

      if(c==12||c==13){
	i++;
	unsigned char* data=this->stream.getData(1);
	if(data==NULL)
	  break;
	delete [] data;
      }else{
	i+=2;
	unsigned char *data=this->stream.getData(2);
	if(data==NULL)
	  break;
	this->checkMidiEvent(c,data,absolutetime);
	delete [] data;
      }
      
    }
  }
  

}

int MidiFileParser::coutMidiEvent(unsigned char t,unsigned char* data,double abtime){


  if(t==8)
    std::cout<<"Event:Midi Event, noteoff "<<(int)data[0]<<" "<<(int)data[1]<<" "<<abtime<<" "<<"\n";
  else if(t==9)
    std::cout<<"Event:Midi Event, noteon "<<(int)data[0]<<" "<<(int)data[1]<<" "<<abtime<<" "<<"\n";
  else if(t==10)
    std::cout<<"Event:Midi Event, note after touch\n";
  else if(t==11)
    std::cout<<"Event:Midi Event, Controller\n";
  else if(t==12)
    std::cout<<"Event:Midi Event, Program change\n";
  else if(t==13)
    std::cout<<"Event:Midi Event, channel after\n";
  else if(t==14)
    std::cout<<"Event:Midi Event, Pitch Bend\n";
  else{
    std::cout<<"Something wrong\n";
    return 1;
  }
  return 0;

}

int MidiFileParser::getAmount(unsigned char *data,int len){
  
  unsigned long num=0;
  
  for(int i=0;i<len;i++){
    num+=data[i];
    if(i!=len-1)
      num=(num<<8);
  }
  
  return (int)num;  
  
}

double MidiFileParser::getAbTime(int delta){

  double multi=this->tempo/this->division;
  return multi*delta/1000000;
}

void MidiFileParser::checkMidiEvent(unsigned char t,unsigned char* data,double abtime){

  if((t==8||t==9)&&((int)data[0]>=this->difficulty&&(int)data[0]<=(this->difficulty+4))){
    int fret=(int)data[0]-this->difficulty+1;
    MidiEvent event(fret,t,abtime);
    this->midis.push_back(event);
  }
  
}

void MidiFileParser::readMidiFile(){

  for(int i=0;i<this->ntracks;i++){
    this->readTrack();
  }
}

std::vector<MidiEvent> MidiFileParser::getMidis(){

  return this->midis;
}

std::vector<Chore> MidiFileParser::getChores(){

  return this->chores;
}

void MidiFileParser::makeChores(){

  std::vector<MidiEvent>::iterator it;

  for(it=this->midis.begin();it!=this->midis.end();it++){

    if((*it).getEvent()==9){
      Chore c((*it).getFret(),(*it).getTime());
      std::vector<MidiEvent>::iterator it2;

      for(it2=it;it2!=this->midis.end();it2++){

	if((*it2).getEvent()==8&&(*it2).getFret()==(*it).getFret()){
	  c.setEnd((*it2).getTime());
	  break;
	}
      }
      this->chores.push_back(c);
    }
  }
}



  






  


  






