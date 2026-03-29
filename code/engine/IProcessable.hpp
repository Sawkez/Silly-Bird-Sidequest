#pragma once

class IProcessable {
    public:
        virtual void Process(float delta) = 0;
};