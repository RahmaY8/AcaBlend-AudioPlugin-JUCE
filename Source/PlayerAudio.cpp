#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() : transportSource() , thumbnailCache(5) 
{
    formatManager.registerBasicFormats();
	thumbnailFormatManager.registerBasicFormats();
	audioThumbnail = std::make_unique<juce::AudioThumbnail>(512, thumbnailFormatManager, thumbnailCache);
    resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false, 2);
}

PlayerAudio::~PlayerAudio()
{
    transportSource.stop();

    if (resamplingSource != nullptr)
    {
        resamplingSource->releaseResources();
        resamplingSource.reset();
    }

    transportSource.setSource(nullptr);

    transportSource.releaseResources();

    readerSource.reset();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    resamplingSource->getNextAudioBlock(bufferToFill); 
    
    if (isLooping && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }

    if (ABLoopActive && transportSource.getCurrentPosition() >= loopPointB) 
    {
        transportSource.setPosition(loopPointA);
    }
}

void PlayerAudio::releaseResources()
{
    juce::Logger::writeToLog("PlayerAudio releasing resources: " + juce::String((intptr_t)this));
    transportSource.releaseResources();
    if (resamplingSource != nullptr)
        resamplingSource->releaseResources(); 
}

void PlayerAudio::start()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

void PlayerAudio::mute(bool shouldMute)
{
    if (shouldMute)
    {
        previousGain = transportSource.getGain();
        transportSource.setGain(0.0f);
        muted = true;
    }
    else
    {
        transportSource.setGain(previousGain);
        muted = false;
    }
}
void PlayerAudio::Pause_Continue()
{
    paused = ! paused;
    if (paused) {
        transportSource.stop();
    }
    else {
        transportSource.start();
    }
}

juce::String PlayerAudio::extractDurationOnly(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            double Duration = reader->lengthInSamples / reader->sampleRate;
            return formatTime(Duration);
        }
    }
    return "00:00";
}
void PlayerAudio::ToStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::ToEnd()
{
    if (readerSource != nullptr) {
        double wholeLength = transportSource.getLengthInSeconds();
        transportSource.setPosition(wholeLength - 1.0);
    }
}

void PlayerAudio::Loop() 
{
    isLooping = !isLooping;
    transportSource.setLooping(isLooping);
}

void PlayerAudio::skipForward(double seconds)
{
    double newPosition = transportSource.getCurrentPosition() + seconds;
    if (newPosition < transportSource.getLengthInSeconds())
    {
        transportSource.setPosition(newPosition);
    }
    else
    {
        transportSource.setPosition(transportSource.getLengthInSeconds());
    }
}

void PlayerAudio::skipBackward(double seconds)
{
    double newPosition = transportSource.getCurrentPosition() - seconds;
    if (newPosition > 0.0)
    {
        transportSource.setPosition(newPosition);
    }
    else
    {
        transportSource.setPosition(0.0);
    }
}


void PlayerAudio::setspeed(float speed)
{
    playbackSpeed = juce::jlimit(0.5f, 2.0f, speed);
    resamplingSource->setResamplingRatio(playbackSpeed);

    float semitones = 12.0f * std::log2(1.0f / playbackSpeed);
}
bool PlayerAudio::LoadFile(const juce::File& file, bool shouldUpdateMetadata )
{
    currentFilePath = file.getFullPathName();
    transportSource.stop();          
    paused = false;
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            currentSampleRate = reader->sampleRate;
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            readerSource->setLooping(isLooping);

            transportSource.setSource(readerSource.get(), 0, nullptr, currentSampleRate);
            resamplingSource->setResamplingRatio(playbackSpeed);

            //Extract MetaData
            juce::String format = file.getFileExtension().toUpperCase();
            juce::String title = reader->metadataValues.getValue("Title", file.getFileNameWithoutExtension());
            juce::String artist = reader->metadataValues.getValue("Artist", "Unknown Artist");
            double Duration = reader->lengthInSamples / reader->sampleRate;
            juce::String duration = formatTime(Duration);

            if (shouldUpdateMetadata && MetadataLoaded)
            {
                MetadataLoaded(title, artist, duration, format);
                audioThumbnail->clear();
                audioThumbnail->setSource(new juce::FileInputSource(file));
            }
           lastDuration = duration;

            return true;
        }
    }
    lastDuration = "00:00";
    return false;
}

//Task14
void PlayerAudio::addMarker(double time)
{
    markerTimes.add(time);
}

juce::String PlayerAudio::formatTime(double seconds)
{
    int minutes = (int)(seconds / 60);
    int secs = (int)seconds % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}


void PlayerAudio::setLoopPointA()
{
    loopPointA = transportSource.getCurrentPosition();
    hasPointA = true;

    if (hasPointA && hasPointB)
    {
        ABLoopActive = true;
        transportSource.setLooping(true);
    }
    if (onABLoopChanged)
        onABLoopChanged(loopPointA, loopPointB, ABLoopActive);
}

void PlayerAudio::setLoopPointB()
{
    loopPointB = transportSource.getCurrentPosition();
    hasPointB = true;

    if (hasPointA && loopPointB <= loopPointA)
    {
        loopPointB = loopPointA + 1.0;
    }


    if (hasPointA && hasPointB)
    {
        ABLoopActive = true;
        transportSource.setLooping(true);
    }


    if (onABLoopChanged)
        onABLoopChanged(loopPointA, loopPointB, ABLoopActive);
}

void PlayerAudio::clearLoopPoints()
{
    ABLoopActive = false;
    hasPointA = false;
    hasPointB = false;
    loopPointA = 0.0;
    loopPointB = 0.0;
    transportSource.setLooping(isLooping);

    if (onABLoopChanged)
        onABLoopChanged(loopPointA, loopPointB, ABLoopActive);
}

void PlayerAudio::toggleABLoop()
{
    if (hasPointA && hasPointB)
    {
        ABLoopActive = !ABLoopActive;
        transportSource.setLooping(ABLoopActive || isLooping);

        if (onABLoopChanged)
            onABLoopChanged(loopPointA, loopPointB, ABLoopActive);
    }
}