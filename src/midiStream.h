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

#include <iostream>
#include <fstream>
#include <string>

/**
 * The MidiStream class reads midifile for MidiFileParser.
 */

class MidiStream{

public:

  /** Constructor.
   *
   * Creates MidiStream object that reads given midiFile.
   *
   * @param file MidiFile to be read
   */

  MidiStream(std::string file);

  /** Destructor.
   *
   * Closes file_stream
   */

  ~MidiStream();

  /** Reads chunk header name (MTrk or MThd)
   *
   * @return chunk header name
   */

  std::string getChunkName();

  /** Reads how many bytes chunk holds
   *
   * @return number of bytes chunk contains
   */
  
  unsigned long getChunkLength();

  /** Reads which format the given midiFile is.
   *
   * @return 0 ,1 or 2
   */

  unsigned short getFormat();

  /** Reads how many tracks the file contains.
   *
   * @return number of tracks in the midi-file.
   */ 

  unsigned short getNTracks();

  /** Reads the division of the midi-file.
   *
   * @return division
   */
 
  short getDivision();

  /** Reads variable-length value
   *
   * @return ridden variable length
   */

  unsigned long readVarLen();

  /** Reads variable-length value and updates how many bytes were ridden
   *
   * @param i Reference to variable that holds ridden bytes.
   * @return ridden variable-length
   */

  unsigned long readVarLen(int &i);

  /** Saves num bytes.
   *
   * @param bytes number of bytes to read.
   * @return Ridden bytes.
   */

  unsigned char *getData(int bytes);

  /** Reads Midi-Event type
   *
   * @return ridden type.
   */

  unsigned char getType();

  /** Reads Mtrk:event type
   *
   * @ return ridden event.
   */

  unsigned char getEvent();

  /** Returns character to midi-stream
   *
   * @param Character to be put back
   */
  void putback(unsigned char c);
 

private:

  std::ifstream fin;
  
};
