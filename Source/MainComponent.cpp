#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(playerGUI);

    // Connect GUI callbacks
    playerGUI.onLoadButton = [this] { loadAudioFile(); };
    playerGUI.onRestartButton = [this] { playerAudio.start(); };
    playerGUI.onStopButton = [this] { playerAudio.stop(); playerAudio.setPosition(0.0); };
    playerGUI.onVolumeChanged = [this](double volume) { playerAudio.setGain((float)volume); };
    playerGUI.onMuteToggle = [this](bool isMuted)
        {
            if (isMuted)
                playerAudio.setGain(0.0f);
            else
                playerAudio.setGain((float)playerGUI.getCurrentVolume());
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
        ".wav;.mp3");

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