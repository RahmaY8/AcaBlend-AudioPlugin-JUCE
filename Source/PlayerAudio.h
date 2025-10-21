#pragma once
#include<JuceHeader.h>

class PlayerAudio : public juce::AudioSource {
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    bool LoadFile(const juce::File& file);
    void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    void mute(bool shouldMute);//Salma
    void Pause_Continue();//Rahma
    bool isPaused() const { return paused; }

    //double GetPositin() const;
    //double GetLength() const;

private:
    bool paused = false;//Rahma
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    bool muted = false;
    float previousGain = 0.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};