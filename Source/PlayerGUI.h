#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener
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
	
private:
	PlayerAudio playerAudio;
	//GUI elements
	juce::TextButton loadButton{ "Load Files" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton stopButton{ "Stop" };
	juce::TextButton muteButton{ "Mute" }; //Salma
	bool isMuted = false;
	double previousVolume = 0.5;
	juce::TextButton PauseButton{ "Pause" }; //Rahma
	juce::TextButton ToStartButton{ " Go To Start" };
	juce::TextButton ToEndButton{ "Go To End" };
	juce::TextButton LoopButton{ "Loop" }; //Kenzy
	bool isLooping = false;
	juce::Slider volumeSlider;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)

};