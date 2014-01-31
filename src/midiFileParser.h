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

#include <vector>
#include "midiStream.h"
#include "midiEvent.h"
#include "chore.h"

/**
 * The Parser class, that contains needed information of given midi-file
 */

class MidiFileParser{

public:

  enum Difficulty
  {
    DIFFICULTY_SUPAEASY,
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_AMAZING
  };

  /** Constructor
   *
   * Creates a MidiFileParser which contains  information of given midifile.
   *
   * @param name Name of midifile, which want to be read
   * @param difficulty How difficulty game is
   */
  
  MidiFileParser(std::string name, Difficulty difficulty);

  /** Destructor
   *
   */

  ~MidiFileParser();

  /** Tells header chunk name.
   *
   * @return Name of header chunk, should be (MThd).
   */ 

  std::string getHeaderChunkName();

  /** Tells the format of midifile.
   *
   * @return Format of midifile.
   */

  unsigned short getFormat();

  /** Tells how many tracks file contains.
   *
   * @return number of tracks in the midi-file.
   */

  unsigned short getNTracks();

  /** Tells the division of midi-file.
   *
   * @return Division of the midi-file
   */

  short getDivision();

  /** Tells how many bytes chunk contains.
   *
   * @return Number of bytes in the chunk.
   */

  unsigned long getChunkLength();

  /** Returns the noteon-noteoff events that midifile contains.
   *
   * @return Vector that contains noteon-noteoff midi-events.
   */

  std::vector<MidiEvent> getMidis();

  /** Returns the Chores that midifile contains.
   *
   * @return Vector that contains Chores.
   */

  std::vector<Chore> getChores();

private:

  int coutMidiEvent(unsigned char t,unsigned char *data,double time);
  int getAmount(unsigned char *data,int i);
  double getAbTime(int delta);
  void checkMidiEvent(unsigned char t,unsigned char* data,double abtime);
  void readMidiFile();
  void readTrack();
  void makeChores();
  MidiStream stream;
  std::string headername;
  unsigned long headerlength;
  unsigned short format;
  unsigned short ntracks;
  short division;
  unsigned char runningstatus;
  int tempo;
  int difficulty;
  std::vector<MidiEvent> midis;
  std::vector<Chore> chores;

};













