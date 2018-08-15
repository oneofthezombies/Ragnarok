#pragma once

#include <unordered_map>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <string>
#include <vector>
#include <array>

#include "vector2.h"
#include "singleton.h"

#include "render_info.h"
#include "text_reader.h"

struct Frame : public std::vector<RenderInfo*>
{
    virtual ~Frame()
    {
        for (auto render_info : *this)
            delete render_info;
    }
};

struct Action : public std::vector<Frame*>
{
    virtual ~Action()
    {
        for (auto frame : *this)
            delete frame;
    }
};

struct Animation
{
    std::string name;
    int number_of_frames;
    float interval_time; // unit : second
    std::array<int, 8> action_indices;
};

struct Act
{
    ~Act()
    {
        for (auto animation : animations)
            delete animation.second;

        for (auto action : actions)
            delete action.second;
    }

    std::unordered_map<int, Animation*> animations;
    std::unordered_map<int, Action*> actions;
};

class _ActManager : public Singleton<_ActManager>
{
public:
    virtual ~_ActManager()
    {
        for (auto act : _acts)
            delete act.second;
    }

    bool load()
    {
        TextReader text_reader("resources/acts/acts_info.ootz");
        if (!text_reader)
            return false;

        auto tokens = text_reader.getNextTokens();
        int number_of_act_files = std::stoi(tokens.front());
        for (int af_i = 0; af_i < number_of_act_files; ++af_i)
        {
            std::stringstream act_file_name;
            act_file_name << std::setw(3) << std::setfill('0')  << std::to_string(af_i);
            TextReader text_reader("resources/acts/" + act_file_name.str() + ".ootz_act");
            if (!text_reader)
                return false;

            Act* act = new Act;
            int number_of_animations = std::stoi(text_reader.getNextTokens().front());
            int number_of_actions = number_of_animations * kNumberOfActionsOfEachAnimation;

            for (int an_i = 0; an_i < number_of_animations; ++an_i)
            {
                Animation* animation = setAnimation(text_reader.getNextTokens());

                for (int ac_i = 0; ac_i < kNumberOfActionsOfEachAnimation; ++ac_i)
                {
                    Action* action = new Action;

                    for (int f_i = 0; f_i < animation->number_of_frames; ++f_i)
                        action->emplace_back(setFrame(&text_reader));

                    int action_i = an_i * kNumberOfActionsOfEachAnimation + ac_i;
                    act->actions[action_i] = action;
                    animation->action_indices[ac_i] = action_i;
                }

                act->animations[an_i] = animation;
            }

            _acts[af_i] = act;
        }

        return true;
    }

    int numberOfActs() const
    {
        return _acts.size();
    }

    int getSizeOfActs() const
    {
        return _acts.size();
    }

    Act* getAct(const int key)
    {
        assert((static_cast<unsigned int>(key) < _acts.size()) && "ActManager::getAct()");

        return _acts.at(key);
    }

    Animation* getAnimation(int const& act_key, int const& animation_key)
    {
        auto act = getAct(act_key);

        assert((static_cast<unsigned int>(animation_key) < act->animations.size()) && "ActManager::getAnimation()");

        return act->animations.at(animation_key);
    }

    Frame* getFrame(const int act_key, const int animation_key, const int action_key, const int frame_key)
    {
        auto animation = getAnimation(act_key, animation_key);

        assert((static_cast<unsigned int>(action_key) < animation->action_indices.size()) && "ActManager::getFrame()");

        auto action_i = animation->action_indices.at(action_key);

        assert((static_cast<unsigned int>(action_i) < _acts.at(act_key)->actions.size()) && "ActManager::getFrame()");

        return _acts.at(act_key)->actions.at(action_i)->at(frame_key);
    }

    int getFrameSize(int const& act_key, int const& animation_key, int const& action_key, int const& frame_key)
    {
        auto action_i = _acts.at(act_key)->animations.at(animation_key)->action_indices.at(action_key);
        return _acts.at(act_key)->actions.at(action_i)->size();
    }

private:
    Animation* setAnimation(std::vector<std::string> const& tokens)
    {
        Animation* animation = new Animation;
        int idx = 0;
        for (auto token : tokens)
            if (token.front() >= '0' &&
                token.front() <= '9')
                break;
            else
                idx++;

        for (int i = 0; i < idx; ++i)
        {
            animation->name += tokens.at(i);
            if (i != (idx - 1))
                animation->name += ' ';
        }

        animation->number_of_frames = std::stoi(tokens.at(idx));
        animation->interval_time = std::stof(tokens.at(idx + 1)) / 1000.0f;

        return animation;
    }

    RenderInfo* setRenderInfo(std::vector<std::string> const& tokens)
    {
        if (tokens.size() != 8)
        {
            for (auto a : tokens)
                std::cout << a << std::endl;
            std::cout << std::endl;
        }
        assert(tokens.size() == 8);

        RenderInfo* render_info = new RenderInfo;

        render_info->sprite_index = std::stoi(tokens.front());
        render_info->position.x = std::stof(tokens.at(1));
        render_info->position.y = std::stof(tokens.at(2));

        if (std::stoi(tokens.at(3)))
            render_info->is_mirror = true;
        else
            render_info->is_mirror = false;

        render_info->alpha = std::stoi(tokens.at(4));
        render_info->scale.x = std::stof(tokens.at(5));
        render_info->scale.y = std::stof(tokens.at(6));
        render_info->rotation = std::stof(tokens.at(7));

        //std::cout << render_info->sprite_index << ' ';
        //std::cout << render_info->position.x << ' ' << render_info->position.y << ' ';
        //std::cout << render_info->is_mirror << ' ';
        //std::cout << static_cast<int>(render_info->alpha) << ' ';
        //std::cout << render_info->scale.x << ' ' << render_info->scale.y << ' ';
        //std::cout << render_info->rotation << std::endl;

        return render_info;
    }

    Frame* setFrame(TextReader* text_reader)
    {
        Frame* frame = new Frame;

        int number_of_sprites_of_this_frame = std::stoi(text_reader->getNextTokens().front());
        for (int i = 0; i < number_of_sprites_of_this_frame; ++i)
            frame->emplace_back(setRenderInfo(text_reader->getNextTokens()));

        return frame;
    }

    const int kNumberOfActionsOfEachAnimation = 8;
    std::unordered_map<int, Act*> _acts;
};

struct ActManager
{
    _ActManager* operator()()
    {
        return _ActManager::getInstance();
    }
};
