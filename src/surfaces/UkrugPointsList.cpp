#include "UkrugPointsList.hpp"
#include <qdebug.h>
#include <qmath.h>

UkrugPointsList::UkrugPointsList() {
  QString fileName = ":/qt/qml/CutieDesignerModule/surfaces/UkrugPointsData.json";
  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Couldn't open file" << fileName;
    return;
  }
  _pointsData = QJsonDocument::fromJson(file.readAll()).object();
  _points.append(UkrugPoint{0, 4, false});
  _points.append(UkrugPoint{M_PI / 2, 4, false});
  _points.append(UkrugPoint{M_PI, 4, false});
  _points.append(UkrugPoint{M_PI * 3 / 2, 4, false});
}

static float absoluteAngle(float angle) {
  while (angle < 0) {
    angle += 360;
  }
  while (angle > 360) {
    angle -= 360;
  }
  return angle;
}

static bool pairContain(std::set<std::pair<int, int>> &pairing, int value, size_t pos) {
  auto pred = [value, pos](const auto &pair) {
    if (pos == 0) {
      return std::get<0>(pair) == value;
    } else {
      return std::get<1>(pair) == value;
    }
  };
  auto found = std::find_if(pairing.begin(), pairing.end(), pred);
  return found != pairing.end();
}

static std::pair<int, int> getClosestMatch(QList<UkrugPoint> &first, QList<UkrugPoint> &second,
                                           std::set<std::pair<int, int>> &alreadyMatched) {
  float distance = std::numeric_limits<float>::max();
  std::pair<int, int> pair{-1, -1};

  for (int i = 0; i != first.count(); i++) {
    if (pairContain(alreadyMatched, i, 0))
      continue;

    for (int j = 0; j != second.count(); j++) {
      if (pairContain(alreadyMatched, j, 1))
        continue;

      if (first[i].distance == second[j].distance) {
        auto currentDist = std::abs(absoluteAngle(first[i].angle) - absoluteAngle(second[j].angle));
        if (currentDist < distance) {
          distance = currentDist;
          pair = {i, j};
        }
      }
    }
  }

  return pair;
}

void UkrugPointsList::assignLetter(QString character) {
  auto find = _pointsData.find(character.toUpper());
  if (find == _pointsData.end()) {
    for (int i = 0; i != _points.length(); i++) {
      _points[i].distance = 4;
      _points[i].animateAngle = true;
    }
  } else {
    auto points = find.value().toArray();
    for (int i = 0; i != points.count(); i++) {
      auto newDist = points[i].toObject()["distance"].toInt();
      if (newDist != 0) {
        auto angle = points[i].toObject()["angle"].toDouble();
        auto prevAngle = qRadiansToDegrees(_points[i].angle);
        if (abs(prevAngle - (angle + 360)) < abs(prevAngle - angle))
          angle += 360;
        _points[i].angle = qDegreesToRadians(angle);
      }
      _points[i].distance = newDist;
      _points[i].animateAngle =
          _points[i].distance != 4 && _points[i].distance != 0 && newDist != 4 && newDist != 0;
    }
    for (int i = points.count(); i != _points.length(); i++) {
      _points[i].distance = 4;
      _points[i].animateAngle = true;
    }
  }
  emit pointsChanged();
}
// void UkrugPointsListModel::setLetter(QString letter) {
//   qDebug() << "letter: " << letter;
//   QList<UkrugPoint> letterData;

//   if (letter == "A") {
//     for (auto &p : A) {
//       letterData.append(p);
//     }
//   } else {
//     for (auto &p : NotA) {
//       letterData.append(p);
//     }
//   }

//   // Find best matches
//   std::set<std::pair<int, int>> indexMatchs;

//   for (int i = 0; i != letterData.count(); i++) {
//     auto [firstIdx, secondIdx] = getClosestMatch(_points, letterData, indexMatchs);
//     if (firstIdx != -1) {
//       qDebug() << firstIdx << " " << secondIdx;
//       indexMatchs.insert({firstIdx, secondIdx});
//     }
//   }

//   // Changed matched angle:
//   for (auto &[first, second] : indexMatchs) {
//     _points[first].angle = letterData[second].angle;
//     emit dataChanged(index(first, 0), index(first, 0), {static_cast<int>(PointRoles::Angle)});
//   }

//   for (int i = _points.count() - 1; i != -1; i--) {
//     if (!pairContain(indexMatchs, i, 0)) {
//       beginRemoveRows({}, i, i);
//       _points.remove(i);
//       endRemoveRows();
//     }
//   }

//   // Add missing elements
//   for (int i = 0; i != letterData.count(); i++) {
//     if (!pairContain(indexMatchs, i, 1)) {
//       beginInsertRows({}, rowCount(), rowCount());
//       _points.append(letterData[i]);
//       endInsertRows();
//     }
//   }
// }
