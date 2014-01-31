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

#include "Sound.h"
#include "Util.h"

#include <stdexcept>
#include <map>
#include <utility>
#include <set>

#include <SFML/Audio.hpp>

namespace SilkyStrings
{

  namespace
  {
    // class SDLMixerLibrary
    // {
    //   public:

    //     SDLMixerLibrary ()
    //     {
    //       if (!refcount++)
    //       {
    //         if (SDL_Init (SDL_INIT_AUDIO))
    //           throw std::runtime_error (
    //               std::string ("Failed to initialize SDL: ") + SDL_GetError ());

    //         if (Mix_OpenAudio (44100, AUDIO_S16LSB, 2, 1536))
    //           if (Mix_OpenAudio (44100, AUDIO_S16MSB, 2, 1536))
    //             throw std::runtime_error ("Failed to open audio device");

    //         Mix_AllocateChannels (NUM_CHANNELS);
    //       }
    //     }

    //     ~SDLMixerLibrary ()
    //     {
    //       if (!--refcount)
    //       {
    //         Mix_CloseAudio ();
    //         SDL_Quit ();
    //       }
    //     }

    //   private:

    //     static unsigned refcount;
    // };

    class Sample
    {
      public:

      Sample (const ::std::string &file)
      {
	if(!sample.loadFromFile(file))
	  throw std::runtime_error ("Failed to load sample ");
	
	sound.setBuffer(sample);
        
      }
      
      void stopSound(){
	sound.stop();
      }

      void playSound(){
	sound.play();
      }

      void setVolume(double volume){
	sound.setVolume(volume);
      }
      
      ~Sample ()
      {
      }
      
      // operator Mix_Chunk * ()
        // {
        //   return sample;
        // }
      
      private:

      sf::SoundBuffer sample;
      sf::Sound sound;
    };
    
  }

  struct Sound::Private
  {
    std::map<std::string, boost::shared_ptr<Sample> > samples;
    static Private *instance;

    Private ()
    {
      if (instance)
        throw std::logic_error ("Only one instance allowed at a time!");
      
      instance = this;
    }

    ~Private ()
    {
      /* guarantee proper cleanup ordering */
      
      for(std::map<std::string, boost::shared_ptr<Sample> >::iterator i = samples.begin ();i!= samples.end(); ++i)
	i->second->stopSound();
      
      samples.clear ();
      
      instance = NULL;
    }
    
    
  };

  Sound::Private *
  Sound::Private::instance = NULL;

  Sound::Sound ()
    : priv (new Private ())
  {
  }

  void
  Sound::preload (const ::std::string &filename)
  {
    if (!priv->samples[filename])
      priv->samples[filename] = boost::shared_ptr<Sample> (new Sample (filename));
  }

  std::string
  Sound::play (const ::std::string &filename)
  {
    if (priv->samples.find (filename) == priv->samples.end ())
      preload (filename);
    
    priv->samples[filename]->playSound();
    return filename;
  }

  void
  Sound::set_volume (const ::std::string &to_mute, double volume)
  {
    if (volume < 0 || volume > 100)
      throw std::out_of_range ("Volume must be between 0 and 100");

    priv->samples[to_mute]->setVolume(volume);
  }

  // void
  // Sound::update ()
  // {
  //   std::set<Handle> to_erase;

  //   for (std::map<Handle, int>::iterator i = priv->playing_channels.begin ();
  //         i != priv->playing_channels.end (); i++)
  //   {
  //     if (!Mix_Playing (i->second))
  //       to_erase.insert (i->first);
  //   }

  //   for (std::set<Handle>::iterator i = to_erase.begin ();
  //       i != to_erase.end (); ++i)
  //   {
  //     priv->playing_channels.erase (*i);
  //   }
  // }
}


