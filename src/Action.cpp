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

#include "Action.h"
#include "Sound.h"

#include <boost/lexical_cast.hpp>

namespace SilkyStrings{

  Action::Action(boost::shared_ptr<WM> w,
                 boost::shared_ptr<Input> in,
                 MidiFileParser &p,
                 boost::shared_ptr<GameView> v,
                 boost::shared_ptr<Reaction> reaction,
                 boost::shared_ptr<Sound> sound,
                 std::string songPath)
    : wm(w),input(in),reaction(reaction), sound(sound), view(v), songPath(songPath){
    
    this->parser=&p;
    this->statetime=0;
    this->initialtime=0;
    this->chores=(this->parser)->getChores();
    this->gamerunning=true;
    this->dt=0;
    for(int i=0;i<5;i++)
      this->pickwaspressed[i]=false;
  }
  
  Action::~Action(){}
  
  void Action::start(){
    
    for (int i = 0; i < 3; i++)
      {
      double initial = wm->get_clock ();
      view->update_beginning_of_frame (initial);
      view->set_popup_notification (boost::lexical_cast<std::string> (3 - i), 1.0);
      while (!wm->update_beginning_of_frame () && wm->get_clock () - initial < 1.1)
      {
        view->update_beginning_of_frame (wm->get_clock ());
        for (int i = 0; i < 5; i++)
          view->draw_fret (i + 1, input->fret_pressed (i + 1));
        view->update_end_of_frame ();
        wm->update_end_of_frame ();
      }
    }

    sound->preload (songPath + "song.ogg");
    sound->preload (songPath + "guitar.ogg");

    initialtime = wm->get_clock() + 2.5;
    statetime = wm->get_clock() - initialtime;

    while(!wm->update_beginning_of_frame() && statetime < 0){
      view->update_beginning_of_frame(statetime);
      
      statetime = wm->get_clock() - initialtime;
      drawGameView();

      for (int i = 0; i < 5; i++)
        view->draw_fret (i + 1, input->fret_pressed (i + 1));

      view->update_end_of_frame();
      wm->update_end_of_frame();
    }

    sound->play(songPath + "song.ogg");
    reaction->registerGuitarTrack (sound->play(songPath + "guitar.ogg"));

    this->initialtime=(this->wm)->get_clock();
    
    while(this->gamerunning){

      if((this->wm)->update_beginning_of_frame())
	break;

      this->dt=(this->wm)->get_clock()-this->initialtime-this->statetime;
      this->statetime=(this->wm)->get_clock()-this->initialtime;
      (this->view)->update_beginning_of_frame(this->statetime);
      this->clearUsedChores();

     
      for(int i=1;i<6;i++){
	
	if((this->input)->fret_pressed(i)){
	  if((this->input)->pick_pressed()){
	    if(!this->pickwaspressed[i]){
	      this->pickwaspressed[i]=true;
	      if(!this->checkFretPressed(i)){
		(this->reaction)->playWrong();
		/**reaction.playWrong();
		 */
     	      }
	    }else{
	      if(this->fretHeld(i))
		(this->reaction)->correctHold(this->dt);
	      /**reaction.addPoints
	       */
	    }
	  }else{
	    this->pickwaspressed[i]=false;
	    if(this->fretHeld(i))
	      (this->reaction)->correctHold(this->dt);
	    this->checkFretFailed(i);
	  }
	}else if(!(this->input)->pick_pressed()){
	  this->pickwaspressed[i]=false;
	  this->checkFretFailed(i);
	}else{
	  this->checkFretFailed(i);
	}
      }
	
      if((this->input)->quit_pressed())
	break;
      
      //sound->update();
      this->drawGameView();
      for (int i = 0; i < 5; i++)
        view->draw_fret (i + 1, input->fret_pressed (i + 1));
      (this->view)->update_end_of_frame();
      (this->wm)->update_end_of_frame();
      
    }
  }
  
  void Action::drawGameView(){
    
    if(this->chores.size()>0){
      
      iter it=this->chores.begin();
      
      while((*it).getStart()<(this->statetime+10)){
	
	GameView::ChordViewState state;
	
	if((*it).getState()==Chore::COMING)
	  state=GameView::CHORD_VIEW_STATE_PASSIVE;
	else if((*it).getState()==Chore::HELD)
	  state=GameView::CHORD_VIEW_STATE_HELD_DOWN;
	else
	  state=GameView::CHORD_VIEW_STATE_FAILED;
	
	
	(this->view)->draw_chord((*it).getFret(),(*it).getStart(),(*it).getEnd(),state);
	
	it++;
	if(it==this->chores.end()){
	  break;
	}
      }
    }
  }
  
  void Action::clearUsedChores(){

    
    if(this->chores.size()>0){

      iter it=this->chores.begin();
      
      if((*it).getEnd()<(this->statetime-0.8)){
	
	while((*it).getEnd()<(this->statetime-0.3)){
	  it++;
	  if(it==this->chores.end())
	    break;
	}

	this->chores.erase(this->chores.begin(),it);
      }
    }
  }

  bool Action::checkFretPressed(int f){
    
    if(this->chores.size()>0){

      iter it=this->chores.begin();
      bool isfound=false;
      
      while((*it).getStart()<(this->statetime+(*it).getDt())){
	if((*it).getFret()==f&&(*it).pressNow(this->statetime)){
	  if((*it).getState()==Chore::COMING){
	    isfound=true;
	    (*it).setState(Chore::HELD);
	    double dt=this->statetime-(*it).getStart();
	    if(dt<0)
	      dt*=-1;
	    (this->reaction)->correctPick(dt);
	    break;
	  }else{
	    isfound=true;
	    (*it).setState(Chore::HELD);
	    /**reaction.addPoints();
	     */
	    break;
	  }
	}else{
	  it++;
	  if(it==this->chores.end())
	    break;
	}
      }

      
      return isfound;
    }
    return true;
  }
  
  bool Action::fretHeld(int f){
    
    if(this->chores.size()>0){
      
      iter it=this->chores.begin();
      bool isfound=false;
      
      while((*it).getStart()<(this->statetime+(*it).getDt())){
	if(((*it).getFret()==f&&(*it).getState()==Chore::HELD)&&((*it).isBetween(this->statetime)||(*it).pressNow(this->statetime))){
	  isfound=true;
	  break;
	}else{
	  it++;
	  if(it==this->chores.end())
	    break;
	}
      }
      return isfound;
    }
    return false;
 
  }
  void Action::checkFretFailed(int f){
    
    if(this->chores.size()>0){
      
      iter it=this->chores.begin();
      
      while((*it).getStart()<(this->statetime+(*it).getDt())){
	if((*it).getFret()==f&&(*it).isBetween(this->statetime)&&(*it).getState()==Chore::COMING){
	  (*it).setState(Chore::FAILED);
      reaction->playWrong (false);
	  break;
	}else if((*it).getFret()==f&&(*it).isBetween(this->statetime)&&(*it).getState()==Chore::HELD&&!input->fret_pressed(f)){
      (*it).setState(Chore::FAILED);
      reaction->playWrong (false);
      break;
    }
    else{
	  it++;
	  if(it==this->chores.end())
	    break;
	}
      }
    }
 
  }

  void Action::addReaction(boost::shared_ptr<Reaction> r){

    this->reaction=r;
  }
}
  

  
  
  


    
    

    


