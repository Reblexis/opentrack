/* Copyright (c) 2023 Eyeware Tech SA https://www.eyeware.tech
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 */

#pragma once

#include "api/plugin-api.hpp"

#include "ui_clearsight.h" // TODO: ??

#include "clearsight_client.hpp" // TODO: Is this correct import?

#include <QObject>
#include <QMutex>

class clearsight_tracker : public QObject, public ITracker
{
    Q_OBJECT

public:
    clearsight_tracker();
    ~clearsight_tracker() override;
    module_status start_tracker(QFrame* frame) override;
    void data(double *data) override;

private:
    ClearsightClient client;
    QMutex mtx;

    double last_pitch_deg = 0.0;
    double last_roll_deg = 0.0;
    double last_yaw_deg = 0.0;
    double last_translation_x_cm = 0.0;
    double last_translation_y_cm = 0.0;
    double last_translation_z_cm = 0.0;
};

class clearsight_dialog : public ITrackerDialog
{
    Q_OBJECT

public:
    clearsight_dialog();
    void register_tracker(ITracker * x) override { tracker = static_cast<clearsight_tracker*>(x); }
    void unregister_tracker() override { tracker = nullptr; }

private:
    Ui::clearsight_ui ui;
    clearsight_tracker* tracker = nullptr;

private Q_SLOTS:
    void doOK();
};

class clearsight_metadata : public Metadata
{
    Q_OBJECT
    QString name() override { return QString("Clearsight"); }
    QIcon icon() override { return QIcon(":/images/clearsight_logo.png"); }
};
