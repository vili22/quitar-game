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

#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "Input.h"
#include "GameView.h"
#include "WM.h"
#include "midiFileParser.h"
#include "Reaction.h"

namespace SilkyStrings{


  typedef std::vector<Chore> Chores;
  typedef std::vector<Chore>::iterator iter;

  /**
   * The Action class keeps game running.
   */

  class Action{
    
   
    
  public:

    /** Constructor
     *
     * @param w A valid pointer to WM instance.
     * @param in A valid pointer to Input instance.
     * @param p Reference to MidiFileParser.
     * @param v A valid pointer to GameView instance.
     * @param reaction A valid pointer to Reaction instance.
     * @param sound A valid pointer to Sound instance.
     * @param songPath File path to directory where the souds lies.
     */
    
    Action(boost::shared_ptr<WM> w,
           boost::shared_ptr<Input> in,
           MidiFileParser &p,
           boost::shared_ptr<GameView> v,
           boost::shared_ptr<Reaction> reaction,
           boost::shared_ptr<Sound> sound,
           std::string songPath);
    ~Action();

    /** Starts game and keeps game rolling.
     */

    void start();

    /** Adds reaction.
     *
     * @param reaction A valid pointer to Reaction instance.
     */

    void addReaction(boost::shared_ptr<Reaction> reaction);
    
    
  private:
    
    Chores chores;
    boost::shared_ptr<WM> wm;
    boost::shared_ptr<Input> input;
    boost::shared_ptr<Reaction> reaction;
    boost::shared_ptr<Sound> sound;

    MidiFileParser *parser;
    boost::shared_ptr<GameView> view;
    std::string songPath;
    double statetime;
    double initialtime;
    double dt;
    bool gamerunning;
    bool pickwaspressed[5];
    void clearUsedChores();
    void drawGameView();
    bool checkFretPressed(int fret);
    bool fretHeld(int fret);
    void checkFretFailed(int fret);
  };
}



