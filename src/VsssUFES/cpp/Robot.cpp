#include "Robot.h"
#include "GLDraw.h"
#include <cmath>
#include <GL/glut.h>
#include <vector>

//#define USING_DELAYED_MOTOR

using namespace std;

Robot::Robot(int idValue, double LValue, double xValue, double yValue, double thetaValue,
                                                                  double *colors, bool customColors)
{
    id = idValue;
    L = LValue;
    x = xValue;
    y = yValue;
    theta = thetaValue;
    u1 = u2 = 0;

    if(customColors)
    {
        for(int i = 0; i < 3; ++i)
        {
            frontColor[i] = colors[i];
            backColor[i] = colors[i+3];
        }
    }
    else
    {
        for(int i = 0; i < 3; ++i)
        {
            frontColor[i] = DEFAULT_FRONT_COLOR[i];
            backColor[i] = DEFAULT_BACK_COLOR[i];
        }
    }


}

void
Robot::draw()
{
    double centroid1x = 0.25*L*cos(theta) + x;
    double centroid1y = 0.25*L*sin(theta) + y;

    double centroid2x = -0.25*L*cos(theta) + x;
    double centroid2y = -0.25*L*sin(theta) + y;

    glColor3dv(frontColor);
    drawRectangle(L/2, L, centroid1x, centroid1y, theta, true);
    glColor3dv(backColor);
    drawRectangle(L/2, L, centroid2x, centroid2y, theta, true);

}

void
Robot::inputControls(double u1Value, double u2Value)
{
#define USING_DELAYED_MOTOR
#ifdef USING_DELAYED_MOTOR
    const double memoryFactor = 0.8;
    u1 = u1*memoryFactor + (1-memoryFactor) * u1Value;
    u2 = u2*memoryFactor + (1-memoryFactor) * u2Value;
#else
    u1 = u1Value;
    u2 = u2Value;
#endif /* USING_DELAYED_MOTOR */

    v = 0.5*(u1 + u2);
    omega = (u1 - u2)/L;
}

double sinct(double x) // This sin(x)/x can avoid zero division by taylor series
{
    if (fabs(x) < 1e-2)
    {
        auto x2 = x*x;
        return 1 - x2/6 + x2*x2/120; // taylor series aproximation
    }
    else
    {
        return sin(x)/x;
    }
}



void
Robot::evolve(double dt)
{
    v = 0.5*(u1 + u2);
    omega = (u1 - u2)/L;

    auto deltaTheta = omega * dt;

    theta += deltaTheta;


    while(theta >= M_PI)
    {
        theta -= 2*M_PI;
    }

    while(theta < -M_PI)
    {
        theta += 2*M_PI;
    }

    auto sincValue = sinct(0.5* deltaTheta);

    auto scaleFactor = dt*v*sincValue;

    x += scaleFactor*cos(theta + 0.5*deltaTheta);
    y += scaleFactor*sin(theta + 0.5*deltaTheta);

    return;
}


int
Robot::getId()
{
    return id;
}

vector<double>
Robot::getState()
{
    vector<double> state;

    state.push_back(x);
    state.push_back(y);
    state.push_back(theta);

    return state;
}