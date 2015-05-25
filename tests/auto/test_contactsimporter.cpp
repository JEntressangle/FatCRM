#include <QtTest/QtTest>
#include <QDebug>
#include "contactsimporter.h"
#include <QSignalSpy>

class TestContactsImporter : public QObject
{
    Q_OBJECT
public:
private Q_SLOTS:
    void testSimilarAccountNames_data()
    {
        QTest::addColumn<QString>("account1");
        QTest::addColumn<QString>("account2");
        QTest::addColumn<bool>("expectedSame");

        QTest::newRow("equal") << "KDAB" << "KDAB" << true;
        QTest::newRow("inc_comma") << "KDAB, Inc." << "KDAB" << true;
        QTest::newRow("inc2_comma") << "KDAB" << "KDAB, Inc." << true;
        QTest::newRow("sa_nocomma") << "KDAB" << "KDAB S.A." << true;
        QTest::newRow("AB_nospace") << "KDAB" << "KD" << false;
        QTest::newRow("sas_sa") << "KDAB S.A.S." << "KDAB S.A." << true;
    }

    void testSimilarAccountNames()
    {
        QFETCH(QString, account1);
        QFETCH(QString, account2);
        QFETCH(bool, expectedSame);

        SugarAccount ac1;
        ac1.setName(account1);
        SugarAccount ac2;
        ac2.setName(account2);
        QCOMPARE(ac1.isSameAccount(ac2), expectedSame);
        if (expectedSame)
            QCOMPARE(ac1.key(), ac2.key());
        else
            QVERIFY(ac1.key() != ac2.key());
    }

    void testMultipleAccounts_data()
    {
        QTest::addColumn<QString>("csv");
        QTest::addColumn<QStringList>("accountNames");

        QTest::newRow("empty") << "" << QStringList();
        QTest::newRow("one") << "David,Faure,Mr,12345,david.faure@example.com,KDAB,\"32, street name\",Vedène,84000,,France,FR 12345" << (QStringList() << "KDAB");
        QTest::newRow("dupe") << "David,Faure,Mr,12345,david.faure@example.com,KDAB,\"32, street name\",Vedène,84000,,France,FR 12345\nClone,Faure,Mr,12345,clone.faure@example.com,KDAB,\"32, street name\",Vedène,84000,,France,FR 12345" << (QStringList() << "KDAB");

        QTest::newRow("2+1") << "David,Faure,Mr,12345,david.faure@example.com,KDAB,\"32, street name\",Vedène,84000,,France,FR 12345\nClone,Faure,Mr,12345,clone.faure@example.com,KDAB Inc.,\"32, street name\",Vedène,84000,,France,FR 12345\nClone,Faure,Mr,12345,clone.faure@foo.com,Foo,\"32, street name\",Vedène,84000,,France,FR 12345" << (QStringList() << "KDAB" << "Foo");
    }

    void testMultipleAccounts()
    {
        QFETCH(QString, csv);
        QFETCH(QStringList, accountNames);

        ContactsImporter importer;
        QTemporaryFile file;
        writeTempFile(file, csv.toUtf8());
        QVERIFY(importer.importFile(file.fileName()));
        const QVector<SugarAccount> accounts = importer.accounts();
        QCOMPARE(extractAccountNames(accounts), accountNames);
    }

    void testImportingAccounts()
    {
        ContactsImporter importer;
        QTemporaryFile file;
        writeTempFile(file,
        "David,Faure,Mr,12345,david.faure@kdab.com,KDAB,\"32, street name\",Vedène,84000,,France,FR 12345");
        QVERIFY(importer.importFile(file.fileName()));
        const QVector<SugarAccount> accounts = importer.accounts();
        QCOMPARE(accounts.size(), 1);
        const SugarAccount account = accounts.at(0);
        QCOMPARE(account.name(), QString("KDAB"));
        QCOMPARE(account.billingAddressStreet(), QString::fromUtf8("32, street name"));
        QCOMPARE(account.billingAddressCity(), QString::fromUtf8("Vedène"));
        QCOMPARE(account.billingAddressCountry(), QString::fromUtf8("France"));
        QCOMPARE(account.billingAddressPostalcode(), QString::fromUtf8("84000"));
        QCOMPARE(account.billingAddressState(), QString());
    }

private:
    void writeTempFile(QTemporaryFile &file, const QByteArray& data) {
        QVERIFY(file.open());
        const QByteArray header = "First Name,Last Name,Title,Phone,Email,Company Name,Address,City,Zipcode,State/Province,Country,EU VAT ID\n";
        QCOMPARE(file.write(header), qlonglong(header.size()));
        QCOMPARE(file.write(data), qlonglong(data.size()));
        QVERIFY(file.seek(0));
    }

    static QStringList extractAccountNames(const QVector<SugarAccount> &accounts)
    {
        QStringList ret;
        foreach(const SugarAccount &account, accounts) {
            ret << account.name();
        }
        return ret;
    }
};

QTEST_MAIN(TestContactsImporter)
#include "test_contactsimporter.moc"
