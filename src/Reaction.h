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

#ifndef REACTION_H
#define REACTION_H

#include "GameView.h"
#include "Sound.h"
#include <boost/shared_ptr.hpp>


namespace SilkyStrings{

  const double HOLD_POINTS_PER_SECOND = 100;
  const double PICK_POINTS = 300;
  const double PERFECT_PICK_THRESHOLD = 0.20;


  /**
   * Reaction class responds to the user's actions
   */
  class Reaction{
  public:

    /** Constructor
     * @param gv a valid GameView instance
     * @param sound a valid Sound instance
     */
    Reaction(boost::shared_ptr<GameView> gv, boost::shared_ptr<Sound> sound);

    /** Reacts to a correct pick
     * @param dt how much the pick went wrong in seconds
     */
    void correctPick(double dt);

    /** Reacts to a correct hold
     * @param dt elapsed time since the last correctHold call for a certain fret = 1/fps
     */
    void correctHold(double dt);

    /** Reacts to a wrong pick
     * @param noisy If the mistake should cause an irritating sound to be
     * played.
     */
    void playWrong(bool noisy = true);

    /** Register a guitar track to mute/unmute
     */
    void registerGuitarTrack(const std::string quitar_track);

  private:
    void showPoints();
    void setMultiplier(unsigned int m);

    boost::shared_ptr<GameView> gv;
    boost::shared_ptr<Sound> sound;
    double points;    
    unsigned int multiplier;
    unsigned int multiplier_counter;
    double mean_value;
    std::string guitar_track;
  };
}

#endif
