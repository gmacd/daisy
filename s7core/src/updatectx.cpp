#include "updatectx.h"

#include "common.h"

namespace s7 {
    
    void UpdateContext::Init()
    {
        _appStartTime = bx::getHPCounter();
        _lastTime = bx::getHPCounter();
    }
    
    void UpdateContext::Update()
    {
        auto now = bx::getHPCounter();
        auto frameTime = now - _lastTime;
        _lastTime = now;
        
        auto freq = double(bx::getHPFrequency());
        _elapsedTimeSeconds = (float)(double(now - _appStartTime) / freq);
        
        _frameTimeSeconds = double(frameTime) / freq;
    }
    
}
