#include <QQmlApplicationEngine>

class EngineAccess {
  public:
  inline static std::unique_ptr<QQmlApplicationEngine> engine = nullptr;

  EngineAccess() {
    if (engine == nullptr) {
      engine = std::make_unique<QQmlApplicationEngine>();
    }
  }
};