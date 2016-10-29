#include "../Headers/mainwindow.h"
#include "../Headers/drawpath.h"

#include <ui_mainwindow.h>

using namespace std;

/* Constructors and Destructors */
/*****************************************************************/
/*****************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Install event filter
    qApp->installEventFilter(this);

    // Open data file in truncated mode
    ifstream myFile(DATA_FILE.c_str() , std::ios::ate);

    // If the file is empty
    if((int)(myFile.tellg()) == 0)
    {
        // Holds user dialog response
        bool ok;

        // Loop while the user rejects the dialog
        do
        {
            // Get user input and store into number
            int number = QInputDialog::getInt(this, tr("Alert:"),
                                          tr("Enter your team number:"), 0000, 0, 99999, 0, &ok);

            // If the user accepts the dialog
            if(ok)
            {
                // Create a new Team object with a number of the user input
                Team defaultTeam(number);

                // Save changes to file
                defaultTeam.writeToFile(DATA_FILE.c_str());
            }
        }while(!ok);
    }

    // Create a new image
    field = new QImage(FIELD_IMAGE.c_str());
    // Create a new pixmap from the image
    item = new QPixmap(QPixmap::fromImage(*field));
    // Draws scene
    drawField();

    // Updates allTeams vector
    on_refreshTeamListButton_clicked();

    // Sets current team to your team
    currentTeamIndex = 0;

    // Updates UI to show your team's details
    refreshUI(currentTeamIndex);
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}





/* Member Functions */
/*****************************************************************/
/*****************************************************************/
void MainWindow::drawField()
{
    delete scene;
    scene = new DrawPath();

    connect(scene , SIGNAL(userHasDrawn()) , this , SLOT(enableAddButton()));

    scene->addPixmap(*item);
    ui->autonomousPlan->setScene(scene);

    return;
}

void MainWindow::drawAutoPath(int teamIndex, int autoIndex , QString color)
{
    const int drawRadius = 3;

    QPen myPen;

    myPen.setColor(color);


    for(int i = 0; i < allTeams[teamIndex].getSinglePath(autoIndex).size(); i ++)
    {
        scene->addEllipse(allTeams[teamIndex].getSinglePath(autoIndex)[i].x()
                          , allTeams[teamIndex].getSinglePath(autoIndex)[i].y()
                          , drawRadius * 2.0 , drawRadius * 2.0 , myPen , QBrush(Qt::SolidPattern));
    }

    return;
}

void MainWindow::addTeam(Team team)
{
    allTeams.push_back(team);

    return;
}

void MainWindow::updateFile(QString filename)
{
    // Delete file so we don't append to it or leave leftover information
    // Fixme... There's a better way to do this
    remove(DATA_FILE.c_str());

    // Write each team to the file
    for(int i = 0; i < allTeams.size(); i ++)
    {
        allTeams[i].writeToFile(DATA_FILE.c_str());
    }

    return;
}

void MainWindow::refreshUI(int teamIndex)
{
    // Resets scene
    drawField();

    // Enable or disable delete team button
    if(teamIndex == 0)
        ui->deleteTeamButton->setEnabled(false);
    else
        ui->deleteTeamButton->setEnabled(true);

    // Data field edit
    ui->nameEditEdit->setText(QString::fromStdString(allTeams[teamIndex].getName()));
    ui->placeEditEdit->setText(QString::number(allTeams[teamIndex].getPlace()));

    ui->teleOpEdit1->setChecked(allTeams[teamIndex].getTeleOpElement(0));
    ui->teleOpEdit2->setChecked(allTeams[teamIndex].getTeleOpElement(1));
    ui->teleOpEdit3->setChecked(allTeams[teamIndex].getTeleOpElement(2));

    ui->autoEdit1->setChecked(allTeams[teamIndex].getAutoElement(0));
    ui->autoEdit2->setChecked(allTeams[teamIndex].getAutoElement(1));
    ui->autoEdit3->setChecked(allTeams[teamIndex].getAutoElement(2));

    ui->otherEditEdit->setText(QString::fromStdString(allTeams[teamIndex].getOther()));

    // Reset tabs
    ui->drawModeTabs->setCurrentIndex(0);

    // If the current team isn't yours, then show the Compatible checkbox
    if(currentTeamIndex == 0)
        ui->isCompatible->hide();
    else
        ui->isCompatible->show();

    // Reset comboboxes
    ui->yourTeamAutos->clear();
    ui->theirTeamAutos->clear();
    ui->pathsBox->clear();

    // Reset Paths combobox
    ui->pathsBox->addItem("New");
    for(int i = 0; i < allTeams[teamIndex].getPaths().size(); i ++)
    {
        ui->pathsBox->addItem(QString::number(i + 1));
    }

    // Reset Your Team Autos combobox
    ui->yourTeamAutos->addItem("Team " + QString::number(allTeams[0].getNumber()));
    for(int i = 0; i < allTeams[0].getPaths().size(); i ++)
    {
        ui->yourTeamAutos->addItem(QString::number(i + 1));
    }

    // Reset Their Team Autos combobox
    ui->theirTeamAutos->addItem("Team " + QString::number(allTeams[teamIndex].getNumber()));
    for(int i = 0; i < allTeams[teamIndex].getPaths().size(); i ++)
    {
        ui->theirTeamAutos->addItem(QString::number(i + 1));
    }

    return;
}





/* Event Filter */
/*****************************************************************/
/*****************************************************************/
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // If the Other text edit goes out of focus, update Team and write to file
    if(obj == ui->otherEditEdit && event->type() == QEvent::FocusOut)
    {
        allTeams[currentTeamIndex].setOther(ui->otherEditEdit->toPlainText().toStdString());
        updateFile(DATA_FILE.c_str());
    }

    return false;
}





/* Slots */
/*****************************************************************/
/*****************************************************************/
// Team List
void MainWindow::on_addButton_clicked()
{
    // Result of the new team dialog
    bool ok;

    // Create a dialog asking for team number
    int number = QInputDialog::getInt(this, tr("New Team"),
                                  tr("Enter the team number:"), 0000, 0, 99999, 0, &ok);

    // If the user decides to create a new team
    if(ok)
    {
        // Create new Team object with number as user input
        Team newTeam(number);

        // Append team to file
        newTeam.writeToFile(DATA_FILE.c_str());

        // That way, if the team number is duplicate, it will be discarded
        // when refreshed
        on_refreshTeamListButton_clicked();

        // Set team list selection on the last element in the list
        ui->teamList->setCurrentRow(static_cast <int>(allTeams.size()) - 2);

        // Update current team to reflect element index
        currentTeamIndex = ui->teamList->count();

        // Update UI to show team info
        refreshUI(currentTeamIndex);
    }

    return;
}

void MainWindow::on_deleteTeamButton_clicked()
{
    // Delete the team from the allTeams vector
    allTeams.erase(allTeams.begin() + ui->teamList->currentRow() + 1);

    // Delete team from list widget
    delete ui->teamList->currentItem();

    // Update file to reflect deletion
    updateFile(DATA_FILE.c_str());

    // If there's still stuff in the list
    // Set the current team index to 1 less because
    // the list will select the item directly preceding the deleted one
    // If the list is empty, default team is selected
    if(ui->teamList->count() > 0)
        currentTeamIndex --;

    // Refresh UI to see team details
    refreshUI(currentTeamIndex);

    return;
}

void MainWindow::on_refreshTeamListButton_clicked()
{
    Team* newTeam = new Team();

    // Number of duplicates encountered
    int numDupes = 0;

    // Start i from allTeams size because allTeams may already have elements
    for(int i = static_cast <int>(allTeams.size()); newTeam->readFromFile(DATA_FILE.c_str() , i , allTeams) != 0; i ++)
    {
        // If the i-th team is found and is not a duplicate, add Team to allTeams
        if(newTeam->readFromFile(DATA_FILE.c_str() , i , allTeams) == 1)
        {
            addTeam(*newTeam);

            // If Team is not the first Team, then add to the list widget
            if(i != 0)
                ui->teamList->addItem(QString::number(allTeams[i - numDupes].getNumber()));
        }
        // If the i-th team is a duplicate, then ignore and add one to the number of duplicates
        else if(newTeam->readFromFile(DATA_FILE.c_str() , i , allTeams) == 2)
            numDupes ++;

        // Delete object and create a new one
        delete newTeam;
        newTeam = new Team();
    }

    // Delete object before leaving scope
    delete newTeam;

    return;
}

void MainWindow::on_yourTeamButton_clicked()
{
    // Move focus out of list widget
    ui->teamList->clearSelection();

    // Set auto compare to defaults
    ui->yourTeamAutos->setCurrentIndex(0);
    ui->theirTeamAutos->setCurrentIndex(0);

    // Set team index to that of your team
    currentTeamIndex = 0;

    // Update UI to show your team details
    refreshUI(currentTeamIndex);

    return;
}

void MainWindow::on_teamList_itemClicked(QListWidgetItem *item)
{
    // I don't need this, and I don't like compiler warnings
    item = 0;

    // Switches to default so that attempts to render don't crash program
    ui->yourTeamAutos->setCurrentIndex(0);
    ui->theirTeamAutos->setCurrentIndex(0);

    // Get current row selected
    int currentRow = ui->teamList->currentRow();

    // Add 1 because of the default team not being on the list
    currentRow ++;

    // Set current team to the selected row plus 1
    currentTeamIndex = currentRow;

    // Update UI to show team details
    refreshUI(currentTeamIndex);

    return;
}


// Editing
void MainWindow::on_nameEditEdit_editingFinished()
{
    allTeams[currentTeamIndex].setName(ui->nameEditEdit->text().toStdString());
    updateFile(DATA_FILE.c_str());

    return;
}

void MainWindow::on_placeEditEdit_editingFinished()
{
    allTeams[currentTeamIndex].setPlace(ui->placeEditEdit->text().toInt());
    updateFile(DATA_FILE.c_str());

    return;
}

void MainWindow::on_teleOpEdit1_clicked(bool checked)
{
    allTeams[currentTeamIndex].setTeleOpElement(0 , checked);
    updateFile(DATA_FILE.c_str());

    return;
}

void MainWindow::on_teleOpEdit2_clicked(bool checked)
{
    allTeams[currentTeamIndex].setTeleOpElement(1 , checked);
    updateFile(DATA_FILE.c_str());

    return;
}

void MainWindow::on_teleOpEdit3_clicked(bool checked)
{
    allTeams[currentTeamIndex].setTeleOpElement(2 , checked);
    updateFile(DATA_FILE.c_str());

    return;
}

void MainWindow::on_autoEdit1_clicked(bool checked)
{
    allTeams[currentTeamIndex].setAutoElement(0 , checked);
    updateFile(DATA_FILE.c_str());

    return;
}

void MainWindow::on_autoEdit2_clicked(bool checked)
{
    allTeams[currentTeamIndex].setAutoElement(1 , checked);
    updateFile(DATA_FILE.c_str());

    return;
}

void MainWindow::on_autoEdit3_clicked(bool checked)
{
    allTeams[currentTeamIndex].setAutoElement(2 , checked);
    updateFile(DATA_FILE.c_str());

    return;
}


// Path Drawing
void MainWindow::on_addPath_clicked()
{
    // Add path to current Team object
    allTeams[currentTeamIndex].addToPathList(scene->getPath());

    // Update file
    updateFile(DATA_FILE.c_str());

    // Reset scene
    drawField();

    // Disable the addPath button to avoid breaking data file
    ui->addPath->setEnabled(false);

    // Reset UI
    refreshUI(currentTeamIndex);

    return;
}

void MainWindow::on_clearPath_clicked()
{
    // Reset scene
    drawField();

    // Clears autonomous compare
    ui->yourTeamAutos->setCurrentIndex(0);
    ui->theirTeamAutos->setCurrentIndex(0);

    // Disable the addPath button to avoid breaking data file
    ui->addPath->setEnabled(false);

    return;
}

void MainWindow::on_removePathButton_clicked()
{
    // Remove path from Team object
    allTeams[currentTeamIndex].erasePath(ui->pathsBox->currentIndex() - 1);

    // Remove path from combobox
    ui->pathsBox->removeItem(ui->pathsBox->currentIndex());

    // Update data file
    updateFile(DATA_FILE.c_str());

    // Update UI to reflect changes
    refreshUI(currentTeamIndex);

    return;
}

void MainWindow::on_removeAllPathsButton_clicked()
{
    // Deletes all paths of current Team object
    allTeams[currentTeamIndex].eraseAllPaths();

    // Clear all paths from combobox and add "New" element
    ui->pathsBox->clear();
    ui->pathsBox->addItem("New");

    // Update data file
    updateFile(DATA_FILE.c_str());

    // Update UI to reflect changes
    refreshUI(currentTeamIndex);

    return;
}

void MainWindow::on_pathsBox_currentIndexChanged(int index)
{
    // Reset scene
    drawField();

    // If the selection is actually a path index
    if(index > 0)
    {
        // Draw path, disallow drawing, and allow for removability of path
        drawAutoPath(currentTeamIndex , index - 1 , "black");
        scene->allowDraw = false;
        ui->removePathButton->setEnabled(true);
    }

    // Otherwise, don't allow path to be removed
    else
        ui->removePathButton->setEnabled(false);

    return;
}

void MainWindow::on_yourTeamAutos_currentIndexChanged(int index)
{
    // Refresh scene
    drawField();

    // If your team's path is selected, draw in blue
    if(index > 0)
        drawAutoPath(0 , index - 1 , "blue");

    // If their team's path is selected, draw in red
    if(ui->theirTeamAutos->currentIndex() > 0)
        drawAutoPath(currentTeamIndex , ui->theirTeamAutos->currentIndex() - 1 , "red");

    return;
}

void MainWindow::on_theirTeamAutos_currentIndexChanged(int index)
{
    // Refresh scene
    drawField();

    // If their team's path is selected, draw in red
    if(index > 0)
        drawAutoPath(currentTeamIndex , index - 1 , "red");

    // If your team's path is selected, draw in blue
    if(ui->yourTeamAutos->currentIndex() > 0)
        drawAutoPath(0 , ui->yourTeamAutos->currentIndex() - 1 , "blue");

    return;
}

void MainWindow::on_drawModeTabs_currentChanged(int index)
{
    // Refresh scene
    drawField();

    // If the UI is in draw mode, allow drawing
    if(index == 0)
        scene->allowDraw = true;

    // If the UI is in compare mode, disallow drawing
    else
        scene->allowDraw = false;

    return;
}


// Custom slots
void MainWindow::enableAddButton()
{
    // Allow the addPath button to be pressed
    ui->addPath->setEnabled(true);

    return;
}

void MainWindow::saveOtherData()
{
    // Saves data from Other field to Team object and data file.
    allTeams[currentTeamIndex].setOther(ui->otherEditEdit->toPlainText().toStdString());
    updateFile(DATA_FILE.c_str());

    return;
}
