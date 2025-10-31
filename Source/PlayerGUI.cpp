#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
    juce::Logger::writeToLog("PlayerGUI constructed: " + juce::String((intptr_t)this));
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton , &muteButton , &PauseButton
              , &ToStartButton , &ToEndButton ,&LoopButton ,& setPointAButton,& setPointBButton,& clearLoopButton,& toggleABLoopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    //Labels for metadata // Rahma2
    addAndMakeVisible(nowPlayingLabel);
    nowPlayingLabel.setText("Now Playing: ", juce::dontSendNotification);
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Title: ", juce::dontSendNotification);
    addAndMakeVisible(artistLabel);
    artistLabel.setText("Artist: ", juce::dontSendNotification);
    addAndMakeVisible(durationLabel);
    durationLabel.setText("Duration: ", juce::dontSendNotification);
    addAndMakeVisible(formatLabel);
    //TrackList
    addAndMakeVisible(tracksLoaded);
    tracksLoaded.setModel(this);

	//Labels for AB Loop //Kenzy3
    formatLabel.setText("Format: ", juce::dontSendNotification);
    loopPointALabel.setText("A: --:--", juce::dontSendNotification);
    addAndMakeVisible(loopPointALabel);
    loopPointBLabel.setText("B: --:--", juce::dontSendNotification);
    addAndMakeVisible(loopPointBLabel);
    abLoopStatusLabel.setText("AB Loop: Off", juce::dontSendNotification);
    addAndMakeVisible(abLoopStatusLabel);

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

	// Speed slider //Salma2
	speedSlider.setRange(0.5, 2.0, 0.1);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	addAndMakeVisible(speedSlider);

    //Progress Slider  //Kenzy2
    progressSlider.setRange(0.0, 1.0, 0.001);
    progressSlider.setValue(0.0);
    progressSlider.addListener(this);
    progressSlider.setSliderStyle(juce::Slider::LinearBar);
    progressSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(progressSlider);

    progressLabel.setText("00:00", juce::dontSendNotification);
    addAndMakeVisible(progressLabel);

    startTimer(100);
}

PlayerGUI::~PlayerGUI() {
    juce::Logger::writeToLog("PlayerGUI destroyed: " + juce::String((intptr_t)this));

    stopTimer();

    onLoadButton = nullptr;
    onRestartButton = nullptr;
    onStopButton = nullptr;
    onMuteToggle = nullptr;
    onPauseButton = nullptr;
    onTostartButton = nullptr;
    onToEndButton = nullptr;
    onLooping = nullptr;
    onVolumeChanged = nullptr;
    onSpeedChanged = nullptr;
    onProgressChanged = nullptr;
    onProgressUpdate = nullptr;

    progressSlider.removeListener(this);
    volumeSlider.removeListener(this);
    speedSlider.removeListener(this);

    juce::Logger::writeToLog("PlayerGUI cleanup completed");

}

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
    muteButton.setBounds(340, y, 80, 40); // Salma

    PauseButton.setBounds(440, y, 80, 40); // Rahma 
    ToStartButton.setBounds(40, 70, 80, 40);
    ToEndButton.setBounds(140, 70, 80, 40);

    LoopButton.setBounds(240, 70, 80, 40); // Kenzy


    int x = 120; 
    setPointAButton.setBounds(40, x, 60, 30);
    setPointBButton.setBounds(110, x, 60, 30);
    clearLoopButton.setBounds(180, x, 70, 30);
    toggleABLoopButton.setBounds(260, x, 70, 30);

    loopPointALabel.setBounds(340, x, 70, 30);
    loopPointBLabel.setBounds(420, x, 70, 30);
    abLoopStatusLabel.setBounds(500, x, 100, 30);

    
    volumeSlider.setBounds(40, x + 40, getWidth() - 40, 30);
    speedSlider.setBounds(40, x + 80, getWidth() - 40, 30); // Salma2

    
    progressSlider.setBounds(40,x + 130, getWidth() - 120, 25); // Kenzy2
    progressLabel.setBounds(40, x + 155, 50, 20); 

    
    nowPlayingLabel.setBounds(40, x + 185, getWidth() - 80, 40);//Rahma2&3 
    titleLabel.setBounds(40, x + 235, getWidth() - 80, 40); 
    artistLabel.setBounds(40,x + 285, getWidth() - 80, 40); 
    durationLabel.setBounds(40, x + 335, getWidth() - 80, 40); 
    formatLabel.setBounds(40, x + 385, getWidth() - 80, 40); 
    tracksLoaded.setBounds(300, x + 185, 350, 250);
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
    else if (button == &LoopButton && onLooping) 
        onLooping();
    else if (button == &setPointAButton && onSetLoopPointA)
        onSetLoopPointA();
    else if (button == &setPointBButton && onSetLoopPointB)
        onSetLoopPointB();
    else if (button == &clearLoopButton && onClearLoopPoints)
        onClearLoopPoints();
    else if (button == &toggleABLoopButton && onToggleABLoop)
        onToggleABLoop();

}
void PlayerGUI::updatePauseButtonText(bool isPaused)//Rahma
{
    PauseButton.setButtonText(isPaused ? "Continue" : "Pause");
}

void PlayerGUI::updateLoopButton(bool isLooping) {
    LoopButton.setButtonText(isLooping ? "Looping" : "Loop");
}

juce::String PlayerGUI::formatTime(double seconds) //Kenzy2
{
    int minutes = (int)(seconds / 60);
    int secs = (int)seconds % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}

void PlayerGUI::updateProgress(double currentPosition, double totalLength)
{
    if (totalLength > 0.0)
    {
        double progress = currentPosition / totalLength;
        progressSlider.setValue(progress, juce::dontSendNotification);

        
        progressLabel.setText(formatTime(currentPosition), juce::dontSendNotification);
    }
}

void PlayerGUI::timerCallback()
{
    if (onProgressUpdate)
        onProgressUpdate();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        // transportSource.setGain((float)slider->getValue());
        onVolumeChanged(slider->getValue());
	else if (slider == &speedSlider) //Salma2
		onSpeedChanged(slider->getValue());
	else if (slider == &progressSlider ) // Kenzy2
       onProgressChanged(slider->getValue());
}

void PlayerGUI::updateMetaData(const juce::String& title, const juce::String& artist,//Rahma2
    const juce::String& duration, const juce::String& format)
{
    titleLabel.setText("Title: " + title, juce::dontSendNotification);
    artistLabel.setText("Artist: " + artist, juce::dontSendNotification);
    durationLabel.setText("Duration: " + duration, juce::dontSendNotification);
    formatLabel.setText("Format: " + format, juce::dontSendNotification);
}
void PlayerGUI::updateTrackList(const juce::StringArray& newtracks)//Rahma3
{
    tracknames = newtracks;
    tracksLoaded.updateContent();
}
void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent& event)
{
    if (onTrackSelected && row >= 0 && row < tracknames.size())
    {
        onTrackSelected(row); 
    }
}
int PlayerGUI::getNumRows()
{
    return tracknames.size();
}
void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::darkcyan);

    g.setColour(juce::Colours::cyan);
    g.drawText(tracknames[rowNumber], 10, 0, width - 10, height, juce::Justification::left);
}



void PlayerGUI::updateABLoopDisplay(double pointA, double pointB, bool active) //Kenzy3
{
    loopPointALabel.setText("A: " + formatTime(pointA), juce::dontSendNotification);
    loopPointBLabel.setText("B: " + formatTime(pointB), juce::dontSendNotification);

    if (active)
    {
        abLoopStatusLabel.setText("AB Loop: ON", juce::dontSendNotification);
        abLoopStatusLabel.setColour(juce::Label::textColourId, juce::Colours::green);
        toggleABLoopButton.setButtonText("AB Stop");
    }
    else
    {
        abLoopStatusLabel.setText("AB Loop: Off", juce::dontSendNotification);
        abLoopStatusLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        toggleABLoopButton.setButtonText("AB Loop");
    }

    setPointAButton.setToggleState(pointA >= 0, juce::dontSendNotification);
    setPointBButton.setToggleState(pointB >= 0, juce::dontSendNotification);
}