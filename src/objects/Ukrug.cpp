#include "Ukrug.hpp"

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

      if (first[i].layer == second[j].layer) {
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

void UkrugPointsListModel::setLetter(QString letter) {
  qDebug() << "letter: " << letter;
  QList<UkrugPoint> letterData;

  if (letter == "A") {
    for (auto &p : A) {
      letterData.append(p);
    }
  } else {
    for (auto &p : NotA) {
      letterData.append(p);
    }
  }

  // Find best matches
  std::set<std::pair<int, int>> indexMatchs;

  for (int i = 0; i != letterData.count(); i++) {
    auto [firstIdx, secondIdx] = getClosestMatch(_points, letterData, indexMatchs);
    if (firstIdx != -1) {
      qDebug() << firstIdx << " " << secondIdx;
      indexMatchs.insert({firstIdx, secondIdx});
    }
  }

  // Changed matched angle:
  for (auto &[first, second] : indexMatchs) {
    _points[first].angle = letterData[second].angle;
    emit dataChanged(index(first, 0), index(first, 0), {static_cast<int>(PointRoles::Angle)});
  }

  for (int i = _points.count() - 1; i != -1; i--) {
    if (!pairContain(indexMatchs, i, 0)) {
      beginRemoveRows({}, i, i);
      _points.remove(i);
      endRemoveRows();
    }
  }

  // Add missing elements
  for (int i = 0; i != letterData.count(); i++) {
    if (!pairContain(indexMatchs, i, 1)) {
      beginInsertRows({}, rowCount(), rowCount());
      _points.append(letterData[i]);
      endInsertRows();
    }
  }
}

// https://iquilezles.org/articles/distfunctions2d/

/*
// The MIT License
// Copyright © 2020 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions: The above copyright notice and this
// permission notice shall be included in all copies or substantial portions of the Software. THE
// SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// Signed distance to a disk

// List of some other 2D distances: https://www.shadertoy.com/playlist/MXdSRf
//
// and iquilezles.org/articles/distfunctions2d


float sdCircle( in vec2 p, in float r )
{
    return length(p)-r;
}


float opUnion( float d1, float d2 )
{
    return min(d1,d2);
}

float opSubtraction( float d1, float d2 )
{
    return max(-d1,d2);
}

float opIntersection( float d1, float d2 )
{
    return max(d1,d2);
}

float opSmoothUnion( float d1, float d2, float k )
{
    k *= 4.0;
    float h = max(k-abs(d1-d2),0.0);
    return min(d1, d2) - h*h*0.25/k;
}

float opSmoothSubtraction( float d1, float d2, float k )
{
    return -opSmoothUnion(d1,-d2,k);

    //float h = max(4.0*k-abs(d1+d2),0.0);
    //return max(-d1, d2) + h*h/(16.0*k);
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
        vec2 p = (2.0*fragCoord-iResolution.xy)/iResolution.y;
    vec2 m = (2.0*iMouse.xy-iResolution.xy)/iResolution.y;

    //float d = max(max(sdCircle(p,0.5), sdCircle(p + 0.3, 0.2) * -1.0), sdCircle(p - 0.3, 0.2) *
-1.0); float d = opSmoothSubtraction(sdCircle(p + 0.3, 0.2), sdCircle(p,0.5), 0.01); d =
opSmoothSubtraction(sdCircle(p - 0.3, 0.2), d, 0.01);

        // coloring
    vec3 col = (d>0.0) ? vec3(0.0,0.0,0.0) : vec3(1.0,1.0,1.0);
    if (d > 0.0) {
        col += 1.0 - (d * 100.);
    }
    //vec3 col = (d>0.0) ? vec3(0.9,0.6,0.3) : vec3(0.65,0.85,1.0);
    //col *= 1.0 - exp(-6.0*abs(d));
        //col *= 0.8 + 0.2*cos(150.0*d);
        //col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,0.01,abs(d)) );

    if( iMouse.z>0.001 )
    {
    d = opSmoothSubtraction(sdCircle(m + 0.3, 0.2), sdCircle(m,0.5), 0.015);
    d = opSmoothSubtraction(sdCircle(m - 0.3, 0.2), d, 0.015);

    col = mix(col, vec3(1.0,1.0,0.0), 1.0-smoothstep(0.0, 0.005, abs(length(p-m)-abs(d))-0.0025));
    col = mix(col, vec3(1.0,1.0,0.0), 1.0-smoothstep(0.0, 0.005, length(p-m)-0.015));
    }

        fragColor = vec4(col,1.0);
}
*/