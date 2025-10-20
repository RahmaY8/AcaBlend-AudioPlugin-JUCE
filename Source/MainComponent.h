#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlayerGUI.h"
#include <vector>

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void resized() override;

private:
    PlayerAudio playerAudio;
    PlayerGUI playerGUI;
    //juce::Component player1;
    //void loadTrack(const juce::File& file);
    std::unique_ptr<juce::FileChooser> fileChooser;

    void loadAudioFile();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};