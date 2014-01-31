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

#include "Reaction.h"
#include "Sound.h"
#include <sstream>
#include <cstdlib>
#include <cassert>

SilkyStrings::Reaction::Reaction(boost::shared_ptr<GameView> gv, boost::shared_ptr<Sound> sound)
  : gv(gv), sound(sound), points(0), multiplier(1), multiplier_counter(0), mean_value(1), guitar_track(""){
  showPoints();

  const std::string preloaded[] =
  {
    "fiba1",
    "fiba2",
    "fiba3",
    "fiba4",
    "fiba5",
    "fiba6",
    "perfect1"
  };

  for (unsigned i = 0; i < sizeof (preloaded) / sizeof (std::string); i++)
    sound->preload (std::string ("./resources/") + preloaded[i] + ".ogg");
//sound->preload (std::string ("../resources/") + preloaded[i] + ".ogg");
}

void SilkyStrings::Reaction::correctPick(double dt){
  dt = dt<0 ? -dt:dt;

  mean_value = 9*mean_value/10 + dt;

  if(mean_value < PERFECT_PICK_THRESHOLD){
    mean_value = 1;

    points += 5*multiplier*PICK_POINTS;
    try{
      //sound->set_volume (sound->play("./resources/perfect1.ogg"),0.9); 	
      sound->set_volume (sound->play("./resources/perfect1.ogg"), 90.0);
    }catch(...){}
    gv->set_popup_notification("Perfect!", 1);

  }else
    points += multiplier*PICK_POINTS;
  
  if(++multiplier_counter >= 10){
    multiplier_counter = 0;
    setMultiplier(multiplier + 1);
  }

  if (guitar_track.length()>0)
    //sound->set_volume (guitar_track, 1.0f);
    sound->set_volume (guitar_track, 100.0);
  
  showPoints();
}

void SilkyStrings::Reaction::correctHold(double dt){
  points += multiplier*dt*HOLD_POINTS_PER_SECOND;
  if (guitar_track.length()>0)
    sound->set_volume (guitar_track, 100.0);
    //sound->set_volume (guitar_track, 1.0f);
  showPoints();
}

void SilkyStrings::Reaction::showPoints(){
  std::ostringstream os;
  os << (int)points;
  
  gv->set_static_notification(os.str());
}

void SilkyStrings::Reaction::playWrong(bool noisy){
  const std::string fibaFiles[6] =
  {
    "fiba1",
    "fiba2",
    "fiba3",
    "fiba4",
    "fiba5",
    "fiba6",
  };
  if (noisy)
  {
    try{
      sound->set_volume(sound->play("./resources/" + fibaFiles[std::rand() % 6] + ".ogg"), 70.0);
    }catch(...){}


    mean_value *= 1.1;
    multiplier_counter = 0;
    setMultiplier(1);
  }

  if (guitar_track.length()>0)
    sound->set_volume (guitar_track, 0.0);

}

void SilkyStrings::Reaction::setMultiplier(unsigned int m){
  if(m > 5 || multiplier == m) return;
  
  multiplier = m;

  std::ostringstream os;
  os << multiplier;

  gv->set_popup_notification(os.str() + "x", 1);
}

void SilkyStrings::Reaction::registerGuitarTrack(std::string g_track){
  guitar_track = g_track;
}
