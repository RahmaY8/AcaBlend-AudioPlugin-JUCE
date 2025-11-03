#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"


class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::TableListBoxModel, 
	public juce::Timer
{
public:
	PlayerGUI();
	~PlayerGUI() override;

	void paint(juce::Graphics& g) override;
	void resized() override;
	void setVolumeSlider(double volume) { volumeSlider.setValue(volume); }

	int getNumRows() override;
	void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width,
		int height, bool rowIsSelected) override;
	void cellClicked(int rowNumber, int columnId, const juce::MouseEvent& event);
	void paintRowBackground( juce::Graphics& g, int rowNumber,
		int width, int height, bool rowIsSelected) override ;//Rahma3

	// Event handlers
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;

	void updateMetaData(const juce::String& title, const juce::String& artist,
		const juce::String& duration, const juce::String& format);

	// Callbacks
	std::function<void()> onLoadButton;
	
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
	std::function<juce::AudioThumbnail* ()> onGetAudioThumbnail; //Salma3
	std::function<bool()> onHasAudioLoaded; 

	juce::TableListBox tracksLoaded;
	juce::StringArray tracknames;
	juce::StringArray durations;
	void updateTrackList(const juce::StringArray& newtracks, const juce::StringArray& newdurations);
	void listBoxItemClicked(int row, const juce::MouseEvent& event) ;
	std::function<void(int trackIndex)> onTrackSelected;//Rahma3

	std::function<void()> activePlayer;	
	void updateActivePlayerButtonText(bool isactive);

private:
	
	juce::TextButton loadButton{ "Load Into Playlist" };
	
	juce::TextButton muteButton{ "Mute" }; //Salma
	bool isMuted = false;
	double previousVolume = 0.5;
	juce::TextButton PauseButton{ "||" }; //Rahma
	juce::TextButton ToStartButton{ "|<" };
	juce::TextButton ToEndButton{ ">|" };
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

	juce::TextButton activeplayerButton{"off"};
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)

};