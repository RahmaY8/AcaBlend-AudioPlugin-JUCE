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
	std::function<void(bool)> onMuteToggle;
	std::function<void(double)> onVolumeChanged;

private:
	PlayerAudio playerAudio;
	//GUI elements
	juce::TextButton loadButton{ "Load Files" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton stopButton{ "Stop" };
	juce::TextButton muteButton{ "Mute" };
	bool isMuted = false;
	double previousVolume = 0.5;
	juce::Slider volumeSlider;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)

};