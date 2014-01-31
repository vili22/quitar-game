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

/** The Chore class that contains information for single chore.
 *
 */


class Chore{

  
 public:

  /**
   * Tells in which state the chore is.
   */

  enum ChoreState{
    
    COMING,
    HELD,
    FAILED
  };

  /** Constructor.
   *
   * @param fret The quitar fret where the chore songs.
   * @param begin The start time of the chore.
   */

  Chore(int fret,double begin);
  Chore(const Chore &c);
  ~Chore();
  Chore &operator=(const Chore &c);
  unsigned char getEvent();

  /** Tells the quitar fret where the chore songs.
   *
   * @return quitar fret.
   */

  int getFret();

  /** Tells how long the chore last.
   *
   * @return Time between end and begin.
   */

  double getTime();

  /** Tells when the chore should be played.
   *
   * @return Chores start time in seconds.
   */

  double getStart();

  /** Tells when the chore ends.
   *
   * @return Chores end in seconds.
   */

  double getEnd();

  /** Sets the end time of chore in seconds.
   *
   * @param end End time in seconds.
   */

  void setEnd(double end);

  /** Tells if the given time is between begin and end.
   *
   * @param time Time is between begin or end or is not.
   * @return true if time is between false otherwise.
   */

  bool isBetween(double time);

  /** Tells if the player should pick this chord at given time
   *
   * @param time Time when the player tries to pick this chore.
   * @return true if chore should be picked at given time false otherwise.
   */

  bool pressNow(double time);

  /** Tells how long is the pick time.
   *
   * @return Size of picktime in seconds.
   */

  double getDt();

  /** Tells in which state the chore is.
   *
   * @return Chore-state.
   */

  ChoreState getState();

  /** Set the chore state
   *
   * @param state Chore-state where the chore-changes.
   */

  void setState(ChoreState state);

private:

 
  int fret;
  unsigned char event;
  double start;
  double end;
  ChoreState state;
  double dt;
};

