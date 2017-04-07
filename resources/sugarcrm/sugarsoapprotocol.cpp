/*
  This file is part of FatCRM, a desktop application for SugarCRM written by KDAB.

  Copyright (C) 2017 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Authors: Jeremy Entressangle <jeremy.entressangle@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sugarsoapprotocol.h"
#include "sugarjob.h"
#include "sugarsession.h"
#include <QNetworkReply>
#include <KLocalizedString>
#include <KDebug>

SugarSoapProtocol::SugarSoapProtocol()
{
}

int SugarSoapProtocol::login(const QString &user, const QString &password, const QString &host, QString &sessionId, QString &errorMessage)
{
    Q_ASSERT(mSoap != nullptr);

    const QByteArray passwordHash = password.toUtf8();

    KDSoapGenerated::TNS__User_auth userAuth;
    userAuth.setUser_name(user);
    userAuth.setPassword(QString::fromAscii(passwordHash));
    userAuth.setVersion(QLatin1String(".01"));

    KDSoapGenerated::TNS__Set_entry_result entry_result = mSoap->login(userAuth, QLatin1String("FatCRM"));
    if (entry_result.error().number() == "0") {
        sessionId = entry_result.id();
        return KJob::NoError;
    } else {
        errorMessage = i18nc("@info:status", "Login for user %1 on %2 failed: %3", user, host, mSoap->lastError());
        int faultcode = mSoap->lastErrorCode();
        if (faultcode == QNetworkReply::UnknownNetworkError ||
           faultcode == QNetworkReply::HostNotFoundError) {
            return SugarJob::CouldNotConnectError;
        } else {
            return SugarJob::LoginError;
        }
    }
}

void SugarSoapProtocol::logout(const QString &sessionId)
{
    if (sessionId.isEmpty() && mSoap != nullptr) {
        KDSoapGenerated::TNS__Error_value errorValue = mSoap->logout(sessionId);
        if (errorValue.number() != "0") {
            kDebug() << "logout returned error" << errorValue.number() << errorValue.name() << errorValue.description();
        }
        if (!mSoap->lastError().isEmpty()) {
            kDebug() << "logout had fault" << mSoap->lastError();
        }
    }
}

