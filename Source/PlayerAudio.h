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
    //void activePlayerFunction();
    juce::String extractDurationOnly(const juce::File& file);
    juce::String getLastDuration() const { return lastDuration; }

    bool isPaused() const { return paused; }
    void toggleActive() { active = !active; }
    bool isActive() const { return active; }
    void ToStart();
    void ToEnd();
    void Loop(); //Kenzy
	void skipForward(double seconds = 10.0); //Salma bonus
	void skipBackward(double seconds = 10.0);
    bool isLooped() const { return isLooping; };
    void setspeed(float speed); //Salma2

    std::function<void(const juce::String& title, const juce::String& artist,
        const juce::String& duration, const juce::String& format)> MetadataLoaded;
    bool LoadFile(const juce::File& file, bool shouldUpdateMetadata = true);
    bool shouldUpdateMetadata = true;
    double getCurrentPosition() const { return transportSource.getCurrentPosition(); }
    double getLengthInSeconds() const { return transportSource.getLengthInSeconds(); }

    void setLoopPointA(); //Kenzy3
    void setLoopPointB();
    void clearLoopPoints();
    void toggleABLoop();
    bool isABLoopActive() const { return ABLoopActive; }
    double getLoopPointA() const { return loopPointA; }
    double getLoopPointB() const { return loopPointB; }
    juce::AudioThumbnail* getAudioThumbnail() { return audioThumbnail.get(); } //Salma3
	bool hasAudioLoaded() const { return readerSource != nullptr; } 

    std::function<void(double pointA, double pointB, bool active)> onABLoopChanged;
	


    //double GetPositin() const;
    //double GetLength() const;

private:
    bool paused = false;//Rahma
    bool active = false;
    juce::String lastDuration;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource; //Salma2
    bool muted = false; //Salma
    float previousGain = 0.5f;
    bool isLooping = false;
    double currentSampleRate = 44100.0;
    float playbackSpeed = 1.0f; //Salma2
    juce::String formatTime(double sec);
    

    double loopPointA = 0.0;  //Kenzy3
    double loopPointB = 0.0;
    bool ABLoopActive = false;
    bool hasPointA = false;
    bool hasPointB = false;
	juce::AudioFormatManager thumbnailFormatManager; //Salma3
	juce::AudioThumbnailCache thumbnailCache;
	std::unique_ptr<juce::AudioThumbnail> audioThumbnail; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};