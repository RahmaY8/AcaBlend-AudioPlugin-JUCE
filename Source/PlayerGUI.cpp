#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton , &muteButton , &PauseButton })
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

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}


void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(40, y, 80, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    muteButton.setBounds(340, y, 80, 40);//Salma
    PauseButton.setBounds(40, 70, 80, 40);//Rahma
    /*prevButton.setBounds(340, y, 80, 40);
    nextButton.setBounds(440, y, 80, 40);*/

    volumeSlider.setBounds(40, 120, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton && onLoadButton)
        onLoadButton();
    else if (button == &restartButton && onRestartButton)
        onRestartButton();
    else if (button == &stopButton && onStopButton)
        onStopButton();
    else if (button == &muteButton && onMuteToggle)//Salma
    {
        isMuted = !isMuted;
        muteButton.setButtonText(isMuted ? "Unmute" : "Mute");
        onMuteToggle(isMuted);
    }
    else if (button == &PauseButton && onPauseButton)//Rahma
        onPauseButton();

}
void PlayerGUI::updatePauseButtonText(bool isPaused)//Rahma
{
    PauseButton.setButtonText(isPaused ? "Continue" : "Pause");
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        // transportSource.setGain((float)slider->getValue());
        onVolumeChanged(slider->getValue());
}