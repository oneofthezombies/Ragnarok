#pragma once

#include <sstream>
#include <iomanip>
#include <vector>

#include "singleton.h"
#include "image.h"
#include "text_reader.h"

class _SpriteManager : public Singleton<_SpriteManager>
{
public:
    virtual ~_SpriteManager()
    {
        for (auto sprites : _sprites)
            for (auto group : sprites.second)
                for (auto sprite : group.second)
                    delete sprite;
    }

    bool load()
    {
        TextReader text_reader("resources/sprites/sprites_info.ootz");
        if (!text_reader)
            return false;

        int number_of_sprites = std::stoi(text_reader.getNextTokens().front());
        for (int i = 0; i < number_of_sprites; ++i)
        {
            std::stringstream sprite_directory;
            sprite_directory << std::setw(3) << std::setfill('0') << std::to_string(i);

            TextReader text_reader("resources/sprites/" + sprite_directory.str() + "/sprite_info.ootz");
            if (!text_reader)
                return false;

            int number_of_sprites = std::stoi(text_reader.getNextTokens().front());

            bool has_mirror_image = false;
            if (std::stoi(text_reader.getNextTokens().front()))
                has_mirror_image = true;

            auto tokens = text_reader.getNextTokens();
            COLORREF color_key = RGB(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]));

            std::vector<Image*> sprites;
            for (int i = 0; i < number_of_sprites; ++i)
            {
                std::stringstream sprite_file_name;
                sprite_file_name << std::setw(3) << std::setfill('0') << std::to_string(i);

                sprites.emplace_back(new Image("resources/sprites/" + sprite_directory.str() + "/normal/" + sprite_file_name.str() + ".bmp", color_key));
            }

            _sprites[i][0] = sprites;

            if (has_mirror_image)
            {
                std::vector<Image*> sprites;
                for (int i = 0; i < number_of_sprites; ++i)
                {
                    std::stringstream sprite_file_name;
                    sprite_file_name << std::setw(3) << std::setfill('0') << std::to_string(i);

                    sprites.emplace_back(new Image("resources/sprites/" + sprite_directory.str() + "/mirror/" + sprite_file_name.str() + ".bmp", color_key));
                }

                _sprites[i][1] = sprites;
            }
        }

        return true;
    }

    Image* getSprite(const int sprites_key, const bool mirror_key = false, const int sprite_key = 0) {
        auto sprites = _getSprites(sprites_key, mirror_key);

        assert((static_cast<unsigned int>(sprite_key) < sprites.size()) && "SpriteManager::getSprite()");

        return sprites.at(sprite_key);
    }

private:
    std::vector<Image*>& _getSprites(const int sprites_key, const bool mirror_key) {
        assert((static_cast<unsigned int>(sprites_key) < _sprites.size()) && "SpriteManager::_getSprites()");

        return _sprites.at(sprites_key).at(Utility::BoolToInt(mirror_key));
    }

    std::unordered_map<int, std::unordered_map<int, std::vector<Image*>>> _sprites;
};

struct SpriteManager
{
    _SpriteManager* operator()()
    {
        return _SpriteManager::getInstance();
    }
};
