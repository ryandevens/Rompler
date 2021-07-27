/*
  ==============================================================================

    Database.h
    Created: 27 Jul 2021 3:05:01pm
    Author:  ryand

  ==============================================================================
*/

#pragma once
#include "Artist.h"
#include <JuceHeader.h>
using namespace juce;
// This class is for intializing each artist and their romples.  This holds the array of file paths
// I wish I could get this to store file paths by artist, but returnign the selected index id from 
// the popup menu is simpler than searching for a key/value via the string of the menu seleciton
// See:  Rompler.h
class Database
{
public:
    Database() { loadFiles(); }
    ~Database() {}

    void loadFiles()
    {
        auto artistFolders = juce::File::getCurrentWorkingDirectory().findChildFiles(juce::File::findDirectories, true);

        for (int i = 0; i < artistFolders.size(); i++)
        {
            Artist artist(artistFolders[i].getFileName());

            auto romples = artistFolders[i].findChildFiles(juce::File::findFiles, true);

            for (int j = 0; j < romples.size(); j++)
            {
                filePaths.add(romples[j].getFullPathName());
                artist.addRomple(romples[j].getFileName().dropLastCharacters(4));
            }

            artists.add(&artist);

        }
    }

    juce::String& getFilePathFromIndex(int index) { return filePaths.getReference(index); }

    juce::OwnedArray<Artist>& getArtistArray() { return artists; }


private:
    juce::OwnedArray<Artist> artists;
    juce::StringArray filePaths;
    //juce::StringArray filePaths = {
    //    /* Devens */
    //    "../Resources/Descent.wav",
    //    "../Resources/Steady.wav",
    //    "../Resources/Rising.wav",
    //    /* Ashby */
    //    "../Resources/Stab Brass Blast.wav"
    //};

};