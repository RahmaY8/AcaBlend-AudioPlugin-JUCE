#include "MainComponent.h"

MainComponent::MainComponent()
{

    //DBG("MainComponent constructed");
    addAndMakeVisible(playerGUI1);
    addAndMakeVisible(playerGUI2);

        // Connect GUI callbacks
        playerGUI1.onLoadButton = [this] { loadAudioFile(1); };
        playerGUI1.onVolumeChanged = [this](double volume) { playerAudio1.setGain((float)volume); };
        playerGUI1.onSpeedChanged = [this](double speed) { playerAudio1.setspeed((float)speed); };//Salma2
        playerGUI1.onMuteToggle = [this](bool isMuted) { playerAudio1.mute(isMuted);//Salma
        if (isMuted) playerGUI1.setVolumeSlider(0.0);
        else playerGUI1.setVolumeSlider(0.5); };

        playerGUI1.onPauseButton = [this] {playerAudio1.Pause_Continue(); //Rahma
        playerGUI1.updatePauseButtonText(playerAudio1.isPaused()); };
        playerGUI1.onTostartButton = [this] { playerAudio1.ToStart(); };
        playerGUI1.onToEndButton = [this] { playerAudio1.ToEnd(); };
        playerGUI1.onLooping = [this] {playerAudio1.Loop(); playerGUI1.updateLoopButton(playerAudio1.isLooped()); }; //Kenzy
        playerGUI1.onSkipForward = [this] { playerAudio1.skipForward(10.0); }; //Salma bonus
        playerGUI1.onSkipBackward = [this] { playerAudio1.skipBackward(10.0); };
        playerGUI1.activePlayer = [this] {playerAudio1.toggleActive();
        playerGUI1.updateActivePlayerButtonText(playerAudio1.isActive()); };

    playerGUI1.onProgressChanged = [this](double progress) { //Kenzy2
        double newPosition = progress * playerAudio1.getLengthInSeconds();
        playerAudio1.setPosition(newPosition);
        };
    playerGUI1.onProgressUpdate = [this]() {
        double currentPos = playerAudio1.getCurrentPosition();
        double totalLength = playerAudio1.getLengthInSeconds();
        playerGUI1.updateProgress(currentPos, totalLength);
        };
	playerGUI1.onGetAudioThumbnail = [this]() { return playerAudio1.getAudioThumbnail(); };//Salma3
	playerGUI1.onHasAudioLoaded = [this]() { return playerAudio1.hasAudioLoaded(); };//Salma3
	

    playerGUI1.onSetLoopPointA = [this] { playerAudio1.setLoopPointA(); }; //Kenzy3
    playerGUI1.onSetLoopPointB = [this] { playerAudio1.setLoopPointB(); };
    playerGUI1.onClearLoopPoints = [this] { playerAudio1.clearLoopPoints(); };
    playerGUI1.onToggleABLoop = [this] { playerAudio1.toggleABLoop(); };

    playerAudio1.onABLoopChanged = [this](double pointA, double pointB, bool active) {
        playerGUI1.updateABLoopDisplay(pointA, pointB, active);
        };

    playerAudio1.MetadataLoaded = [this](const juce::String& title, const juce::String& artist, //Rahma2
        const juce::String& duration, const juce::String& format) {
            playerGUI1.updateMetaData(title, artist, duration, format);
        };

        playerGUI1.onTrackSelected = [this](int trackindx) {//Rahma3
            if (trackindx < loadedFiles1.size())
            {
                playerAudio1.LoadFile(loadedFiles1[trackindx], true);
                playerAudio1.start();
                playerGUI1.updatePauseButtonText(false);
            }
            };

            playerGUI2.onLoadButton = [this] { loadAudioFile(2); };
            playerGUI2.onVolumeChanged = [this](double volume) { playerAudio2.setGain((float)volume); };
            playerGUI2.onSpeedChanged = [this](double speed) { playerAudio2.setspeed((float)speed); };//Salma2
            playerGUI2.onMuteToggle = [this](bool isMuted) { playerAudio2.mute(isMuted);//Salma
            if (isMuted) playerGUI2.setVolumeSlider(0.0);
            else playerGUI2.setVolumeSlider(0.5); };
            playerGUI2.onPauseButton = [this] {playerAudio2.Pause_Continue(); //Rahma
            playerGUI2.updatePauseButtonText(playerAudio2.isPaused()); };
            playerGUI2.onTostartButton = [this] { playerAudio2.ToStart(); };
            playerGUI2.onToEndButton = [this] { playerAudio2.ToEnd(); };
            playerGUI2.onLooping = [this] {playerAudio2.Loop(); playerGUI2.updateLoopButton(playerAudio2.isLooped()); }; //Kenzy
            playerGUI2.onSkipForward = [this] { playerAudio2.skipForward(10.0); }; //Salma bonus
            playerGUI2.onSkipBackward = [this] { playerAudio2.skipBackward(10.0); };
            playerGUI2.activePlayer = [this] {playerAudio2.toggleActive();
            playerGUI2.updateActivePlayerButtonText(playerAudio2.isActive()); };

            playerGUI2.onProgressChanged = [this](double progress) { //Kenzy2
                double newPosition = progress * playerAudio2.getLengthInSeconds();
                playerAudio2.setPosition(newPosition);
                };
            playerGUI2.onProgressUpdate = [this]() {
                double currentPos = playerAudio2.getCurrentPosition();
                double totalLength = playerAudio2.getLengthInSeconds();
                playerGUI2.updateProgress(currentPos, totalLength);
                };
            playerGUI2.onGetAudioThumbnail = [this]() { return playerAudio2.getAudioThumbnail(); };//Salma3
            playerGUI2.onHasAudioLoaded = [this]() { return playerAudio2.hasAudioLoaded(); };//Salma3


            playerGUI2.onSetLoopPointA = [this] { playerAudio2.setLoopPointA(); }; //Kenzy3
            playerGUI2.onSetLoopPointB = [this] { playerAudio2.setLoopPointB(); };
            playerGUI2.onClearLoopPoints = [this] { playerAudio2.clearLoopPoints(); };
            playerGUI2.onToggleABLoop = [this] { playerAudio2.toggleABLoop(); };

            playerAudio2.onABLoopChanged = [this](double pointA, double pointB, bool active) {
                playerGUI2.updateABLoopDisplay(pointA, pointB, active);
                };

            playerAudio2.MetadataLoaded = [this](const juce::String& title, const juce::String& artist, //Rahma2
                const juce::String& duration, const juce::String& format) {
                    playerGUI2.updateMetaData(title, artist, duration, format);
                };

            playerGUI2.onTrackSelected = [this](int trackindx) {//Rahma3
                if (trackindx < loadedFiles2.size())
                {
                    playerAudio2.LoadFile(loadedFiles2[trackindx], true);
                    playerAudio2.start();
                    playerGUI2.updatePauseButtonText(false);
                }
                };
    setSize(1400, 550);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    /*DBG("MainComponent destruction started");

    playerGUI.stopTimer();

    playerGUI.onLoadButton = nullptr;
    playerGUI.onVolumeChanged = nullptr;
    playerGUI.onSpeedChanged = nullptr;
    playerGUI.onMuteToggle = nullptr;
    playerGUI.onPauseButton = nullptr;
    playerGUI.onTostartButton = nullptr;
    playerGUI.onToEndButton = nullptr;
    playerGUI.onLooping = nullptr;
    playerGUI.onProgressChanged = nullptr;
    playerGUI.onProgressUpdate = nullptr;

    playerAudio1.stop();
    playerAudio2.stop();*/

    shutdownAudio();

   // DBG("MainComponent destruction completed");
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    playerAudio2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    bool player1Active = playerAudio1.isActive();
    bool player2Active = playerAudio2.isActive();

    if (player1Active && player2Active)
    {
        juce::AudioBuffer<float> tempBuffer1(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
        juce::AudioBuffer<float> tempBuffer2(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);

        juce::AudioSourceChannelInfo info1(&tempBuffer1, 0, bufferToFill.numSamples);
        juce::AudioSourceChannelInfo info2(&tempBuffer2, 0, bufferToFill.numSamples);

        playerAudio1.getNextAudioBlock(info1);
        playerAudio2.getNextAudioBlock(info2);
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto* output = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
            auto* data1 = tempBuffer1.getReadPointer(channel);
            auto* data2 = tempBuffer2.getReadPointer(channel);

            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                output[sample] = data1[sample] + data2[sample];
            }
        }
    }
    else if (player1Active)
    { 
        playerAudio1.getNextAudioBlock(bufferToFill);
    }
    else if (player2Active)
    {
        playerAudio2.getNextAudioBlock(bufferToFill);
    }
}

void MainComponent::releaseResources()
{
    playerAudio1.releaseResources();
    playerAudio2.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    playerGUI1.setBounds(area.removeFromLeft(getWidth() / 2));
    playerGUI2.setBounds(area);
}

void MainComponent::loadAudioFile(int playernumber)
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Select an audio file for player "+juce::String(playernumber),
        juce::File{},
        "*.wav;*.mp3");

    fileChooser->launchAsync(
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::canSelectMultipleItems, [this, playernumber](const juce::FileChooser& fc)
        {
            auto results = fc.getResults();

            juce::StringArray tracknames;
            juce::StringArray durations;
            for (auto& file : results)  // Loop through each file
            {
                if (file.existsAsFile())
                {
                    if (playernumber == 1) {
                        playerAudio1.LoadFile(file, false);  // Use playerAudio1
                        durations.add(playerAudio1.getLastDuration());
                        loadedFiles1.add(file);
                    }
                    else
                    {
                        playerAudio2.LoadFile(file, false);  // Use playerAudio2  
                        durations.add(playerAudio2.getLastDuration());
                        loadedFiles2.add(file);
                    }
   
                    tracknames.add(file.getFileName());  // Add to display list
                    
                    
                }
            }
            if(playernumber == 1)
                playerGUI1.updateTrackList(tracknames, durations);
            else 
                playerGUI2.updateTrackList(tracknames, durations);
        });
}
