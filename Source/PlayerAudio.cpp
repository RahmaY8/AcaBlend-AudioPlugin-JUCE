#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() : transportSource()
{
    formatManager.registerBasicFormats();
	// wrap the transport source in a resampling source to allow speed changes //Salma2
	resamplingSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false,2);
    /*transportSource.setSource(readerSource.get());*/ 
    juce::Logger::writeToLog("PlayerAudio constructed: " + juce::String((intptr_t)this));

}

PlayerAudio::~PlayerAudio()
{
    juce::Logger::writeToLog("PlayerAudio destroyed: " + juce::String((intptr_t)this));

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
	resamplingSource->prepareToPlay(samplesPerBlockExpected, sampleRate);//Salma2

	// DSP for Pitch correction //Salma22
	/*spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlockExpected;
	spec.numChannels = 2;

	pitchShifter.prepare(spec);
	pitchShifter.setPitchSemitones(0.0f);*/

}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

	 resamplingSource->getNextAudioBlock(bufferToFill); //Salma2
	/*transportSource.getNextAudioBlock(bufferToFill);*/ //Salma2
	 // Pitch correction process //Salma22
	/* juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);
	 juce::dsp::ProcessContextReplacing<float> context(block);
	 pitchShifter.process(context);*/


    if (isLooping && transportSource.hasStreamFinished())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }

	if (ABLoopActive && transportSource.getCurrentPosition() >= loopPointB) //Kenzy3
    {
        transportSource.setPosition(loopPointA);
    }
}

void PlayerAudio::releaseResources()
{
    juce::Logger::writeToLog("PlayerAudio releasing resources: " + juce::String((intptr_t)this));
    transportSource.releaseResources();
    if (resamplingSource != nullptr)
        resamplingSource->releaseResources(); //salma2
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

void PlayerAudio::setspeed(float speed) //Salma2
{
	playbackSpeed = juce::jlimit(0.5f, 2.0f, speed);
    resamplingSource->setResamplingRatio(playbackSpeed);

	float semitones = 12.0f * std::log2(1.0f / playbackSpeed);
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
			resamplingSource->setResamplingRatio(playbackSpeed);//Salma2

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

juce::String PlayerAudio::formatTime(double seconds) //Kenzy3
{
    int minutes = (int)(seconds / 60);
    int secs = (int)seconds % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}


void PlayerAudio::setLoopPointA()  //Kenzy3
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
