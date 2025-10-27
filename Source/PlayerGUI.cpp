#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton , &muteButton , &PauseButton
                     , &ToStartButton , &ToEndButton ,&LoopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    //Labels for metadata
    addAndMakeVisible(nowPlayingLabel);
    nowPlayingLabel.setText("Now Playing: ", juce::dontSendNotification);
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Title: ", juce::dontSendNotification);
    addAndMakeVisible(artistLabel);
    artistLabel.setText("Artist: ", juce::dontSendNotification);
    addAndMakeVisible(durationLabel);
    durationLabel.setText("Duration: ", juce::dontSendNotification);
    addAndMakeVisible(formatLabel);
    formatLabel.setText("Format: ", juce::dontSendNotification);


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
    ToStartButton.setBounds(140, 70, 80, 40);
    ToEndButton.setBounds(240, 70, 80, 40);
    LoopButton.setBounds(340, 70, 80, 40);// Kenzy
    /*prevButton.setBounds(340, y, 80, 40);
    nextButton.setBounds(440, y, 80, 40);*/
    volumeSlider.setBounds(40, 120, getWidth() - 40, 30);

    nowPlayingLabel.setBounds(40, 200, 80, 40);
    titleLabel.setBounds(40, 250, 80, 40);
    artistLabel.setBounds(40, 300, 80, 40);
    durationLabel.setBounds(40, 350, 80, 40);
    formatLabel.setBounds(40, 400, 80, 40);
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
    else if (button == &ToStartButton && onTostartButton)
        onTostartButton();
    else if (button == &ToEndButton && onToEndButton)
        onToEndButton();
    else if (button == &LoopButton && onLooping) {
        onLooping();
    }

}
void PlayerGUI::updatePauseButtonText(bool isPaused)//Rahma
{
    PauseButton.setButtonText(isPaused ? "Continue" : "Pause");
}

void PlayerGUI::updateLoopButton(bool isLooping) {
    LoopButton.setButtonText(isLooping ? "Looping" : "Loop");
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        // transportSource.setGain((float)slider->getValue());
        onVolumeChanged(slider->getValue());
}
void PlayerGUI::updateMetaData(const juce::String& title, const juce::String& artist,
    const juce::String& duration, const juce::String& format)
{
    titleLabel.setText("Title: " + title, juce::dontSendNotification);
    artistLabel.setText("Artist: " + artist, juce::dontSendNotification);
    durationLabel.setText("Duration: " + duration, juce::dontSendNotification);
    formatLabel.setText("Format: " + format, juce::dontSendNotification);
}