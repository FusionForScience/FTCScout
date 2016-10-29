#ifndef TEAM_H
#define TEAM_H

#include "drawpath.h"

#include <fstream>
#include <QDebug>

using namespace std;

class Team
{
public:
    // Constructors
    Team();
    Team(int teamNumber);

    int getNumber();
    void setNumber(int num);

    string getName();
    void setName(string str);

    int getPlace();
    void setPlace(int num);

    int getTeleOpElement(int i);
    void setTeleOpElement(int i , bool num);

    int getAutoElement(int i);
    void setAutoElement(int i , bool num);

    void addToPathList(vector <QPointF> path);
    vector <QPointF> getSinglePath(int i);
    vector <vector <QPointF>> getPaths();

    string getOther();
    void setOther(string str);

    void writeToFile(string fileName);
    int readFromFile(string fileName , int targetReadNumber , vector <Team> alreadyRead);

    void erasePath(int index);
    void eraseAllPaths();

    static const int TELEOP_NUM_ABILITIES = 3;
    static const int AUTO_NUM_ABILITIES = 3;

private:
    int number;
    string name;
    int place;
    bool teleOpAbilities[TELEOP_NUM_ABILITIES] = {false};
    bool autoAbilities[AUTO_NUM_ABILITIES] = {false};
    vector <vector <QPointF>> myPaths;
    string other;
    bool isCompatible;
};

#endif // TEAM_H
