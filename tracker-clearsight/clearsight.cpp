/* Copyright (c) 2023 Eyeware Tech SA https://www.eyeware.tech
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 */

#include "clearsight.h"

#include <QMutexLocker>

constexpr double rad_to_deg = 180.0 / M_PI;

clearsight_tracker::clearsight_tracker(): client("127.0.0.1", 8692)
{
}

clearsight_tracker::~clearsight_tracker()
{
    QMutexLocker lck(&mtx);
    client.closeConnection();
}

module_status clearsight_tracker::start_tracker(QFrame* videoframe)
{
    QMutexLocker lck(&mtx);
    try
    {
        client.connectToServer();
    }
    catch (...)
    {
        return error("Clearsight tracker initialization has failed");
    }

    return status_ok();
}

void clearsight_tracker::data(double *data)
{
     QMutexLocker lck(&mtx);

     TrackingData tracking_info = client.getLatestData();
     if (tracking_info.is_valid)
     {
        last_yaw_deg = tracking_info.head_yaw * rad_to_deg;
        last_pitch_deg = tracking_info.head_pitch * rad_to_deg;
        last_roll_deg = tracking_info.head_roll * rad_to_deg;
        last_translation_x_cm = tracking_info.eye_x;
        last_translation_y_cm = tracking_info.eye_y;
        last_translation_z_cm = tracking_info.eye_z;
        //std::cout<<"eye_z: "<<tracking_info.eye_z<<std::endl;
     }

     data[TX] = last_translation_x_cm;
     data[TY] = -last_translation_y_cm; // TODO: Translation as in movement or current position?
     data[TZ] = last_translation_z_cm;
     data[Yaw] = last_yaw_deg;
     data[Pitch] = last_pitch_deg;
     data[Roll] = last_roll_deg;
}

clearsight_dialog::clearsight_dialog()
{
    ui.setupUi(this);

    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(doOK()));
}

void clearsight_dialog::doOK()
{
    close();
}

OPENTRACK_DECLARE_TRACKER(clearsight_tracker, clearsight_dialog, clearsight_metadata)
