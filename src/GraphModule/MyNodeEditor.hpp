#pragma once

#include <QtNodes/QmlWrapper>
#include <qqmlintegration.h>

class MyNodeEditor : public QtNodes::QmlWrapper
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit MyNodeEditor(QQuickItem *parent = nullptr)
        : QtNodes::QmlWrapper(parent)
    {}

protected:
    [[nodiscard]] std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels() const override;
};