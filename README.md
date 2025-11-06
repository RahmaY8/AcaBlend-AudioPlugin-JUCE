# AcaBlend - Dual Audio Player Plugin

AcaBlend is a dual-track audio player plugin built with JUCE framework.

## Features

### 🎵 Dual Player Architecture
- Two independent audio players with separate controls
- Master start/stop controls for synchronized playback
- Individual activation/deactivation for each player

### 🎛️ Advanced Playback Controls
- **Transport Controls**: Play, pause, stop, skip forward/backward (10s increments)
- **Looping**: Standard loop and A-B loop functionality
- **Speed Control**: Variable playback speed (0.5x to 2.0x)
- **Position Control**: Precise seeking with progress slider
- **To Start/To End**: Quick navigation to beginning or end of track

### 📊 Visual Feedback
- **Waveform Display**: Real-time audio waveform visualization
- **Progress Tracking**: Visual progress bar with time display
- **AB Loop Visualization**: Clear display of loop points A and B
- **Marker System**: Visual markers on waveform for quick navigation

### 🎚️ Audio Processing
- **Volume Control**: Individual gain control for each player
- **Mute Function**: Instant mute/unmute with volume preservation
- **Resampling**: High-quality audio resampling for speed changes
- **Mixdown**: Automatic mixing of both audio streams when both players active

### 📋 Playlist Management
- **Track Lists**: Load multiple files into each player's playlist
- **Metadata Display**: Title, artist, duration, and format information
- **Quick Track Selection**: Click to switch between loaded tracks

### ⚡ Professional Features
- **Session Management**: Automatic save/restore of playback state
- **Marker System**: Add, remove, and jump to precise time markers
- **AB Loop Points**: Set precise loop sections with visual feedback
- **Real-time Updates**: Continuous progress and waveform updates

## Supported Formats
- WAV files
- MP3 files

## Usage

### Basic Operation
1. **Load Audio**: Click "Load Into Playlist" to add files to either player
2. **Start Playback**: Use individual play buttons or master controls
3. **Adjust Mix**: Use volume sliders to balance both players
4. **Navigate**: Use transport controls or click on waveform to seek

### Advanced Features
- **AB Looping**: Set points A and B, then activate AB loop
- **Markers**: Add markers for quick navigation to important sections
- **Speed Control**: Adjust playback speed without pitch alteration
- **Player Activation**: Toggle individual players on/off for flexible mixing

## File Structure
- `Main.cpp` - Application entry point and window management
- `MainComponent.h/cpp` - Main audio component and dual player management
- `PlayerAudio.h/cpp` - Core audio playback engine
- `PlayerGUI.h/cpp` - User interface components and controls

## Session Management
AcaBlend automatically saves your session to:
`Documents/AcaBlend_session.xml`

## This includes:
- Currently loaded files
- Playback positions
- Play/pause states
- Restored automatically on next launch

## Technical Details
- Built with JUCE framework
- Real-time audio processing


## License

AcaBlend is licensed under the MIT License. See the [LICENSE.md](LICENSE.md) file for full details.

### Third-Party Licenses
This project uses:
- JUCE Framework ([JUCE License](https://juce.com/legal/))
---

**AcaBlend** - Blend your ACAPELLA audio with precision and style!
