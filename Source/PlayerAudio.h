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
    void mute(bool shouldMute);
    void Pause_Continue();
    juce::String extractDurationOnly(const juce::File& file);
    juce::String getLastDuration() const { return lastDuration; }

    bool isPaused() const { return paused; }
    void toggleActive() { active = !active; }
    bool isActive() const { return active; }
    void ToStart();
    void ToEnd();
    void Loop(); 
	void skipForward(double seconds = 10.0); 
	void skipBackward(double seconds = 10.0);
    bool isLooped() const { return isLooping; };
    void setspeed(float speed); 

    std::function<void(const juce::String& title, const juce::String& artist,
        const juce::String& duration, const juce::String& format)> MetadataLoaded;
    bool LoadFile(const juce::File& file, bool shouldUpdateMetadata = true);
    bool shouldUpdateMetadata = true;
    double getCurrentPosition() const { return transportSource.getCurrentPosition(); }
    double getLengthInSeconds() const { return transportSource.getLengthInSeconds(); }

    void setLoopPointA(); 
    void setLoopPointB();
    void clearLoopPoints();
    void toggleABLoop();
    bool isABLoopActive() const { return ABLoopActive; }
    double getLoopPointA() const { return loopPointA; }
    double getLoopPointB() const { return loopPointB; }
    juce::AudioThumbnail* getAudioThumbnail() { return audioThumbnail.get(); } 
	bool hasAudioLoaded() const { return readerSource != nullptr; } 


    juce::String currentFilePath;
    juce::String getCurrentFilePath() const { return currentFilePath; }

    std::function<void(double pointA, double pointB, bool active)> onABLoopChanged;
	
    void addMarker(double time);
    juce::Array<double> getMarkerTimes() const { return markerTimes; };

private:
    //Task14
    juce::Array <double> markerTimes;
    juce::String lastDuration;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::ResamplingAudioSource> resamplingSource;
    juce::String formatTime(double sec);

    bool paused = false;
    bool active = false;
    bool muted = false;
    float previousGain = 0.5f;
    bool isLooping = false;
    double currentSampleRate = 44100.0;
    float playbackSpeed = 1.0f;
    double loopPointA = 0.0;
    double loopPointB = 0.0;
    bool ABLoopActive = false;
    bool hasPointA = false;
    bool hasPointB = false;

	juce::AudioFormatManager thumbnailFormatManager;
	juce::AudioThumbnailCache thumbnailCache;
	std::unique_ptr<juce::AudioThumbnail> audioThumbnail; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};