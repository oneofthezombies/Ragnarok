#pragma once

#include <functional>

#include "time.h"

// 스킬 온오프 모드 추가하기
class CoolTimer
{
public:
    CoolTimer(float const& time = 0.1f, 
              std::function<void()> const& operation = []() {},
              int const& count = 0x7FFFFFFF)
        : _operation(operation)
        , _cool_time(time)
        , _cool_down(_cool_time)
        , _count(count)
        , _work(std::bind(&CoolTimer::work, this))
    {
    }

    bool operator()()
    {
        return _work();
    }

    void setCoolTime(float const& time)
    {
        _cool_time = time;
        _cool_down = _cool_time;
    }

    float getCoolDown() const
    {
        return _cool_down;
    }

private:
    bool work()
    {
        if (_count <= 0)
        {
            _work = std::bind(&CoolTimer::doNothing, this);
            return false;
        }

        _cool_down -= Time::DeltaTime();

        if (_cool_down <= 0.0f)
        {
            if (_count < 0x7FFFFFFF)
                _count--;

            _cool_down = _cool_time;

            _operation();
            return true;
        }

        return false;
    }

    bool doNothing()
    {
        // do nothing
        return false;
    }

    float _cool_time;
    float _cool_down;
    int _count;
    std::function<void()> _operation;
    std::function<bool()> _work;
};
