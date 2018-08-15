#include "field_scene.h"

#include "window_manager.h"
#include "converter.h"
#include "text_reader.h"
#include "utility.h"
#include "game_object_manager.h"
#include "boss.h"
#include "sound_manager.h"
#include "ragnarok_sound.h"
#include "player.h"

FieldScene::FieldScene()
{
    //getRenderer()->setSpritesKey(2);
    getRenderer()->setSpritesKey(3);
    
    auto _tr = getTransform();
    _tr->setPosition(2900.0f/2.0f, 2550.0f/2.0f);

    _map.cell_size = Vector2(WindowInfo::kCellWidth, WindowInfo::kCellHeight);

    TextReader tr("resources/map_infos/map_info.ootz");
    _map.resize(51, std::vector<int>(58, 0));
    for (unsigned int i = 0; i < _map.size(); ++i)
    {
        auto tokens = tr.getNextTokens();
        for (unsigned int j = 0; j < _map[i].size(); ++j)
            _map[i][j] = std::stoi(tokens[j]);
    }

    SoundManager{}()->registerSound(RagnarokSound::Background::S000{});

    SoundManager{}()->registerSound(RagnarokSound::Boss::Move{});
    SoundManager{}()->registerSound(RagnarokSound::Boss::Attack{});
    SoundManager{}()->registerSound(RagnarokSound::Boss::Hit{});
    SoundManager{}()->registerSound(RagnarokSound::Boss::Die{});

    SoundManager{}()->registerSound(RagnarokSound::Player::Hit{});

    SoundManager{}()->registerSound(RagnarokSound::Player::Lightning::Storm{});
    SoundManager{}()->registerSound(RagnarokSound::Player::Lightning::Hit{});

    SoundManager{}()->registerSound(RagnarokSound::Player::CarrotMissile::Fall{});
    SoundManager{}()->registerSound(RagnarokSound::Player::CarrotMissile::Explosion{});
    SoundManager{}()->registerSound(RagnarokSound::Player::CarrotMissile::Hit{});

    SoundManager{}()->registerSound(RagnarokSound::JinBoss::Spawn{});

    SoundManager{}()->play(RagnarokSound::Background::S000{});
}

void FieldScene::update()
{
}

void FieldScene::render(HDC const& hdc)
{
    auto image = SpriteManager{}()->getSprite(2);
    auto pos = Vector2::zero - GameObjectManager::GetFactorOfPosition();
    BitBlt(hdc, 0, 0, 1600, 900, image->getDC(), static_cast<int>(pos.x), static_cast<int>(pos.y), SRCCOPY);

    //auto factor = GameObjectManager::GetFactorOfPosition();
    //for (int i = 0; i < _map.size(); ++i)
    //    for (int j = 0; j < _map[i].size(); ++j)
    //    {
    //        if (!_map[i][j])
    //        {
    //            auto pos = Converter::MapSpaceToWorldSpace(std::make_pair(j, i), getCellSize());

    //            std::string cell_attributes_str = std::to_string(_map[i][j]);
    //            TextOut(hdc, static_cast<int>(pos.x + factor.x) - 5, static_cast<int>(pos.y + factor.y) - 10, cell_attributes_str.c_str(), cell_attributes_str.size());
    //        }
    //    }
}