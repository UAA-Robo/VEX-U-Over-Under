#include "AutoDrive.h"


AutoDrive::AutoDrive(Hardware *hardware, RobotConfig *robotConfig, Telemetry *telemetry) : Drive(hardware, robotConfig, telemetry) {}

void AutoDrive::drive() {}

void AutoDrive::rotateToRelativeAngle(double angle) // Based on ENCODERS,
{

    double numberDriveTrainRevolutions = angle * (rc->DRIVETRAINWIDTH) * M_PI / (360 * rc->WHEELCIRC);
    double revolutionsLeftWheels = -numberDriveTrainRevolutions;
    double revolutionsRightWheels = numberDriveTrainRevolutions;

    std::pair<double, double> vel = calculateDriveTrainVel({0, rc->autoRotateVelPercent});

    hw->left_drivetrain_motors.spinFor(revolutionsLeftWheels, vex::rotationUnits::rev, vel.first, vex::velocityUnits::pct, false);
    hw->right_drivetrain_motors.spinFor(revolutionsRightWheels, vex::rotationUnits::rev, vel.second, vex::velocityUnits::pct);
    while (fabs(hw->left_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0 || fabs(hw->right_drivetrain_motors.velocity(vex::velocityUnits::pct)) > 0)
        ; // Blocks other tasks from starting
}

void AutoDrive::rotateToHeading(double heading)
{
    // Corrects heading to be from 0-360 from the x axis counterclockwise if applicable
    heading = fmod(heading, 360);
    if (heading < 0)
        heading += 360;

    if (IS_USING_INERTIA_HEADING)
    {
        // turns heading from counterclockwise to clockwise bc smartDriveTrain.turnToHeading is measured clockwisee
        int clockwiseHeading = fmod(360.0 - heading, 360.0);
        clockwiseHeading = (heading >= 0 ? heading : heading + 360.0);

        hw->smartDriveTrain.turnToHeading(clockwiseHeading, vex::degrees, rc->autoRotateVelPercent, vex::velocityUnits::pct);
    }
    else // checks encoder heading with gps
    {
        double angleToRotate = heading - tm->getCurrHeading();
        angleToRotate = fmod(angleToRotate, 360); // make sure the angle to rotate is -360 to 360

        // Determines whether to rotate left or right based on the  shortest distance
        if (360 - fabs(angleToRotate) < angleToRotate)
            angleToRotate = angleToRotate - 360;
        rotateToRelativeAngle(angleToRotate + robotAngleOffset);
    }
    tm->setCurrHeading(heading);
    tm->headingErrorCorrection();
    tm->positionErrorCorrection();
}

void AutoDrive::rotateToPosition(std::pair<double, double> finalPosition, bool ISBACKROTATION)
{
    // if (IS_USING_GPS_POSITION) tm->setCurrPosition(tm->getGPSPosition());
    double heading = tm->getHeadingBtwnPoints(tm->getCurrPosition(), finalPosition);

    if (ISBACKROTATION)
        heading -= 180;
    rotateToHeading(heading);
}

// void AutoDrive::rotateToPosition(GameElement *gameElement)
// {
//     // if (IS_USING_GPS_POSITION) tm->setCurrPosition(tm->getGPSPosition());
//     double heading = tm->getHeadingBtwnPoints(tm->getCurrPosition(), gameElement->GetPositionWithMinOffset());

//     if (gameElement->GetAlignment())
//         heading -= 180;
//     rotateToHeading(heading);
// }

// void AutoDrive::rotateAndDriveToPosition(GameElement *element)
// {
//     // if (IS_USING_GPS_POSITION) tm->setCurrPosition(tm->getGPSPosition());

//     std::pair<double, double> position = element->GetPositionWithMinOffset();

//     rotateToPosition(element);
//     double distanceToPosition = tm->getDistanceBtwnPoints(tm->getCurrPosition(), position); // inches
//     if (element->GetAlignment())
//         distanceToPosition = -distanceToPosition;
//     moveDriveTrainDistance({rc->autoDriveVelPercent, 0}, distanceToPosition);
// }

void AutoDrive::rotateAndDriveToPosition(std::pair<double, double> position, bool ISBACKTOPOSITION)
{

    rotateToPosition(position, ISBACKTOPOSITION);

    // if (IS_USING_GPS_HEADING) tm->setCurrPosition(tm->getGPSPosition());

    double distanceToPosition = tm->getDistanceBtwnPoints(tm->getCurrPosition(), position); // inches
    if (ISBACKTOPOSITION)
        distanceToPosition = -distanceToPosition;
    moveDriveTrainDistance({rc->autoDriveVelPercent, 0}, distanceToPosition); // Drive at autoDriveVelPercent% velocity
}

