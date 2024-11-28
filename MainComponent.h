#pragma once

#include <JuceHeader.h>

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // 볼륨 조절 슬라이더
    juce::Slider volumeSlider;
    juce::Label volumeLabel;

    // 오디오 장치 선택 관련
    juce::ComboBox inputDeviceSelector;
    juce::ComboBox outputDeviceSelector;
    juce::Label inputLabel, outputLabel;

    juce::AudioDeviceManager audioDeviceManager;

    void setupAudioDeviceManager();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
