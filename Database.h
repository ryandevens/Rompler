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
    Database() 
    { 
        initFiles(); 
    }
    ~Database() {}

    void initFiles()
    {
        auto artistFolders = juce::File("C:/ProgramData/Recluse-Audio/Rompler/").findChildFiles(1, true);

        for (int i = 0; i < artistFolders.size(); i++)
        {
            auto artist = new Artist(artistFolders[i].getFileName());

            auto romples = artistFolders[i].findChildFiles(juce::File::findFiles, true);

            for (int j = 0; j < romples.size(); j++)
            {
                filePaths.add(romples[j].getFullPathName());
                artist->addRomple(romples[j].getFileNameWithoutExtension());
                fileNames.add(romples[j].getFileNameWithoutExtension());
            }

            artists.add(artist);

        }
    }

    juce::String& getFilePathFromIndex(int index) 
    { 
        return filePaths.getReference(index); 
    }

    juce::String& getFileNameFromIndex(int index)
    {
        return fileNames.getReference(index);
    }


    
private:
    juce::OwnedArray<Artist> artists;
    juce::StringArray filePaths;
    juce::StringArray fileNames;

    friend class RompMenu;
};