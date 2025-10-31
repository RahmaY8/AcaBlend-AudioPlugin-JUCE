#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"


class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::Timer
{
public:
	PlayerGUI();
	~PlayerGUI() override;
	
	void paint(juce::Graphics& g) override;
	void resized() override;
	void setVolumeSlider(double volume) { volumeSlider.setValue(volume); }

	// Event handlers
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;

	void updateMetaData(const juce::String& title, const juce::String& artist,
		const juce::String& duration, const juce::String& format);

	// Callbacks
	std::function<void()> onLoadButton;
	std::function<void()> onRestartButton;
	std::function<void()> onStopButton;
	std::function<void(bool)> onMuteToggle; //Salma
	std::function<void()> onPauseButton;	//Rahma
	void updatePauseButtonText(bool isPaused);
	std::function<void()> onTostartButton;
	std::function<void()> onToEndButton;
	std::function<void()> onLooping; //Kenzy
	void updateLoopButton(bool isLooping);
	std::function<void(double)> onVolumeChanged;
	std::function<void(double)> onSpeedChanged; //Salma2
	std::function<void(double)> onProgressChanged; //Kenzy2
	std::function<void()> onProgressUpdate;
	void updateProgress(double positionInSeconds, double totalLengthInSeconds);
	std::function<void()> onSetLoopPointA; //Kenzy3
	std::function<void()> onSetLoopPointB;
	std::function<void()> onClearLoopPoints;
	std::function<void()> onToggleABLoop;
	void updateABLoopDisplay(double pointA, double pointB, bool active);

private:
	//PlayerAudio playerAudio;
	//GUI elements
	juce::TextButton loadButton{ "Load Files" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton stopButton{ "Stop" };
	juce::TextButton muteButton{ "Mute" }; //Salma
	bool isMuted = false;
	double previousVolume = 0.5;
	juce::TextButton PauseButton{ "Pause" }; //Rahma
	juce::TextButton ToStartButton{ "Go To Start" };
	juce::TextButton ToEndButton{ "Go To End" };
	juce::TextButton LoopButton{ "Loop" }; //Kenzy
	bool isLooping = false;
	juce::Slider volumeSlider;
	juce::Slider speedSlider; //Salma2
	juce::Slider progressSlider; //Kenzy2
	juce::Label progressLabel;
	juce::String formatTime(double seconds);
	void timerCallback() override;

	juce::Label nowPlayingLabel; //Rahma2
	juce::Label titleLabel, artistLabel, durationLabel, formatLabel;

	juce::TextButton setPointAButton{ "Set A" }; //Kenzy3
	juce::TextButton setPointBButton{ "Set B" };
	juce::TextButton clearLoopButton{ "Clear AB" };
	juce::TextButton toggleABLoopButton{ "AB Loop" };
	juce::Label loopPointALabel;
	juce::Label loopPointBLabel;
	juce::Label abLoopStatusLabel;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)

};