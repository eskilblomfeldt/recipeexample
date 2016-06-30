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
