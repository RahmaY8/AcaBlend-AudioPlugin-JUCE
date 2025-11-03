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
    PlayerAudio playerAudio1;
    PlayerAudio playerAudio2;
    PlayerGUI playerGUI1;
    PlayerGUI playerGUI2;
   // juce::Component player1;
    //juce::Component player2;
    //void loadTrack(const juce::File& file);
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::Array<juce::File> loadedFiles1;//Rahma3
    juce::Array<juce::File> loadedFiles2;

    void loadAudioFile(int playernumber);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};