/*
  ==============================================================================

    RompMenu.cpp
    Created: 23 Jul 2021 5:40:04pm
    Author:  ryand

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RompMenu.h"

//==============================================================================
RompMenu::RompMenu(SamplerAudioProcessor& p) : audioProcessor(p)
{
    setSize(200, 30);
    setLookAndFeel(&pulsarUIFeel);

    text.setText("Romples");
    text.setColour(juce::Colours::white);
    text.setJustification(juce::Justification::centred);
    text.setFont(juce::Font("Consolas", "Bold", 18.f), false);
    addAndMakeVisible(text);

    prepareArtists();
    prepareMenu();


}

RompMenu::~RompMenu()
{
    menu.clear();
}

void RompMenu::paint (juce::Graphics& g)
{

    g.setColour(juce::Colours::black);
    g.fillAll();
}

void RompMenu::resized()
{

}


void RompMenu::mouseDown(const juce::MouseEvent& e)
{
    int selection = menu.showAt(this) - 1; // item id's start at 1, but passing to a vector storign file paths at [0]

    audioProcessor.loadFromIndex(selection);
}

void RompMenu::prepareMenu()
{
    auto artists = audioProcessor.getArtistArray();
    menu.setLookAndFeel(&pulsarUIFeel);

    juce::ScopedPointer<juce::PopupMenu> artistsMenu = new juce::PopupMenu;

    int itemIndex = 1; // used to properly index artist romples without resetting in the artist loop

    for (size_t i = 0; i < artists.size(); i++)
    {
        juce::ScopedPointer<juce::PopupMenu> romplerMenu = new juce::PopupMenu;

        for (size_t j = 0; j < artists[i]->numberOfRomples(); j++)
        {
            romplerMenu->addItem(itemIndex, artists[i]->getRompleName(j));
            itemIndex++;
        }

        auto artist = artists[i];
        artistsMenu->addSubMenu(artist->getArtistName(), *romplerMenu);
    }

    menu.addSectionHeader("Rompler");
    menu.addSeparator();
    menu.addSubMenu("Artists", *artistsMenu);
    
}

