
#pragma once
#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
#include <vector>


class ofApp : public ofBaseApp{
    
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;
    
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
    // For drawing
    float waveform[4096]; //make this bigger, just in case
    int waveIndex;
    
    ofSoundStream soundStream;
    
    /* ofxMaxi*/
    void audioIn(ofSoundBuffer& input) override; // not used in this example
    void audioOut(ofSoundBuffer& output) override;
    
    
    maxiOsc oscillator[3];
    
    int sampleRate;
    int bufferSize;
    
    std::vector<float> currentFreq;
    maxiFFT myFFT;
    
    ofParameter<float> frequency;
    ofParameter<float> amplitude;
    ofxPanel gui;
    
    
    float first, second, third, fourth;
    
    ofPath path;
    
    maxiFilter filter;
    
    
    vector<float> inputBuffer;
};
