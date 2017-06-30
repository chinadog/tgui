/* ---------------------------------------------------------------------------
 * Copyright © 2017 The trifecta Company Ltd.
 * Contact: http://www.trifecta.ru/licensing/
 * Author: BM
 *
 * This file is part of the Trifecta.
 *
 * Trifecta is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Trifecta is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 * ---------------------------------------------------------------------------
 */

#ifndef TGUI_H
#define TGUI_H

#include "menu.h"
#include "loglistwidget.h"
#include <QWidget>
#include "config.h"
#include <TDB/TDB>
#include "circleprogressbar.h"
#include "systemmonitor.h"

class TGUI : public QWidget
{
    Q_OBJECT

public:
    TGUI(const QString& configPath, QWidget *parent = 0);
    ~TGUI();
private:
    void initInterface();
    void initStyle();
    void initConnections();
    void initFunctional();
    void removeDatabase();

    LogListWidget* logListWidget;
    Config m_config;
    TDB *m_tdb;
    QPushButton* m_button;
    CircleProgressBar *m_circleCpu;
    CircleProgressBar *m_circleMem;
    SystemMonitor* m_systemMonitor;

    QList<int> m_orders;
private slots:
    void setStartValues();
    void getData();
    void addLog(const QList<Event>&);
    void addLog(const Event&);
    void emitLog();
    void updateEvent(int id);
};

#endif // TGUI_H
