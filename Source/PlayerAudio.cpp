#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() : transportSource()
{
    formatManager.registerBasicFormats();
    transportSource.setSource(readerSource.get());
}

PlayerAudio::~PlayerAudio()
{
    releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);


    if (isLooping && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
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

void PlayerAudio::mute(bool shouldMute)//Salma
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
void PlayerAudio::Pause_Continue()//Rahma
{
    paused = !paused;
    if (paused) {
        transportSource.stop();
    }
    else {
        transportSource.start();
    }
}
void PlayerAudio::ToStart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::ToEnd()
{
    if (readerSource != nullptr) {
        double wholeLength = transportSource.getLengthInSeconds();
        transportSource.setPosition(wholeLength - 3.0);
    }
}

void PlayerAudio::Loop() //kenzy
{
    isLooping = !isLooping;
    transportSource.setLooping(isLooping);

}


bool PlayerAudio::LoadFile(const juce::File& file)
{
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
            //Extract MetaData

            juce::String format = file.getFileExtension().toUpperCase();
            juce::String title = reader->metadataValues.getValue("Title", file.getFileNameWithoutExtension());
            juce::String artist = reader->metadataValues.getValue("Artist", "Unknown Artist");
            double Duration = reader->lengthInSamples / reader->sampleRate;
            juce::String duration = formatTime(Duration);

            if (MetadataLoaded)
                MetadataLoaded(title, artist, duration, format);

            return true;
        }
    }
    return false;
}

juce::String PlayerAudio::formatTime(double seconds)
{
    int minutes = (int)(seconds / 60);
    int secs = (int)seconds % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}
