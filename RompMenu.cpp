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
RompMenu::RompMenu(RomplerAudioProcessor& p) : audioProcessor(p), database()
{
    setLookAndFeel(&pulsarFeel);

    menuButton = std::make_unique<TextButton>("Romples");
    menuButton->addListener(this);
    addAndMakeVisible(menuButton.get());
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

    g.setColour(Colours::white);
    g.setFont(15.0f);
    auto textBounds = getLocalBounds().reduced(10, 10);
    g.drawFittedText(fileName, textBounds, Justification::topLeft, 1);


}

void RompMenu::resized()
{
    menuButton->setBoundsRelative(0.85f, 0.05f, 0.1f, 0.25f);
}


void RompMenu::mouseDown(const juce::MouseEvent& e)
{
    //int selection = menu.showAt(this) - 1; // item id's start at 1, but passing to a vector storign file paths at [0]
    //auto filePath = database.getFilePathFromIndex(selection);
    //audioProcessor.loadFile(filePath);
}

void RompMenu::buttonClicked(Button* b)
{
    if (b == menuButton.get())
    {
        auto parentScreen = getParentMonitorArea();
        auto menuArea = Rectangle<int>(getParentWidth() * 0.5f, parentScreen.getY(), getParentWidth() / 2, getParentHeight());
        int selection = menu.showMenu(PopupMenu::Options().withTargetScreenArea(menuArea));
        if (selection == 0)
        {
            menu.dismissAllActiveMenus();
        }
        if (selection > 0)
        {
            auto path = database.getFilePathFromIndex(selection - 1); // item id's start at 1, but passing to a vector storign file paths at [0]
            audioProcessor.loadFile(path);
            fileName = database.getFileNameFromIndex(selection - 1);
        }
    }
    repaint();
}

void RompMenu::prepareMenu()
{
    menu.setLookAndFeel(&pulsarFeel);

    auto menuArea = Rectangle<float>(getParentWidth() / 2, 0, getParentWidth() / 2, getParentHeight());
    juce::ScopedPointer<juce::PopupMenu> artistsMenu = new juce::PopupMenu();

    int itemIndex = 1; // used to properly index artist romples without resetting in the artist loop

    for (size_t i = 0; i < database.artists.size(); i++)
    {
        juce::ScopedPointer<juce::PopupMenu> romplerMenu = new juce::PopupMenu;

        for (size_t j = 0; j < database.artists[i]->numberOfRomples(); j++)
        {
            romplerMenu->addItem(itemIndex, database.artists[i]->getRompleName(j));
            itemIndex++;
        }

        auto artist = database.artists[i];
        artistsMenu->addSubMenu(artist->getArtistName(), *romplerMenu);
    }

    menu.addSectionHeader("Rompler");
    menu.addSeparator();
    menu.addSubMenu("Artists", *artistsMenu);
    
}

