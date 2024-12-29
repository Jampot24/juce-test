#pragma once

#include <JuceHeader.h>

class MainComponent : public juce::AudioAppComponent, 
                      public juce::ComboBox::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;

private:
    juce::ComboBox inputDeviceSelector; 
    juce::ComboBox outputDeviceSelector;
    juce::Label inputDeviceLabel;
    juce::Label outputDeviceLabel;

    juce::Slider volumeSlider;

    void populateDeviceList(); // 장치 목록 초기화
    void setAudioDevice(const juce::String& inputDeviceName, const juce::String& outputDeviceName);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
