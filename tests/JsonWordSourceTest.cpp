// 测试 JsonWordSource：从 words.json 读出单词和翻译。

#include <QtTest>

class JsonWordSourceTest : public QObject {
    Q_OBJECT

private slots:
    void load_returnsWords() {}
};

QTEST_MAIN(JsonWordSourceTest)
#include "JsonWordSourceTest.moc"
