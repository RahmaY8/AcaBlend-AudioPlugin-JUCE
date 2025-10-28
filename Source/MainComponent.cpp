#include "MainComponent.h"

MainComponent::MainComponent()
{
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
    playerAudio.MetadataLoaded = [this](const juce::String& title, const juce::String& artist,
        const juce::String& duration, const juce::String& format) {
            playerGUI.updateMetaData(title, artist, duration, format);
        };
    setSize(500, 250);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
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
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                playerAudio.LoadFile(file);
                playerAudio.start();
            }
        });
}