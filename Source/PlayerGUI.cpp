#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
    juce::Logger::writeToLog("PlayerGUI constructed: " + juce::String((intptr_t)this));
    // Add buttons
    for (auto* btn : { &loadButton , &muteButton , &PauseButton, &ToStartButton ,
                       &ToEndButton ,&LoopButton ,&setPointAButton,&setPointBButton,&clearLoopButton,
                       &toggleABLoopButton, &activeplayerButton })
    {
        btn->setColour(juce::TextButton::buttonColourId, juce::Colour(130 ,115,255));
        btn->addListener(this);
        addAndMakeVisible(btn);
        activeplayerButton.onClick = [this] {
            if (activePlayer)
                activePlayer();  // This calls MainComponent
            };
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
    tracksLoaded.getHeader().addColumn("Track", 1, 200); // Track names
    tracksLoaded.getHeader().setColour(1, juce::Colour(160, 160, 175));
    tracksLoaded.getHeader().addColumn("Duration", 2, 80);
    tracksLoaded.getHeader().setColour(2, juce::Colour(160, 160, 175));

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
    
    g.fillAll(juce::Colour(25, 25, 40));
    // Draw Waveform //Salma3
    int x = 120;
    int waveformY = x + 180;
    int waveformHeight = 80;
    juce::Rectangle<int> waveformArea(40, 20, 610, waveformHeight);
    g.setColour(juce::Colour(15,15,25));
    g.fillRoundedRectangle(waveformArea.toFloat(), 10.0f);

    if (onGetAudioThumbnail && onHasAudioLoaded)
    {
        auto* thumbnail = onGetAudioThumbnail();
        bool hasAudio = onHasAudioLoaded();

        if (thumbnail && hasAudio && thumbnail->getTotalLength() > 0.0)
        {
            g.setColour(juce::Colours::lightgrey);
            thumbnail->drawChannels(g, waveformArea, 0.0, thumbnail->getTotalLength(), 1.0f);

            double progress = progressSlider.getValue();
            int pointerX = waveformArea.getX() + (int)(progress * waveformArea.getWidth());

            g.setColour(juce::Colour(109,93,255));
            g.drawLine(pointerX, waveformArea.getY(), pointerX, waveformArea.getBottom(), 2.0f);
        }
        else
        {
            g.setColour(juce::Colours::white);
            g.drawText("No audio loaded", waveformArea, juce::Justification::centred);
        }
    }

    // Draw metadata table background and border
    int metaX = 360, metaY = 380, metaWidth = 270, metaHeight = 160;
    g.setColour(juce::Colour(40,40,60));
    g.fillRect(metaX, metaY, metaWidth, metaHeight);
    g.setColour(juce::Colour(160,160,175));
    g.drawRect(metaX, metaY, metaWidth, metaHeight, 1);

    // Draw table header
    g.setColour(juce::Colour(160,160,175));
    g.fillRect(metaX, metaY, metaWidth, 40);
    g.setColour(juce::Colours::black);
    g.drawText("Now Playing", metaX + 10, metaY, metaWidth - 20, 25, juce::Justification::left);

    // Draw row separators
    g.setColour(juce::Colours::grey);
    for (int i = 1; i < 4; i++)
    {
        g.drawLine(metaX, metaY + 40 * i, metaX + metaWidth, metaY + 40 * i, 1);
    }
    // Boarder for buttons
    g.setColour(juce::Colour(50, 50, 70));
    g.fillRoundedRectangle(50, 180, 580, 50, 10.0f);

    // Boarder for AB  
    g.setColour(juce::Colour(50, 50, 70));
    g.fillRoundedRectangle(50, 240, 580, 50, 10.0f);

    // Boarder for VolumeSlider
    g.setColour(juce::Colour(50, 50, 70));
    g.fillRoundedRectangle(50, 310, 280, 50, 10.0f);

    // Boarder for SpeedSlider
    g.setColour(juce::Colour(50, 50, 70));
    g.fillRoundedRectangle(340, 310, 290, 50, 10.0f);

    g.setColour(juce::Colour(50, 50, 70));
	g.drawRect(getLocalBounds(), 2);
	progressSlider.setColour(juce::Slider::trackColourId, juce::Colour(130,115,255));// Kenzy2
    tracksLoaded.setColour(juce::TableListBox::backgroundColourId, juce::Colour(40,40,60));
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(130, 115, 255));
    volumeSlider.setColour(juce::Slider::trackColourId, juce::Colour(130, 115, 255));
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(160,160,175));
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colour(160,160,175));
   
}



void PlayerGUI::resized()
{
    int y = 20;
    int x = 120;
    loadButton.setBounds(50, 380, 130, 30);
    muteButton.setBounds(60, 190, 80, 30); // Salma
    PauseButton.setBounds(300, 190, 80, 30); // Rahma 
    ToStartButton.setBounds(180, 190, 80, 30);
    ToEndButton.setBounds(420, 190, 80, 30);
    LoopButton.setBounds(540, 190, 80, 30); // Kenzy
    activeplayerButton.setBounds(250,380,80,30);

    setPointAButton.setBounds(60, 250, 60, 30);
    setPointBButton.setBounds(130, 250, 60, 30);
    clearLoopButton.setBounds(200, 250, 70, 30);
    toggleABLoopButton.setBounds(280, 250, 70, 30);

    loopPointALabel.setBounds(360, 250, 70, 30);
    loopPointBLabel.setBounds(440, 250, 70, 30);
    abLoopStatusLabel.setBounds(520, 250, 100, 30);


    volumeSlider.setBounds(60, 320, 270, 30);
    speedSlider.setBounds(350, 320, 280, 30); // Salma2


    progressSlider.setBounds(40, 120, 610, 20); // Kenzy2
    progressLabel.setBounds(40, 145, 50, 20);

    int waveformY = x + 180; // Salma3

    
    tracksLoaded.setBounds(50, 420, 280, 120); //Salma3
   
    


    int metaX = 360, metaY = 380, metaWidth = 310;

    
    titleLabel.setBounds(metaX + 10, metaY + 40, metaWidth - 20, 25);
    artistLabel.setBounds(metaX + 10, metaY + 80, metaWidth - 20, 25);
    formatLabel.setBounds(metaX + 10, metaY + 120, metaWidth - 20, 25);

}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton && onLoadButton)
        onLoadButton();
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
    PauseButton.setButtonText(isPaused ? "continue" : "||");
}

void PlayerGUI::updateLoopButton(bool isLooping) {
    LoopButton.setButtonText(isLooping ? "Looping" : "Loop");
}

void PlayerGUI::updateActivePlayerButtonText(bool active) {
    activeplayerButton.setButtonText(active ? "On" : "Off");
};

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
    repaint(); // To update the waveform display //Salma3
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        // transportSource.setGain((float)slider->getValue());
        onVolumeChanged(slider->getValue());
    else if (slider == &speedSlider) //Salma2
        onSpeedChanged(slider->getValue());
    else if (slider == &progressSlider) // Kenzy2
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
void PlayerGUI::updateTrackList(const juce::StringArray& newtracks, const juce::StringArray& newdurations)
{

    tracknames.addArray(newtracks);
    durations.addArray(newdurations);
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
void PlayerGUI::paintRowBackground(juce::Graphics& g, int rowNumber,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colour(130,115,255));

    else if (rowNumber % 2)
        g.fillAll(juce::Colour(130,115,225).withAlpha(0.1f));
}
void PlayerGUI::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::white);

    if (columnId == 1) // Track name column
        g.drawText(tracknames[rowNumber], 10, 0, width - 10, height, juce::Justification::left);
    else if (columnId == 2) // Duration column  
        g.drawText(durations[rowNumber], 0, 0, width, height, juce::Justification::centred);// Temporary

}
void PlayerGUI::cellClicked(int rowNumber, int columnId, const juce::MouseEvent& event)
{
    if (onTrackSelected && rowNumber < tracknames.size())
        onTrackSelected(rowNumber);
}



void PlayerGUI::updateABLoopDisplay(double pointA, double pointB, bool active) //Kenzy3
{
    loopPointALabel.setText("A: " + formatTime(pointA), juce::dontSendNotification);
    loopPointBLabel.setText("B: " + formatTime(pointB), juce::dontSendNotification);

    if (active)
    {
        abLoopStatusLabel.setText("AB Loop: ON", juce::dontSendNotification);
        abLoopStatusLabel.setColour(juce::Label::textColourId, juce::Colour(109,93,255));
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