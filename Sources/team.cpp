#include "../Headers/team.h"

Team::Team()
{
    // Do nothing
}

Team::Team(int teamNumber)
{
    number = teamNumber;
    name = "Amazing name here :)";
    place = 1;

    teleOpAbilities[TELEOP_NUM_ABILITIES];
    autoAbilities[AUTO_NUM_ABILITIES];

    other = "";

    isCompatible = false;
}

int Team::getNumber()
{
    return number;
}

void Team::setNumber(int num)
{
    number = num;

    return;
}

string Team::getName()
{
    return name;
}

void Team::setName(string str)
{
    name = str;

    return;
}

int Team::getPlace()
{
    return place;
}

void Team::setPlace(int num)
{
    place = num;

    return;
}

int Team::getTeleOpElement(int i)
{
    return teleOpAbilities[i];
}

void Team::setTeleOpElement(int i, bool num)
{
    teleOpAbilities[i] = num;
    return;
}

int Team::getAutoElement(int i)
{
    return autoAbilities[i];
}

void Team::setAutoElement(int i, bool num)
{
    autoAbilities[i] = num;
    return;
}

void Team::addToPathList(vector <QPointF> path)
{
    myPaths.push_back(path);
    return;
}

vector<QPointF> Team::getSinglePath(int i)
{
    return myPaths[i];
}

vector<vector<QPointF> > Team::getPaths()
{
    return myPaths;
}

string Team::getOther()
{
    return other;
}

void Team::setOther(string str)
{
    other = str;
    return;
}

void Team::writeToFile(string fileName)
{
    ofstream myFile;

    // Write to file, WOW
    myFile.open(fileName , std::ios_base::app , ios::binary);

    // Denote start of a new team
    myFile << "~" << endl;
    myFile << "Number: " << number << endl;
    myFile << "Name: " << name << endl;
    myFile << "Place: " << place << endl;
    myFile << "TeleOp Abilities: ";
    for(int i = 0; i < TELEOP_NUM_ABILITIES; i ++)
    {
        myFile << teleOpAbilities[i];
        if(!(i == TELEOP_NUM_ABILITIES - 1))
            myFile << ",";
    }
    myFile << endl;

    myFile << "Auto Abilities: ";
    for(int i = 0; i < AUTO_NUM_ABILITIES; i ++)
    {
        myFile << autoAbilities[i];
        if(!(i == AUTO_NUM_ABILITIES - 1))
            myFile << ",";
    }
    myFile << endl;

    myFile << "Auto Paths: ";
    for(int i = 0; i < myPaths.size(); i ++)
    {
        for(int j = 0; j < getSinglePath(i).size(); j ++)
        {
            myFile << getSinglePath(i)[j].x() << ",";
            myFile << getSinglePath(i)[j].y();

            if(!(j == getSinglePath(i).size() - 1))
                myFile << "+";
        }
        if(!(i == myPaths.size() - 1))
            myFile << "!";
    }
    myFile << endl;

    myFile << "Other: " << other << endl;

    myFile << "Is Compatible: " << isCompatible << endl;

    // Denote end of team
    myFile << "#" << endl;

    myFile.close();
}

int Team::readFromFile(string fileName , int targetReadNumber , vector <Team> alreadyRead)
{
    int teamsRead = -1;

    int returnCode = 0;

    bool readThis = true;

    bool readPath = true;

    const string TEAM_DEFAULT_START = "@";
    const string TEAM_START = "~";
    const string TEAM_END = "#";
    const string NUMBER_STRING = "Number: ";
    const string NAME_STRING = "Name: ";
    const string PLACE_STRING = "Place: ";
    const string TELEOP_ABILITIES_STRING = "TeleOp Abilities: ";
    const string AUTO_ABILITIES_STRING = "Auto Abilities: ";
    const string AUTO_PATHS_STRING = "Auto Paths: ";
    const string OTHER_STRING = "Other: ";
    const string COMPATIBLE_STRING = "Is Compatible: ";

    string temp;

    ifstream myFile;

    QPointF point;

    vector <QPointF> path;

    myFile.open(fileName);

    myPaths.clear();
    path.clear();

    // While there's more and we're not done
    for(string line; getline(myFile , line);)
    {
        // If the line is the team start character
        if(!line.compare(TEAM_START))
        {
            teamsRead ++;
            readThis = teamsRead == targetReadNumber;
        }

        // If we're supposed to read this data
        if(readThis)
        {
            temp = line;

            // If we are reading the number data
            if(temp.find(NUMBER_STRING) != string::npos)
            {
                temp = temp.substr(NUMBER_STRING.length() , line.length() - NUMBER_STRING.length());

                // Check duplicates
                for(int i = 0; i < alreadyRead.size() && returnCode != 2; i ++)
                {
                    if(stoi(temp) == alreadyRead[i].number)
                    {
                        returnCode = 2;
                        return 2;
                    }
                }
                if(returnCode != 2)
                    number = stoi(temp);
            }
            // If the team we're reading isn't a duplicate
            if(returnCode != 2)
            {
                // Read the rest
                if(temp.find(NAME_STRING) != string::npos)
                {
                    temp = temp.substr(NAME_STRING.length() , line.length() - NAME_STRING.length());
                    name = temp;
                }
                else if(temp.find(PLACE_STRING) != string::npos)
                {
                    temp = temp.substr(PLACE_STRING.length() , line.length() - PLACE_STRING.length());
                    place = stoi(temp);
                }
                else if(temp.find(TELEOP_ABILITIES_STRING) != string::npos)
                {
                    temp = temp.substr(TELEOP_ABILITIES_STRING.length() , line.length() - TELEOP_ABILITIES_STRING.length());

                    for(int i = 0; i < TELEOP_NUM_ABILITIES; i ++)
                    {
                        teleOpAbilities[i] = stoi(temp.substr(i * 2 , 1));
                    }
                }
                else if(temp.find(AUTO_ABILITIES_STRING) != string::npos)
                {
                    temp = temp.substr(AUTO_ABILITIES_STRING.length() , line.length() - AUTO_ABILITIES_STRING.length());

                    for(int i = 0; i < AUTO_NUM_ABILITIES; i ++)
                    {
                        autoAbilities[i] = stoi(temp.substr(i * 2 , 1));
                    }
                }
                else if(temp.find(AUTO_PATHS_STRING) != string::npos)
                {
                    temp = temp.substr(AUTO_PATHS_STRING.length() , line.length() - AUTO_PATHS_STRING.length());

                    if(temp.size() == 0)
                        readPath = false;

                    // The really hard part :(
                    for(int i = 0; temp.length() != 0 && readPath; i ++)
                    {
                        // Get X coordinate
                        point.setX(stoi(temp.substr(0 , temp.find(","))));
                        temp.erase(0 , temp.find(",") + 1);

                        // Get Y coordinate
                        if(temp.find("+") == string::npos)
                        {
                            point.setY(stoi(temp));
                            temp.erase();
                        }
                        else if(temp.find("!") < temp.find("+"))
                        {
                            point.setY(stoi(temp.substr(0 , temp.find("!"))));

                            myPaths.push_back(path);
                            path.clear();

                            temp.erase(0 , temp.find("!") + 1);
                        }
                        else
                        {
                            point.setY(stoi(temp.substr(0 , temp.find("+"))));
                            temp.erase(0 , temp.find("+") + 1);
                        }

                        path.push_back(point);
                    }
                    if(readPath)
                        myPaths.push_back(path);
                    // Actually, that wasn't too hard :P (I think)
                    // BUG FIXING IS HELL I WAS WRONG ^
                }
                else if(temp.find(OTHER_STRING) != string::npos)
                {
                    temp = temp.substr(OTHER_STRING.length() , temp.length() - OTHER_STRING.length());

                    other = temp;
                }
                else if(temp.find(COMPATIBLE_STRING) != string::npos)
                {
                    temp = temp.substr(COMPATIBLE_STRING.length() , temp.length() - COMPATIBLE_STRING.length());

                    isCompatible = stoi(temp);
                }
                else if(temp.find(TEAM_END) != string::npos)
                {
                    returnCode = 1;
                    return returnCode;
                }
            }
        }
    }

    returnCode = 0;
    return returnCode;

    myFile.close();
}

void Team::erasePath(int index)
{
    myPaths.erase(myPaths.begin() + index);

    return;
}

void Team::eraseAllPaths()
{
    myPaths.clear();

    return;
}
