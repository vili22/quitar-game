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

//original LauncherWindow.h
#include "moc_LauncherWindow.cpp"

#include "SongIterator.h"
#include "Key.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <QApplication>
#include <QFont>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QMessageBox>

#include <string>
#include <iostream>
#include <sstream>

#include <unistd.h>


Launcher::LauncherWindow::LauncherWindow()
: QWidget(0){

	setWindowTitle("SilkyStrings - launcher");
  setGeometry(100, 100, 370, 300);

  /* settingsGroup */
  QGroupBox *settingsGroup = new QGroupBox("Settings");
  QGridLayout *settingsLayout = new QGridLayout;
  settingsGroup->setLayout(settingsLayout);


	/* resolution */
  resolution = new QComboBox();
  resolution->addItem("800x600");
  resolution->addItem("1024x768");
  resolution->addItem("1600x1200");
	settingsLayout->addWidget(new QLabel("Resolution: "), 0, 0, Qt::AlignRight);
  settingsLayout->addWidget(resolution, 0, 1);
	
  /* fullscreen */
  fullscreen = new QCheckBox();
  fullscreen->setCheckState(Qt::Checked);
  settingsLayout->addWidget(new QLabel("Fullscreen: "), 1, 0, Qt::AlignRight);
  settingsLayout->addWidget(fullscreen, 1, 1);
  settingsLayout->setRowMinimumHeight(2, 10);

  /* difficulty */
  difficulty = new QComboBox();
  difficulty->addItem("Supaeasy");
  difficulty->addItem("Easy");
  difficulty->addItem("Medium");
  difficulty->addItem("Amazing");
  settingsLayout->addWidget(new QLabel("Difficulty: "), 3, 0, Qt::AlignRight);
  settingsLayout->addWidget(difficulty, 3, 1);
  settingsLayout->setRowMinimumHeight(4, 10);

  /* gameButtons */
	const char *button_strings[7] =  
	{
		"Fret 1: ",
		"Fret 2: ",
		"Fret 3: ",
		"Fret 4: ",
		"Fret 5: ",
		"Pick: ",
		"Quit: ",
	};
	const int defaultKeys[7] = 
	{
		SilkyStrings::KEY_F1,
		SilkyStrings::KEY_F2,
		SilkyStrings::KEY_F3,
		SilkyStrings::KEY_F4,
		SilkyStrings::KEY_F5,
		SilkyStrings::KEY_ENTER,
		SilkyStrings::KEY_ESC,
	};
	for(int i=0; i<7; i++){
		gameButtons[i] = new QComboBox();
		
    for(int j=0; j<SilkyStrings::KEY_NUMBER_OF; j++){
			gameButtons[i]->addItem(SilkyStrings::key_strings[j]);
		}

		gameButtons[i]->setCurrentIndex(defaultKeys[i]);
		
    settingsLayout->addWidget(new QLabel(button_strings[i]), i+5, 0, Qt::AlignRight);
		settingsLayout->addWidget(gameButtons[i], i+5,1);
	}



  /* songGroup */
	QGroupBox *songGroup = new QGroupBox("Song selection");
  QGridLayout *songLayout = new QGridLayout();
  songGroup->setLayout(songLayout);

  songList = new QListWidget;
	
	loadSongs();

  songLayout->addWidget(songList);


  /* buttonLayout */
  QHBoxLayout *buttonLayout = new QHBoxLayout;

  QPushButton *exitButton = new QPushButton("Exit");
  QPushButton *refreshButton = new QPushButton("Refresh");
  QPushButton *playButton = new QPushButton("Play");

  connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
  connect(refreshButton, SIGNAL(clicked()), this, SLOT(loadSongs()));
  connect(playButton, SIGNAL(clicked()), this, SLOT(play()));

  buttonLayout->addWidget(refreshButton);
  buttonLayout->addWidget(exitButton);
  buttonLayout->addWidget(playButton);


  /* mainLayout */
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addWidget(songGroup, 0,1);
  mainLayout->addLayout(buttonLayout, 1,0,1,2);
  mainLayout->addWidget(settingsGroup, 0,0);


  setLayout(mainLayout);
}


void Launcher::LauncherWindow::loadSongs(){
  songList->clear();
  boost::filesystem::path songPath("./resources/songs");

  if(boost::filesystem::exists(songPath) && boost::filesystem::is_directory(songPath)){		
		for(Launcher::SongIterator iter = Launcher::SongIterator(songPath); iter != Launcher::SongIterator(); ++iter){
			std::string filename=*iter;//newline
			songList->addItem((filename.substr(18,filename.length()-17)).c_str());//newline
			//songList->addItem((*iter).c_str());
		}
	}

  songList->setCurrentRow(0);
}

bool Launcher::LauncherWindow::checkValidity(){
	// same key used twice
	for(int i=0; i<7; i++){
		for(int j=i+1; j<7; j++){
      if(gameButtons[i]->currentIndex() == gameButtons[j]->currentIndex()){
				QMessageBox errorMsg(QMessageBox::Critical,
					"SilkyStrings - launcher",
					"Key conflict",
					QMessageBox::Ok);
				
        errorMsg.exec();
				
        return false;
			}
		}
	}

	// no song selected
	if(songList->currentRow() == -1){
		QMessageBox errorMsg(QMessageBox::Critical,
			"SilkyStrings - launcher",
			"No song selected",
			QMessageBox::Ok);
		
    errorMsg.exec();

		return false;
	}

	return true;
}


void Launcher::LauncherWindow::play(){
	if(!checkValidity()) return;

	// order: param[i] = {song fret1 fret2 fret3 fret4 fret5 pick quit width height fullscreen difficulty}
  std::ostringstream param[12];

	std::istringstream res(resolution->currentText().toStdString());
	int width, height;
	res >> width;
	res.ignore();
	res >> height;

  param[0] << songList->currentItem()->text().toStdString();
	for(int i=0; i<7; i++){
		param[i+1] << gameButtons[i]->currentIndex();
	}
  param[8] << width;
  param[9] << height;
  param[10] << (fullscreen->checkState() == Qt::Unchecked ? false : true);
  param[11] << difficulty->currentText().toStdString();
	
  for(int i=0;i<12;i++)
    std::cout<<param[i].str().c_str()<<"\n";

	// execute silkystring and exit launcher
  execl("quitargame",
    "quitargame",
    param[0].str().c_str(),
    param[1].str().c_str(),
    param[2].str().c_str(),
    param[3].str().c_str(),
    param[4].str().c_str(),
    param[5].str().c_str(),
    param[6].str().c_str(),
    param[7].str().c_str(),
    param[8].str().c_str(),
    param[9].str().c_str(),
    param[10].str().c_str(),
    param[11].str().c_str(),
    0);


	// if execution fails
	QMessageBox errorMsg(QMessageBox::Critical,
		"SilkyStrings launcher",
		"Couldn't launch SilkyStrings",
		QMessageBox::Ok);
	errorMsg.exec();
}



int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
	Launcher::LauncherWindow widget;
  widget.show();

  return app.exec();
}
