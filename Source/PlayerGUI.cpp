#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}

    //setSize(500, 250);
    //setAudioChannels(0, 2);


PlayerGUI::~PlayerGUI() {}

    void PlayerGUI::paint(juce::Graphics & g)
    {
        g.fillAll(juce::Colours::darkgrey);
    }


    void PlayerGUI::resized()
    {
        int y = 20;
        loadButton.setBounds(20, y, 100, 40);
        restartButton.setBounds(140, y, 80, 40);
        stopButton.setBounds(240, y, 80, 40);
        /*prevButton.setBounds(340, y, 80, 40);
        nextButton.setBounds(440, y, 80, 40);*/

        volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
    }     

    void PlayerGUI::buttonClicked(juce::Button* button)
    {
        if (button == &loadButton && onLoadButton)
            onLoadButton();
        else if (button == &restartButton && onRestartButton)
            onRestartButton();
        else if (button == &stopButton && onStopButton)
            onStopButton();
    }

    void PlayerGUI::sliderValueChanged(juce::Slider* slider)
    {
        if (slider == &volumeSlider )
           // transportSource.setGain((float)slider->getValue());
            onVolumeChanged(slider->getValue());
    }
