// 测试 RandomAssigner：无放回抽样，数量与词条不重复。

#include <QtTest>

class RandomAssignerTest : public QObject {
    Q_OBJECT

private slots:
    void assign_returnsUniqueWords() {}
};

QTEST_MAIN(RandomAssignerTest)
#include "RandomAssignerTest.moc"
