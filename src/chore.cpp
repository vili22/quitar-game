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

#include "chore.h"



Chore::Chore(int frt,double begin){
  this->fret=frt;
  this->start=begin;
  this->state=COMING;
  this->dt=0.075;
}
Chore::Chore(const Chore &c){
  this->fret=c.fret;
  this->event=c.event;
  this->start=c.start;
  this->end=c.end;
  this->dt=c.dt;
  this->state=c.state;
}

Chore::~Chore(){}
Chore &Chore::operator=(const Chore &c){
  if(&c!=this){
    this->fret=c.fret;
    this->event=c.event;
    this->start=c.start;
    this->end=c.end;
    this->dt=c.dt;
    this->state=c.state;
  }
  return *this;
}

unsigned char Chore::getEvent(){
  return this->event;
}

int Chore::getFret(){
  return this->fret;
}

double Chore::getTime(){
  return (this->end-this->start);
}

double Chore::getStart(){
  return this->start;
}

double Chore::getEnd(){

  return this->end;
}

void Chore::setEnd(double e){

  this->end=e;
}

bool Chore::isBetween(double time){

  return (time>this->start&&time<this->end);
}

Chore::ChoreState Chore::getState(){
  
  return this->state;
}

void Chore::setState(Chore::ChoreState s){
  
  this->state=s;
}

bool Chore::pressNow(double t){
  
  return (t>(this->start-this->dt)&&t<(this->start+dt));
}

double Chore::getDt(){

  return this->dt;
}



