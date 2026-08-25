#pragma once

#include "NodeDelegateModel.hpp"
#include "RegisteredNodeModel.hpp"

#include <QObject>
#include <QQmlEngine>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtQmlIntegration>

#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

/// Class uses map for storing models (name, model)
class NodeDelegateModelRegistry : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("Nodes must be created from C++")

  private:
  QQmlEngine *_engine;

  public:
  Q_PROPERTY(RegisteredNodeModel *nodesModel READ getNodesModel CONSTANT)
  Q_PROPERTY(QList<QString> categories READ categories NOTIFY categoriesChanged)

  using RegistryItemPtr = std::unique_ptr<NodeDelegateModel>;
  using RegistryItemCreator = std::function<RegistryItemPtr()>;
  using CategoryName = QString;

  using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
  using Categories = QList<CategoryName>;

  NodeDelegateModelRegistry(QQmlEngine *engine, QObject *parent = nullptr)
      : QObject(parent), _engine(engine) {};
  ~NodeDelegateModelRegistry() = default;
  NodeDelegateModelRegistry(NodeDelegateModelRegistry const &) = delete;
  NodeDelegateModelRegistry &operator=(NodeDelegateModelRegistry const &) = delete;

  protected:
  template <typename ModelType>
  void registerModel(RegistryItemCreator creator, QString const &category = "Nodes") {
    NodeDelegateModel::ModelInfos const infos = computeInfos<ModelType>(creator);
    if (!_registeredItemCreators.count(infos.name)) {
      _registeredItemCreators[infos.name] = std::move(creator);
      if (!_categories.contains(category)) {
        _categories.push_front(category);
        _categories.sort(Qt::CaseInsensitive);
        emit categoriesChanged();
      }
      _nodesModel.registerNode(infos, category);
    }
  }

  template <typename ModelCreator>
  void registerModel(ModelCreator &&creator, QString const &category = "Nodes") {
    using ModelType = compute_model_type_t<decltype(creator())>;
    registerModel<ModelType>(std::forward<ModelCreator>(creator), category);
  }

  public:
  template <typename ModelType>
  void registerModel(QString const &category = "Nodes") {
    RegistryItemCreator creator = [this]() { return std::make_unique<ModelType>(_engine); };
    registerModel<ModelType>(std::move(creator), category);
  }
#if 0
  template<typename ModelType>
  void
  registerModel(RegistryItemCreator creator,
                QString const&      category = "Nodes")
  {
    registerModel<ModelType>(std::move(creator), category);
  }


  template <typename ModelCreator>
  void
  registerModel(QString const& category, ModelCreator&& creator)
  {
    registerModel(std::forward<ModelCreator>(creator), category);
  }


  void
  registerTypeConverter(TypeConverterId const& id,
                        TypeConverter          typeConverter)
  {
    _registeredTypeConverters[id] = std::move(typeConverter);
  }

#endif

  std::unique_ptr<NodeDelegateModel> create(QString const &modelName);

  RegisteredModelCreatorsMap const &registeredModelCreators() const;
  RegisteredNodeModel *getNodesModel() { return &_nodesModel; }

  Categories const &categories() const;
  Categories categories();

#if 0
  TypeConverter
  getTypeConverter(NodeDataType const& d1,
                   NodeDataType const& d2) const;
#endif

  private:
  RegisteredNodeModel _nodesModel;
  Categories _categories;

  RegisteredModelCreatorsMap _registeredItemCreators;

#if 0
  RegisteredTypeConvertersMap _registeredTypeConverters;
#endif

  private:
  // If the registered ModelType class has the static member method
  // `static ModelInfos ModelInfos();`, use it. Otherwise use the non-static
  // method: `virtual ModelInfos ModelInfos() const;`
  template <typename T, typename = void>
  struct HasStaticMethodName : std::false_type {};

  template <typename T>
  struct HasStaticMethodName<
      T, typename std::enable_if<
             std::is_same<decltype(T::ModelInfos()), NodeDelegateModel::ModelInfos>::value>::type>
      : std::true_type {};

  template <typename ModelType,
            typename std::enable_if_t<HasStaticMethodName<ModelType>::value, bool> = true>
  static NodeDelegateModel::ModelInfos computeInfos(RegistryItemCreator const &_creator) {
    return ModelType::ModelInfos();
  }

  template <typename ModelType,
            typename std::enable_if_t<!HasStaticMethodName<ModelType>::value, bool> = true>
  static NodeDelegateModel::ModelInfos computeInfos(RegistryItemCreator const &creator) {
    return creator()->modelInfos();
  }

  template <typename T>
  struct UnwrapUniquePtr {
    // Assert always fires, but the compiler doesn't know this:
    static_assert(!std::is_same<T, T>::value,
                  "The ModelCreator must return a std::unique_ptr<T>, where T "
                  "inherits from NodeDelegateModel");
  };

  template <typename T>
  struct UnwrapUniquePtr<std::unique_ptr<T>> {
    static_assert(std::is_base_of<NodeDelegateModel, T>::value,
                  "The ModelCreator must return a std::unique_ptr<T>, where T "
                  "inherits from NodeDelegateModel");
    using type = T;
  };

  template <typename CreatorResult>
  using compute_model_type_t = typename UnwrapUniquePtr<CreatorResult>::type;

  public:
  signals:
  void categoriesChanged();
};
