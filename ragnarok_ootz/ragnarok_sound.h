#pragma once

#include "sound.h"

struct RagnarokSound {
    struct Background {
        struct Dir : Sound {
            virtual std::string getPath() const override { return Sound::getPath() + "background/"; }
        };

        struct S000 : Dir {
            virtual std::string getPath() const override { return Dir::getPath() + "000.mp3"; }
            virtual bool isDirectory() const override { return false; }
        };
    };

    struct Boss {
        struct Dir : Sound {
            virtual std::string getPath() const override { return Sound::getPath() + "boss/"; }
        };

        struct Die : Dir {
            virtual std::string getPath() const override { return Dir::getPath() + "die.wav"; }
            virtual bool isDirectory() const override { return false; }
        };

        struct Attack : Dir {
            virtual std::string getPath() const override { return Dir::getPath() + "attack.wav"; }
            virtual bool isDirectory() const override { return false; }
        };

        struct Hit : Dir {
            virtual std::string getPath() const override { return Dir::getPath() + "hit.wav"; }
            virtual bool isDirectory() const override { return false; }
        };

        struct Move : Dir {
            virtual std::string getPath() const override { return Dir::getPath() + "move.wav"; }
            virtual bool isDirectory() const override { return false; }
        };
    };

    struct Player {
        struct Dir : Sound {
            virtual std::string getPath() const override { return Sound::getPath() + "player/"; }
        };

        struct Hit : Dir {
            virtual std::string getPath() const override { return Dir::getPath() + "hit.wav"; }
            virtual bool isDirectory() const override { return false; }
        };

        struct Lightning {
            struct Dir : Player::Dir {
                virtual std::string getPath() const override { return Player::Dir::getPath() + "lightning/"; }
            };

            struct Storm : Dir {
                virtual std::string getPath() const override { return Dir::getPath() + "storm.wav"; }
                virtual bool isDirectory() const override { return false; }
            };

            struct Hit : Dir {
                virtual std::string getPath() const override { return Dir::getPath() + "hit.wav"; }
                virtual bool isDirectory() const override { return false; }
            };
        };

        struct CarrotMissile {
            struct Dir : Player::Dir {
                virtual std::string getPath() const override { return Player::Dir::getPath() + "carrot_missile/"; }
            };

            struct Fall : Dir {
                virtual std::string getPath() const override { return Dir::getPath() + "fall.wav"; }
                virtual bool isDirectory() const override { return false; }
            };

            struct Explosion : Dir {
                virtual std::string getPath() const override { return Dir::getPath() + "explosion.wav"; }
                virtual bool isDirectory() const override { return false; }
            };

            struct Hit : Dir {
                virtual std::string getPath() const override { return Dir::getPath() + "hit.wav"; }
                virtual bool isDirectory() const override { return false; }
            };
        };
    };

    struct JinBoss
    {
        struct Dir : Sound
        {
            virtual std::string getPath() const override { return Sound::getPath() + "jin_boss/"; }
        };

        struct Spawn : Dir
        {
            virtual std::string getPath() const override { return Dir::getPath() + "spawn.wav"; }
            virtual bool isDirectory() const override { return false; }
        };
    };
};
