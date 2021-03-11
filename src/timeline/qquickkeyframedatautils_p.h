/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick Designer Components.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQUICKKEYFRAMEDATAUTILS_H
#define QQUICKKEYFRAMEDATAUTILS_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QCborStreamReader>
#include <QtCore/QCborStreamWriter>
#include <QtCore/QVariant>
#include <QtCore/QMetaType>

#include <QtCore/QRect>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QVector4D>
#include <QtGui/QQuaternion>
#include <QtGui/QColor>

// This file contains useful methods to read & write keyframes
// CBOR binary files and understand the format.

// Read property 'type' value from CBOR and return it as QVariant.
QVariant readValue(QCborStreamReader &reader, QMetaType::Type type)
{
    switch (type) {
    case QMetaType::Bool: {
        bool b = reader.toBool();
        reader.next();
        return QVariant(b);
        break;
    }
    case QMetaType::Int: {
        int i = reader.toInteger();
        reader.next();
        return QVariant(i);
        break;
    }
    case QMetaType::Float: {
        float f = reader.toFloat();
        reader.next();
        return QVariant(f);
        break;
    }
    case QMetaType::Double: {
        double d = reader.toDouble();
        reader.next();
        return QVariant(d);
        break;
    }
    case QMetaType::QString: {
        QString s = reader.readString().data;
        return QVariant(s);
        break;
    }
    case QMetaType::QVector2D: {
        QVector2D v;
        v.setX(reader.toFloat());
        reader.next();
        v.setY(reader.toFloat());
        reader.next();
        return QVariant(v);
        break;
    }
    case QMetaType::QVector3D: {
        QVector3D v;
        v.setX(reader.toFloat());
        reader.next();
        v.setY(reader.toFloat());
        reader.next();
        v.setZ(reader.toFloat());
        reader.next();
        return QVariant(v);
        break;
    }
    case QMetaType::QVector4D: {
        QVector4D v;
        v.setX(reader.toFloat());
        reader.next();
        v.setY(reader.toFloat());
        reader.next();
        v.setZ(reader.toFloat());
        reader.next();
        v.setW(reader.toFloat());
        reader.next();
        return QVariant(v);
        break;
    }
    case QMetaType::QQuaternion: {
        QQuaternion q;
        q.setScalar(reader.toFloat());
        reader.next();
        q.setX(reader.toFloat());
        reader.next();
        q.setY(reader.toFloat());
        reader.next();
        q.setZ(reader.toFloat());
        reader.next();
        return QVariant(q);
        break;
    }
    case QMetaType::QColor: {
        QColor c;
        c.setRed(reader.toInteger());
        reader.next();
        c.setGreen(reader.toInteger());
        reader.next();
        c.setBlue(reader.toInteger());
        reader.next();
        c.setAlpha(reader.toInteger());
        reader.next();
        return QVariant(c);
        break;
    }
    case QMetaType::QRect: {
        QRect r;
        r.setX(reader.toInteger());
        reader.next();
        r.setY(reader.toInteger());
        reader.next();
        r.setWidth(reader.toInteger());
        reader.next();
        r.setHeight(reader.toInteger());
        reader.next();
        return QVariant(r);
        break;
    }
    default: {
        qWarning() << "Keyframe property type not handled:" << type;
    }
    }

    return QVariant();
}

// Read a string from COB.
QString readString(QCborStreamReader &reader)
{
    QString result;
    auto r = reader.readString();
    while (r.status == QCborStreamReader::Ok) {
        result += r.data;
        r = reader.readString();
    }

    if (r.status == QCborStreamReader::Error) {
        // handle error condition
        result.clear();
    }
    return result;
}

// Read a real (double or float) from CBOR.
double readReal(QCborStreamReader &reader)
{
    double result = 0.0;
    if (reader.isDouble()) {
        result = reader.toDouble();
        reader.next();
    } else if (reader.isFloat()) {
        result = reader.toFloat();
        reader.next();
    }
    return result;
}

// Read keyframes file header and return version.
// If header is not valid, -1 is returned.
int readKeyframesHeader(QCborStreamReader &reader)
{
    int version = -1;
    if (reader.lastError() == QCborError::NoError && reader.isArray()) {
        // Start root array
        reader.enterContainer();
        if (reader.isString()) {
            QString header = readString(reader);
            if (header == QStringLiteral("QTimelineKeyframes")) {
                if (reader.isInteger()) {
                    version = reader.toInteger();
                    reader.next();
                } else {
                    qWarning() << "Invalid keyframeSource version";
                }
            } else {
                qWarning() << "Invalid keyframeSource header";
            }
        } else {
            qWarning() << "Invalid keyframeSource container";
        }
    }
    return version;
}

void writeKeyframesHeader(QCborStreamWriter &writer, QMetaType::Type type, int version = 1)
{
    // Root array
    writer.startArray();
    // header name
    writer.append("QTimelineKeyframes");
    // file version
    writer.append(version);
    // property type
    writer.append(type);
}

// Write QVariant value into CBOR in correct type.
void writeValue(QCborStreamWriter &writer, const QVariant &value)
{
    const QMetaType type = value.metaType();
    switch (type.id()) {
    case QMetaType::Bool: {
        bool b = value.toBool();
        writer.append(b);
        break;
    }
    case QMetaType::Int: {
        int i = value.toInt();
        writer.append(i);
        break;
    }
    case QMetaType::Float: {
        float f = value.toFloat();
        writer.append(f);
        break;
    }
    case QMetaType::Double: {
        double d = value.toDouble();
        writer.append(d);
        break;
    }
    case QMetaType::QVector2D: {
        QVector2D v = value.value<QVector2D>();
        writer.append(v.x());
        writer.append(v.y());
        break;
    }
    case QMetaType::QVector3D: {
        QVector3D v = value.value<QVector3D>();
        writer.append(v.x());
        writer.append(v.y());
        writer.append(v.z());
        break;
    }
    case QMetaType::QVector4D: {
        QVector4D v = value.value<QVector4D>();
        writer.append(v.x());
        writer.append(v.y());
        writer.append(v.z());
        writer.append(v.w());
        break;
    }
    case QMetaType::QQuaternion: {
        QQuaternion q = value.value<QQuaternion>();
        writer.append(q.scalar());
        writer.append(q.x());
        writer.append(q.y());
        writer.append(q.z());
        break;
    }
    case QMetaType::QColor: {
        QColor c = value.value<QColor>();
        writer.append(c.red());
        writer.append(c.green());
        writer.append(c.blue());
        writer.append(c.alpha());
        break;
    }
    case QMetaType::QRect: {
        QRect r = value.value<QRect>();
        writer.append(r.x());
        writer.append(r.y());
        writer.append(r.width());
        writer.append(r.height());
        break;
    }
    default: {
        qDebug() << "Not able to add:" << value << "of type:" << type.name();
        qDebug() << "Please add support for this type into generator.";
        break;
    }
    }
}

#endif // QQUICKKEYFRAMEDATAUTILS_H
