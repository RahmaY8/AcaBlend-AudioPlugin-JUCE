//#define //JUCE_ACCESSIBILITY 0
#include <JuceHeader.h>
#include "MainComponent.h"

struct PlayerAudioLeakDetector { JUCE_DECLARE_NON_COPYABLE(PlayerAudioLeakDetector) };
struct PlayerGUILeakDetector { JUCE_DECLARE_NON_COPYABLE(PlayerGUILeakDetector) };

// Our application class
class SimpleAudioPlayer : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Simple Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.1"; }

    void initialise(const juce::String&) override
    {
        //juce::Desktop::getInstance().setAccessibilityEnabled(false);
        // Create and show the main window
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    void shutdown() override
    {
        mainWindow = nullptr; // Clean up
    }

private:
    // The main window of the app
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                juce::Colours::lightgrey,
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true); // MainComponent = our UI + logic
            centreWithSize(1400, 550);
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    std::unique_ptr<MainWindow> mainWindow;
};

// This macro starts the app
START_JUCE_APPLICATION(SimpleAudioPlayer)