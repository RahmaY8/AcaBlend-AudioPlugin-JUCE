#include "MainComponent.h"

MainComponent::MainComponent()
{

    DBG("MainComponent constructed");
    addAndMakeVisible(playerGUI);

    // Connect GUI callbacks
    playerGUI.onLoadButton = [this] { loadAudioFile(); };
    playerGUI.onRestartButton = [this] { playerAudio.stop(); playerAudio.setPosition(0.0); playerAudio.start(); };
    playerGUI.onStopButton = [this] { playerAudio.stop(); playerAudio.setPosition(0.0); };
    playerGUI.onVolumeChanged = [this](double volume) { playerAudio.setGain((float)volume); };
    playerGUI.onSpeedChanged = [this](double speed) { playerAudio.setspeed((float)speed); };//Salma2
    playerGUI.onMuteToggle = [this](bool isMuted) { playerAudio.mute(isMuted);//Salma
    if (isMuted) playerGUI.setVolumeSlider(0.0);
    else playerGUI.setVolumeSlider(0.5); };
    playerGUI.onPauseButton = [this] {playerAudio.Pause_Continue(); //Rahma
    playerGUI.updatePauseButtonText(playerAudio.isPaused()); };
    playerGUI.onTostartButton = [this] { playerAudio.ToStart(); };
    playerGUI.onToEndButton = [this] { playerAudio.ToEnd(); };
    playerGUI.onLooping = [this] {playerAudio.Loop(); playerGUI.updateLoopButton(playerAudio.isLooped()); }; //Kenzy

    playerGUI.onProgressChanged = [this](double progress) { //Kenzy2
        double newPosition = progress * playerAudio.getLengthInSeconds();
        playerAudio.setPosition(newPosition);
        };
    playerGUI.onProgressUpdate = [this]() {
        double currentPos = playerAudio.getCurrentPosition();
        double totalLength = playerAudio.getLengthInSeconds();
        playerGUI.updateProgress(currentPos, totalLength);
        };
	playerGUI.onGetAudioThumbnail = [this]() { return playerAudio.getAudioThumbnail(); };//Salma3
	playerGUI.onHasAudioLoaded = [this]() { return loadedFiles.size() > 0; };//Salma3
	

    playerGUI.onSetLoopPointA = [this] { playerAudio.setLoopPointA(); }; //Kenzy3
    playerGUI.onSetLoopPointB = [this] { playerAudio.setLoopPointB(); };
    playerGUI.onClearLoopPoints = [this] { playerAudio.clearLoopPoints(); };
    playerGUI.onToggleABLoop = [this] { playerAudio.toggleABLoop(); };

    playerAudio.onABLoopChanged = [this](double pointA, double pointB, bool active) {
        playerGUI.updateABLoopDisplay(pointA, pointB, active);
        };

    playerAudio.MetadataLoaded = [this](const juce::String& title, const juce::String& artist, //Rahma2
        const juce::String& duration, const juce::String& format) {
            playerGUI.updateMetaData(title, artist, duration, format);
        };

    playerGUI.onTrackSelected = [this](int trackindx) {//Rahma3
        if (trackindx < loadedFiles.size())
        {
            playerAudio.LoadFile(loadedFiles[trackindx]);
            playerAudio.start();
        }
        };

    setSize(700, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    DBG("MainComponent destruction started");

    playerGUI.stopTimer();

    playerGUI.onLoadButton = nullptr;
    playerGUI.onRestartButton = nullptr;
    playerGUI.onStopButton = nullptr;
    playerGUI.onVolumeChanged = nullptr;
    playerGUI.onSpeedChanged = nullptr;
    playerGUI.onMuteToggle = nullptr;
    playerGUI.onPauseButton = nullptr;
    playerGUI.onTostartButton = nullptr;
    playerGUI.onToEndButton = nullptr;
    playerGUI.onLooping = nullptr;
    playerGUI.onProgressChanged = nullptr;
    playerGUI.onProgressUpdate = nullptr;

    playerAudio.stop();

    shutdownAudio();

    DBG("MainComponent destruction completed");
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    playerAudio.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    playerAudio.releaseResources();
}

void MainComponent::resized()
{
    playerGUI.setBounds(getLocalBounds());
}

void MainComponent::loadAudioFile()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Select an audio file...",
        juce::File{},
        "*.wav;*.mp3");

    fileChooser->launchAsync(
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles |
        juce::FileBrowserComponent::canSelectMultipleItems, [this](const juce::FileChooser& fc)
        {
            auto results = fc.getResults();

            juce::StringArray tracknames;
            loadedFiles.clear();
            for (auto& file : results)  // Loop through each file
            {
                if (file.existsAsFile())
                {
                    tracknames.add(file.getFileName());  // Add to display list
                    loadedFiles.add(file);
                }
            }
            playerGUI.updateTrackList(tracknames);
        });
}