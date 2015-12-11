#pragma once

#include <cstdlib>

namespace s7 {
    
    class UpdateContext
    {
    public:
        void Init();
        void Update();
        
        float ElapsedTimeSeconds() const { return _elapsedTimeSeconds; }
        float FrameTimeSeconds() const { return _frameTimeSeconds; }
        
    private:
        float _elapsedTimeSeconds;
        float _frameTimeSeconds;
        
        int64_t _appStartTime;
        int64_t _lastTime;
    };
    
}
