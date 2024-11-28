#include "MainComponent.h"

MainComponent::MainComponent()
{
    // 오디오 장치 설정 초기화
    setupAudioDeviceManager();

    // 볼륨 슬라이더 초기화
    volumeSlider.setRange(0.0, 1.0); // 볼륨 범위: 0.0 (음소거) ~ 1.0 (최대)
    volumeSlider.setValue(0.5);      // 기본값: 50%
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    volumeSlider.onValueChange = [this]() {
        volumeLabel.setText("Volume: " + juce::String(volumeSlider.getValue() * 100, 0) + "%", juce::dontSendNotification);
    };
    addAndMakeVisible(volumeSlider);

    // 볼륨 레이블 초기화
    volumeLabel.setText("Volume: 50%", juce::dontSendNotification);
    volumeLabel.setFont(juce::Font(15.0f));
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volumeLabel);

    // 입력 장치 선택 UI
    inputLabel.setText("Input Device:", juce::dontSendNotification);
    inputLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(inputLabel);
    addAndMakeVisible(inputDeviceSelector);

    // 출력 장치 선택 UI
    outputLabel.setText("Output Device:", juce::dontSendNotification);
    outputLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(outputLabel);
    addAndMakeVisible(outputDeviceSelector);

    setSize(500, 300);
}

MainComponent::~MainComponent()
{
    shutdownAudio(); // 오디오 장치 종료
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::Logger::writeToLog("Audio initialized. Sample rate: " + juce::String(sampleRate));
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1);

    float volume = static_cast<float>(volumeSlider.getValue());

    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        leftChannel[sample] *= volume;
        rightChannel[sample] *= volume;
    }
}

void MainComponent::releaseResources()
{
    juce::Logger::writeToLog("Audio resources released.");
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawText("Audio Device Example", getLocalBounds().removeFromTop(40), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto labelHeight = 20;

    inputLabel.setBounds(area.removeFromTop(labelHeight));
    inputDeviceSelector.setBounds(area.removeFromTop(labelHeight));

    outputLabel.setBounds(area.removeFromTop(labelHeight));
    outputDeviceSelector.setBounds(area.removeFromTop(labelHeight));

    volumeLabel.setBounds(area.removeFromTop(30));
    volumeSlider.setBounds(area.removeFromTop(40));
}

void MainComponent::setupAudioDeviceManager()
{
    // 오디오 장치 초기화 (입력 2개, 출력 2개)
    audioDeviceManager.initialise(2, 2, nullptr, true);

    // 오디오 장치 유형 가져오기
    auto& availableDevices = audioDeviceManager.getAvailableDeviceTypes(); // 참조 사용

    for (auto* deviceType : availableDevices)
    {
        if (deviceType != nullptr)
        {
            // 입력 장치 추가
            juce::StringArray inputDevices = deviceType->getDeviceNames(true); // true: 입력 장치 가져오기
            for (const auto& deviceName : inputDevices)
                inputDeviceSelector.addItem(deviceName, inputDeviceSelector.getNumItems() + 1);

            // 출력 장치 추가
            juce::StringArray outputDevices = deviceType->getDeviceNames(false); // false: 출력 장치 가져오기
            for (const auto& deviceName : outputDevices)
                outputDeviceSelector.addItem(deviceName, outputDeviceSelector.getNumItems() + 1);
        }
    }

    // 선택된 장치 출력 (디버깅)
    inputDeviceSelector.onChange = [this]() {
        juce::Logger::writeToLog("Selected Input Device: " + inputDeviceSelector.getText());
    };

    outputDeviceSelector.onChange = [this]() {
        juce::Logger::writeToLog("Selected Output Device: " + outputDeviceSelector.getText());
    };
}

