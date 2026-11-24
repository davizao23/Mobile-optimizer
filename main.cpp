#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>

using namespace geode::prelude;

static void applyFps() {
    auto mod = Mod::get();
    int fps = static_cast<int>(mod->getSettingValue<int64_t>("fps-limit"));
    if (mod->getSettingValue<bool>("battery-saver")) fps = 30;
    if (fps < 1) fps = 60;
    CCDirector::get()->setAnimationInterval(1.0 / static_cast<double>(fps));
}

$execute {
    listenForSettingChanges<int64_t>("fps-limit", [](int64_t) { applyFps(); });
    listenForSettingChanges<bool>("battery-saver", [](bool) { applyFps(); });
}

// Aplica o FPS ao carregar o menu principal
#include <Geode/modify/MenuLayer.hpp>
class $modify(OptMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        applyFps();
        return true;
    }
};

// Pula o update de partículas durante a fase
class $modify(OptParticle, CCParticleSystem) {
    void update(float dt) {
        if (PlayLayer::get() && Mod::get()->getSettingValue<bool>("disable-particles")) {
            this->setVisible(false);
            return;
        }
        CCParticleSystem::update(dt);
    }
};
