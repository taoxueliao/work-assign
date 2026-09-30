#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTemporaryDir>
#include <QUrl>
#include <QtTest>

#include "viewmodel/AssignController.h"

class PublishFlowTest : public QObject {
    Q_OBJECT

private slots:
    void publish_showsRowsOnTheRight()
    {
        QCoreApplication::setOrganizationName(QStringLiteral("WordAssignTest"));
        QCoreApplication::setApplicationName(QStringLiteral("PublishFlowTest"));

        QQmlApplicationEngine engine;
        AssignController controller;
        engine.rootContext()->setContextProperty(QStringLiteral("assignController"), &controller);
        engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
        QVERIFY(!engine.rootObjects().isEmpty());

        QObject *root = engine.rootObjects().first();
        QObject *countField = root->findChild<QObject *>(QStringLiteral("countField"));
        QObject *publishButton = root->findChild<QObject *>(QStringLiteral("publishButton"));
        QObject *resultList = root->findChild<QObject *>(QStringLiteral("resultList"));
        QVERIFY(countField);
        QVERIFY(publishButton);
        QVERIFY(resultList);

        QVERIFY(countField->setProperty("text", QStringLiteral("3")));
        QMetaObject::invokeMethod(publishButton, "clicked");

        QCOMPARE(resultList->property("count").toInt(), 3);
        QCOMPARE(controller.assignments()->rowCount(), 3);

        QObject *exportButton = root->findChild<QObject *>(QStringLiteral("exportButton"));
        QVERIFY(exportButton);
        QCOMPARE(exportButton->property("enabled").toBool(), true);

        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        QVERIFY(controller.exportTo(QUrl::fromLocalFile(directory.path())));

        QFile wordFile(directory.filePath(QStringLiteral("1月_1.txt")));
        QFile translationFile(directory.filePath(QStringLiteral("1月_1_translation.txt")));
        QVERIFY(wordFile.open(QIODevice::ReadOnly));
        QVERIFY(translationFile.open(QIODevice::ReadOnly));

        const auto linesOf = [](QFile &file) {
            QString text = QString::fromUtf8(file.readAll());
            if (text.startsWith(QChar(0xFEFF)))
                text.remove(0, 1);
            if (text.endsWith(QLatin1Char('\n')))
                text.chop(1);
            return text.split(QLatin1Char('\n'));
        };
        QCOMPARE(linesOf(wordFile).size(), 3);
        QCOMPARE(linesOf(translationFile).size(), 3);
        QCOMPARE(controller.exportDirectory(), QUrl::fromLocalFile(QDir(directory.path()).absolutePath()));
    }
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    PublishFlowTest test;
    return QTest::qExec(&test, argc, argv);
}

#include "PublishFlowTest.moc"
