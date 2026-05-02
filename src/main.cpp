#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace geode::prelude;

static bool g_seeded = false;

class $modify(DeathMessagesLayer, PlayLayer) {

    struct Fields {
        bool hasDied = false;
        int attemptCount = 0;
        int lastProgressStep = -1;
    };

    void showMessage(const char* text, cocos2d::CCPoint pos, float scale = 0.75f) {
        auto label = CCLabelBMFont::create(text, "goldFont.fnt");
        if (!label) return;

        label->setPosition(pos);
        label->setScale(0.f);
        label->setOpacity(255);
        label->setZOrder(9999);
        label->setAnchorPoint({ 0.5f, 0.5f });

        this->addChild(label);

        auto popIn = CCEaseElasticOut::create(
            CCScaleTo::create(0.4f, scale),
            0.6f
        );

        auto seq = CCSequence::create(
            popIn,
            CCDelayTime::create(1.2f),
            CCFadeOut::create(0.4f),
            CCMoveBy::create(0.4f, { 0.f, 15.f }),
            CCRemoveSelf::create(),
            nullptr
        );

        label->runAction(seq);
    }

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        if (!g_seeded) {
            std::srand((unsigned)std::time(nullptr));
            g_seeded = true;
        }

        m_fields->attemptCount = 1;
        m_fields->hasDied = false;
        m_fields->lastProgressStep = -1;

        auto size = CCDirector::get()->getWinSize();

        static const std::vector<std::string> startMessages = {
            "good luck!",
            "lock in",
            "dont die instantly 💀",
            "first attempt energy",
            "you got this"
        };

        showMessage(
            startMessages[std::rand() % startMessages.size()].c_str(),
            { size.width / 2.f, size.height / 2.f + 60.f },
            0.75f
        );

        return true;
    }

    void destroyPlayer(PlayerObject * player, GameObject * obj) {
        PlayLayer::destroyPlayer(player, obj);

        if (player != m_player1) return;
        if (m_fields->hasDied) return;

        m_fields->hasDied = true;

        auto size = CCDirector::get()->getWinSize();

        if (m_fields->attemptCount <= 1) return;

        static const std::vector<std::string> deathMessages = {
            "bro really did that 💀",
            "skill issue confirmed",
            "geometry said NO",
            "that spike was personal",
            "gravity won again",
            "you blinked and lost",
            "almost had it...",
            "practice mode exists btw",
            "pain.",
            "touch grass after this one"
        };

        showMessage(
            deathMessages[std::rand() % deathMessages.size()].c_str(),
            { size.width / 2.f, size.height / 2.f + 60.f },
            0.75f
        );
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        float percent = this->getCurrentPercent();
        int step = (int)(percent / 10.0f);

        if (step == m_fields->lastProgressStep) return;
        m_fields->lastProgressStep = step;

        auto size = CCDirector::get()->getWinSize();

        // 🔥 COOKING MODE (moved left + up)
        if (percent >= 80.f) {

            static const std::vector<std::string> cookingMessages = {
                "YOU'RE COOKING 🔥",
                "LOCKED IN",
                "DON'T SELL",
                "FINAL STRETCH",
                "INSANE RUN"
            };

            showMessage(
                cookingMessages[std::rand() % cookingMessages.size()].c_str(),
                { size.width - 120.f, 60.f },
                0.75f
            );

            return;
        }

        // 📊 PROGRESS (same position as cooking)
        if (step > 0) {

            static const std::vector<std::string> progressMessages = {
                "keep going",
                "you got this",
                "focus up",
                "solid run",
                "not bad",
                "clean progress",
                "nice movement",
                "don't choke",
                "locked in?",
                "still alive 👍"
            };

            showMessage(
                progressMessages[std::rand() % progressMessages.size()].c_str(),
                { size.width - 120.f, 60.f },
                0.75f
            );
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();

        m_fields->hasDied = false;
        m_fields->attemptCount++;
        m_fields->lastProgressStep = -1;
    }
};