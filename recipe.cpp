/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "recipe.h"

#include <QDebug>

#if defined(Q_OS_ANDROID)
#  include <QtAndroidExtras/QAndroidJniObject>
#  include <QtAndroidExtras/QtAndroid>
#endif

Recipe::Recipe(int id, const QString &title, int time, const QString &directions, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_title(title)
    , m_time(time)
    , m_directions(directions)
{
}

#if defined(Q_OS_ANDROID)
void Recipe::createTimer(const QString &message) const
{
    QAndroidJniObject ACTION_SET_TIMER = QAndroidJniObject::getStaticObjectField<jstring>("android/provider/AlarmClock",
                                                                                          "ACTION_SET_TIMER");
    QAndroidJniObject intent("android/content/Intent",
                             "(Ljava/lang/String;)V",
                             ACTION_SET_TIMER.object<jstring>());

    QAndroidJniObject EXTRA_MESSAGE = QAndroidJniObject::getStaticObjectField<jstring>("android/provider/AlarmClock",
                                                                                       "EXTRA_MESSAGE");
    QAndroidJniObject messageObject = QAndroidJniObject::fromString(message);
    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                            EXTRA_MESSAGE.object<jstring>(),
                            messageObject.object<jstring>());

    QAndroidJniObject EXTRA_LENGTH = QAndroidJniObject::getStaticObjectField<jstring>("android/provider/AlarmClock",
                                                                                      "EXTRA_LENGTH");
    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;I)Landroid/content/Intent;",
                            EXTRA_LENGTH.object<jstring>(),
                            jint(m_time * 60));

    QAndroidJniObject EXTRA_SKIP_UI = QAndroidJniObject::getStaticObjectField<jstring>("android/provider/AlarmClock",
                                                                                      "EXTRA_SKIP_UI");
    intent.callObjectMethod("putExtra",
                            "(Ljava/lang/String;Z)Landroid/content/Intent;",
                            EXTRA_SKIP_UI.object<jstring>(),
                            jboolean(true));

    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject packageManager = activity.callObjectMethod("getPackageManager",
                                                                 "()Landroid/content/pm/PackageManager;");
    QAndroidJniObject componentName = intent.callObjectMethod("resolveActivity",
                                                              "(Landroid/content/pm/PackageManager;)Landroid/content/ComponentName;",
                                                              packageManager.object());
    if (componentName.isValid())
        QtAndroid::startActivity(intent, 0);
    else
        qWarning() << "Unable to resolve activity";
}
#else
void Recipe::createTimer() const
{
    qWarning() << "Not implemented on this platform";
}
#endif
