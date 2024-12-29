#include "MainComponent.h"

MainComponent::MainComponent()
{
    // 입력/출력 장치 선택 박스 설정
    addAndMakeVisible(inputDeviceSelector);
    inputDeviceSelector.addListener(this);
    inputDeviceLabel.setText("Input Device", juce::dontSendNotification);
    addAndMakeVisible(inputDeviceLabel);

    addAndMakeVisible(outputDeviceSelector);
    outputDeviceSelector.addListener(this);
    outputDeviceLabel.setText("Output Device", juce::dontSendNotification);
    addAndMakeVisible(outputDeviceLabel);

    // 볼륨 슬라이더 설정
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(volumeSlider);

    // 오디오 장치 초기화
    setAudioChannels(2, 2); // 기본 입력/출력 채널 설정
    populateDeviceList();   // 장치 목록 초기화
    setSize(400, 300);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

// 입력 및 출력 장치 목록 가져오기
void MainComponent::populateDeviceList()
{
    auto& deviceManager = this->deviceManager; // JUCE의 deviceManager 사용
    auto& availableTypes = deviceManager.getAvailableDeviceTypes();

    for (auto* type : availableTypes)
    {
        type->scanForDevices();

        auto inputDevices = type->getDeviceNames(true);  // 입력 장치 목록
        auto outputDevices = type->getDeviceNames(false); // 출력 장치 목록

        for (const auto& deviceName : inputDevices)
            inputDeviceSelector.addItem(deviceName, inputDeviceSelector.getNumItems() + 1);

        for (const auto& deviceName : outputDevices)
            outputDeviceSelector.addItem(deviceName, outputDeviceSelector.getNumItems() + 1);
    }
}

// 장치 선택 콤보 박스 변경 이벤트
void MainComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &inputDeviceSelector || comboBoxThatHasChanged == &outputDeviceSelector)
    {
        auto inputDevice = inputDeviceSelector.getText();
        auto outputDevice = outputDeviceSelector.getText();
        setAudioDevice(inputDevice, outputDevice);
    }
}

// 장치 설정
void MainComponent::setAudioDevice(const juce::String& inputDeviceName, const juce::String& outputDeviceName)
{
    auto* currentDevice = deviceManager.getCurrentAudioDevice();

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    if (currentDevice)
        deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = inputDeviceName;
    setup.outputDeviceName = outputDeviceName;

    // 반환 타입이 juce::String인 경우
    auto result = deviceManager.setAudioDeviceSetup(setup, true);

    if (result.isNotEmpty()) // 결과가 비어 있지 않으면 오류 발생
    {
        juce::Logger::writeToLog("Failed to set audio device: " + result);
    }
    else
    {
        juce::Logger::writeToLog("Audio device set successfully.");
    }
}


// 오디오 콜백
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::Logger::writeToLog("Audio initialized. Sample rate: " + juce::String(sampleRate));
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* inputData = bufferToFill.buffer->getArrayOfReadPointers();
    auto* outputData = bufferToFill.buffer->getArrayOfWritePointers();
    auto numSamples = bufferToFill.numSamples;
    auto volume = static_cast<float>(volumeSlider.getValue());

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        if (inputData[channel] && outputData[channel])
        {
            for (int sample = 0; sample < numSamples; ++sample)
                outputData[channel][sample] = inputData[channel][sample] * volume;
        }
    }
}

void MainComponent::releaseResources()
{
    juce::Logger::writeToLog("Audio resources released.");
}

// GUI 배경
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawText("Guitar Input to Output Example", getLocalBounds(), juce::Justification::centred, true);
}

// GUI 레이아웃
void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(10);
    inputDeviceLabel.setBounds(area.removeFromTop(20));
    inputDeviceSelector.setBounds(area.removeFromTop(30));
    outputDeviceLabel.setBounds(area.removeFromTop(20));
    outputDeviceSelector.setBounds(area.removeFromTop(30));
    volumeSlider.setBounds(area.reduced(20));
}
