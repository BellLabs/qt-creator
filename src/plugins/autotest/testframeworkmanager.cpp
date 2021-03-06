/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "testframeworkmanager.h"

#include "autotestconstants.h"
#include "autotestplugin.h"
#include "itestsettings.h"
#include "testsettings.h"

#include <utils/algorithm.h>
#include <utils/qtcassert.h>

#include <QSettings>

using namespace Utils;

namespace Autotest {

static TestFrameworkManager *s_instance = nullptr;

TestFrameworkManager::TestFrameworkManager()
{
    s_instance = this;
}

TestFrameworkManager::~TestFrameworkManager()
{
    qDeleteAll(m_registeredFrameworks);
    s_instance = nullptr;
}

bool TestFrameworkManager::registerTestFramework(ITestFramework *framework)
{
    QTC_ASSERT(framework, return false);
    QTC_ASSERT(!m_registeredFrameworks.contains(framework), return false);
    // TODO check for unique priority before registering
    m_registeredFrameworks.append(framework);
    Utils::sort(m_registeredFrameworks, &ITestFramework::priority);
    return true;
}

void TestFrameworkManager::activateFrameworksFromSettings(const Internal::TestSettings *settings)
{
    for (ITestFramework *framework : qAsConst(s_instance->m_registeredFrameworks)) {
        framework->setActive(settings->frameworks.value(framework->id(), false));
        framework->setGrouping(settings->frameworksGrouping.value(framework->id(), false));
    }
}

TestFrameworks TestFrameworkManager::registeredFrameworks()
{
    return s_instance->m_registeredFrameworks;
}

ITestFramework *TestFrameworkManager::frameworkForId(Id frameworkId)
{
    return Utils::findOrDefault(s_instance->m_registeredFrameworks,
            [frameworkId](ITestFramework *framework) {
                return framework->id() == frameworkId;
            });
}

void TestFrameworkManager::synchronizeSettings(QSettings *s)
{
    Internal::AutotestPlugin::settings()->fromSettings(s);
    for (ITestFramework *framework : qAsConst(m_registeredFrameworks)) {
        if (ITestSettings *fSettings = framework->testSettings())
            fSettings->fromSettings(s);
    }
}

} // namespace Autotest
