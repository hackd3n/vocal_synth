#include "ofApp.h"

//--------------------------------------------------------------

std::vector<float> fftFreqs(int fftSize, int sampleRate) {
    std::vector<float> theFreqs;
    for(int i = 0; i < fftSize; ++i){
        theFreqs.push_back((sampleRate/float(fftSize)) * i);
    }
    return theFreqs;
}


void ofApp::setup(){
    ofBackground(0,0,0);
    
    //Initialize the drawing variables
    for (int i = 0; i < ofGetWidth(); ++i) {
        waveform[i] = 0;
    }
    waveIndex = 0;
    
    c
    
    gui.setup("wave parameters");
    gui.add(frequency.set("frequency", 250, 0, 10000));
    gui.add(amplitude.set("amplitude", 0.1, 0., 1.0));
    
    
    sampleRate = 44100;
    bufferSize= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    // Setup ofSound
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.setInListener(this);
    auto devices = soundStream.getDeviceList();
    settings.setInDevice(devices[1]);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 1;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
    int fftSize = 1024;
    myFFT.setup(fftSize, fftSize, fftSize /2);
    
    
    currentFreq = fftFreqs(fftSize, sampleRate);
    
    
    inputBuffer.resize(bufferSize); // 1 channel
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gui.draw();
    /////////////// waveform
    ofTranslate(0, ofGetHeight()/2);
    ofSetColor(0, 255, 0);
    ofFill();
    ofDrawLine(0, 0, 1, waveform[1] * ofGetHeight()/2.); //first line
    for(int i = 1; i < (ofGetWidth() - 1); ++i) {
        ofDrawLine(i, waveform[i] * ofGetHeight()/2., i + 1, waveform[i+1] * ofGetHeight()/2.);
    }
    
    
    ofSetColor(255, 255, 0);
    int binWidth = (ofGetWidth()/myFFT.bins)*2;
    ofDrawLine(0, 0, ofGetWidth(), 0);
    
    int drawFreq = 0;
    int offset = 0;
    
    // define the custom shape using the path methods
    float x = ofGetWidth() / 2;
    float y = 0;
    float r = 50;
    path.moveTo(x + r, y); // move to the starting point
    for(size_t i = 0; i < myFFT.bins; ++i ){
        ofDrawRectangle(i*binWidth, 0, binWidth, myFFT.magnitudes[i] * -2.);
        
        int threshold = i%30;
        
        if (threshold == 0) {
            
            float sumOfSurounding = myFFT.magnitudes[i] + myFFT.magnitudes[i - 1] + myFFT.magnitudes[i -2 ] + myFFT.magnitudes[i - 3] + myFFT.magnitudes[i - 4] + myFFT.magnitudes[i -5] +
            myFFT.magnitudes[i] + myFFT.magnitudes[i + 1] + myFFT.magnitudes[i + 2] + myFFT.magnitudes[i + 3] + myFFT.magnitudes[i + 4] + myFFT.magnitudes[i + 5];
            ofDrawEllipse(i * binWidth, 200, sumOfSurounding/10, sumOfSurounding/10);
    
        }
        if(drawFreq <= 0 && myFFT.magnitudes[i] > 1.0){
            ofDrawBitmapString(currentFreq[i + 2], i * binWidth, 20 + offset);
            drawFreq = 5;
            offset += 15;
            
        }
        --drawFreq;
    }
    
        path.setFillColor(ofColor(0, 0, 255)); // set the fill color to blue
        


        path.close(); // close the shape
        
        path.draw(); // draw the shape
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer& input){
    std::size_t nChannels = input.getNumChannels();
    
    for (size_t i = 0; i < input.getNumFrames(); i++)
    {
        inputBuffer[i] = input[i];
        // handle input here
    }
}
//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output){
    std::size_t outChannels = output.getNumChannels();
    for (int i = 0; i < output.getNumFrames(); ++i){
        
        float inputSample = inputBuffer[i];
        float currentSample = inputSample;
        
        
        // using the audio input to vary the FM synthis -
        
        //output[i * outChannels] += oscillator[1].sinewave(frequency * oscillator[2].sinewave(0.01)) * std::abs(currentSample) * amplitude;
        //output[i * outChannels] += oscillator[1].sinewave(frequency * std::abs(currentSample))  * amplitude;
        
        // vocal synthesisor using the absolute value of the audio input as a amplitude modulator to create a robotic sound
        output[i * outChannels] += oscillator[1].sinewave(frequency) * std::abs(currentSample) * amplitude;
        
        output[i * outChannels] += (currentSample * amplitude);
        
        
        
        //output[i * outChannels] += currentSample * oscillator[1].sinewave(frequency * oscillator[2].sinewave(0.01)) * amplitude * 0.1;
        output[i * outChannels + 1] = output[i * outChannels];

        float wave = output[i * outChannels];
        myFFT.process(wave);
        //Hold the values so the draw method can draw them
        waveform[waveIndex] =  output[i * outChannels];
        
        
        if (waveIndex < (ofGetWidth() - 1)) {
            ++waveIndex;
        } else {
            waveIndex = 0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    std::cout << myFFT.bins << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

void ofApp::exit(){
    ofSoundStreamClose();
}
