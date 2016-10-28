#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

#include "drawpath.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);

    ifstream myFile(DATA_FILE.c_str() , std::ios::ate);

    if((int)(myFile.tellg()) == 0)
    {
        bool ok;

        do
        {
            int number = QInputDialog::getInt(this, tr("Alert:"),
                                          tr("Enter your team number:"), 0000, 0, 99999, 0, &ok);
            if(ok)
            {
                Team defaultTeam(number);

                defaultTeam.writeToFile(DATA_FILE.c_str());
            }
        }while(!ok);
    }

    field = new QImage(FIELD_IMAGE.c_str());
    item = new QPixmap(QPixmap::fromImage(*field));
    drawField();

    on_refreshTeamListButton_clicked();

    currentTeamIndex = 0;

    refreshUI(currentTeamIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTeam(Team team)
{
    allTeams.push_back(team);
    return;
}

void MainWindow::updateFile(QString filename)
{
    remove(DATA_FILE.c_str());

    for(int i = 0; i < allTeams.size(); i ++)
    {
        allTeams[i].writeToFile(DATA_FILE.c_str());
    }
}

void MainWindow::refreshUI(int teamIndex)
{
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

    if(currentTeamIndex == 0)
        ui->isCompatible->hide();
    else
        ui->isCompatible->show();

    // Reset comboboxes
    ui->yourTeamAutos->clear();
    ui->theirTeamAutos->clear();
    ui->pathsBox->clear();

    ui->pathsBox->addItem("New");
    for(int i = 0; i < allTeams[teamIndex].getPaths().size(); i ++)
    {
        ui->pathsBox->addItem(QString::number(i + 1));
    }

    ui->yourTeamAutos->addItem("Team " + QString::number(allTeams[0].getNumber()));
    for(int i = 0; i < allTeams[0].getPaths().size(); i ++)
    {
        ui->yourTeamAutos->addItem(QString::number(i + 1));
    }

    ui->theirTeamAutos->addItem("Team " + QString::number(allTeams[teamIndex].getNumber()));
    for(int i = 0; i < allTeams[teamIndex].getPaths().size(); i ++)
    {
        ui->theirTeamAutos->addItem(QString::number(i + 1));
    }
}

vector<Team> MainWindow::getAllTeams()
{
    return allTeams;
}

void MainWindow::on_addButton_clicked()
{
    bool ok;

    int number = QInputDialog::getInt(this, tr("New Team"),
                                  tr("Enter the team number:"), 0000, 0, 99999, 0, &ok);

    if(ok)
    {
        Team defaultTeam(number);

        defaultTeam.writeToFile(DATA_FILE.c_str());

        on_refreshTeamListButton_clicked();

        ui->teamList->setCurrentRow(static_cast <int>(allTeams.size()) - 2);

        currentTeamIndex = ui->teamList->count();

        refreshUI(currentTeamIndex);
    }
}

void MainWindow::on_refreshTeamListButton_clicked()
{
    Team* newTeam = new Team();

    // Number of duplicates encountered
    int numDupes = 0;

    for(int i = static_cast <int> (allTeams.size()); newTeam->readFromFile(DATA_FILE.c_str() , i , allTeams) != 0; i ++)
    {
        if(newTeam->readFromFile(DATA_FILE.c_str() , i , allTeams) == 1)
        {
            addTeam(*newTeam);

            if(i != 0)
            {
                ui->teamList->addItem(QString::number(allTeams[i - numDupes].getNumber()));

                newTeam->writeToFile("C:/Users/dogea/Desktop/data2.txt");
            }
        }
        else if(newTeam->readFromFile(DATA_FILE.c_str() , i , allTeams) == 2)
        {
            numDupes ++;
        }
        newTeam = new Team();
    }

    return;
}

void MainWindow::drawAutoPath(int teamIndex, int autoIndex , QString color)
{
    int drawRadius = 3;

    QPen myPen;

    myPen.setColor(color);


    for(int i = 0; i < allTeams[teamIndex].getSinglePath(autoIndex).size(); i ++)
    {
        scene->addEllipse(allTeams[teamIndex].getSinglePath(autoIndex)[i].x()
                          , allTeams[teamIndex].getSinglePath(autoIndex)[i].y()
                          , drawRadius * 2.0 , drawRadius * 2.0 , myPen , QBrush(Qt::SolidPattern));
    }
}

void MainWindow::on_yourTeamAutos_currentIndexChanged(int index)
{
    drawField();

    if(index > 0)
        drawAutoPath(0 , index - 1 , "blue");

    if(ui->theirTeamAutos->currentIndex() > 0)
        drawAutoPath(currentTeamIndex , ui->theirTeamAutos->currentIndex() - 1 , "red");


}

void MainWindow::on_theirTeamAutos_currentIndexChanged(int index)
{
    drawField();

    // Draw their auto
    if(index > 0)
        drawAutoPath(currentTeamIndex , index - 1 , "red");

    if(ui->yourTeamAutos->currentIndex() > 0)
        drawAutoPath(0 , ui->yourTeamAutos->currentIndex() - 1 , "blue");
}

void MainWindow::on_yourTeamButton_clicked()
{
    ui->teamList->clearSelection();

    ui->yourTeamAutos->setCurrentIndex(0);
    ui->theirTeamAutos->setCurrentIndex(0);

    currentTeamIndex = 0;

    refreshUI(currentTeamIndex);
}

void MainWindow::drawField()
{
    delete scene;
    scene = new DrawPath();

    connect(scene , SIGNAL(userHasDrawn()) , this , SLOT(enableAddButton()));

    scene->addPixmap(*item);
    ui->autonomousPlan->setScene(scene);
}

void MainWindow::on_deleteTeamButton_clicked()
{
    allTeams.erase(allTeams.begin() + ui->teamList->currentRow() + 1);

    delete ui->teamList->currentItem();

    updateFile(DATA_FILE.c_str());

    // If there's still stuff in the list
    // Set the current team index to 1 less because
    // the list will select the item directly preceding the deleted one
    // If the list is empty, default team is selected
    if(ui->teamList->count() > 0)
        currentTeamIndex --;

    refreshUI(currentTeamIndex);

    return;
}

void MainWindow::on_drawModeTabs_currentChanged(int index)
{
    drawField();

    if(index == 0)
        scene->allowDraw = true;
    else
        scene->allowDraw = false;

    return;
}


void MainWindow::on_addPath_clicked()
{
    allTeams[currentTeamIndex].addToPathList(scene->getPath());

    updateFile(DATA_FILE.c_str());

    drawField();

    ui->addPath->setEnabled(false);

    refreshUI(currentTeamIndex);
}

void MainWindow::on_pathsBox_currentIndexChanged(int index)
{
    drawField();

    if(index > 0)
    {
        drawAutoPath(currentTeamIndex , index - 1 , "black");
        scene->allowDraw = false;
        ui->removePathButton->setEnabled(true);
    }
    else
        ui->removePathButton->setEnabled(false);
}

void MainWindow::on_teamList_itemClicked(QListWidgetItem *item)
{
    // I don't need this, and I don't like compiler warnings
    item = 0;

    // Pretty important, I'm tired, don't be a dumbass and delete this
    ui->yourTeamAutos->setCurrentIndex(0);
    ui->theirTeamAutos->setCurrentIndex(0);

    int currentRow = ui->teamList->currentRow();

    currentRow ++;

    currentTeamIndex = currentRow;

    refreshUI(currentTeamIndex);
}

void MainWindow::enableAddButton()
{
    ui->addPath->setEnabled(true);
}

void MainWindow::saveOtherData()
{
    allTeams[currentTeamIndex].setOther(ui->otherEditEdit->toPlainText().toStdString());
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_clearPath_clicked()
{
    drawField();

    ui->yourTeamAutos->setCurrentIndex(0);
    ui->theirTeamAutos->setCurrentIndex(0);

    ui->addPath->setEnabled(false);
}

void MainWindow::on_nameEditEdit_editingFinished()
{
    allTeams[currentTeamIndex].setName(ui->nameEditEdit->text().toStdString());
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_placeEditEdit_editingFinished()
{
    allTeams[currentTeamIndex].setPlace(ui->placeEditEdit->text().toInt());
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_teleOpEdit1_clicked(bool checked)
{
    allTeams[currentTeamIndex].setTeleOpElement(0 , checked);
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_teleOpEdit2_clicked(bool checked)
{
    allTeams[currentTeamIndex].setTeleOpElement(1 , checked);
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_teleOpEdit3_clicked(bool checked)
{
    allTeams[currentTeamIndex].setTeleOpElement(2 , checked);
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_autoEdit1_clicked(bool checked)
{
    allTeams[currentTeamIndex].setAutoElement(0 , checked);
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_autoEdit2_clicked(bool checked)
{
    allTeams[currentTeamIndex].setAutoElement(1 , checked);
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_autoEdit3_clicked(bool checked)
{
    allTeams[currentTeamIndex].setAutoElement(2 , checked);
    updateFile(DATA_FILE.c_str());
}

void MainWindow::on_otherEditEdit_textChanged()
{

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->otherEditEdit && event->type() == QEvent::FocusOut)
    {
        allTeams[currentTeamIndex].setOther(ui->otherEditEdit->toPlainText().toStdString());
        updateFile(DATA_FILE.c_str());
    }
    return false;
}

void MainWindow::on_removePathButton_clicked()
{
    allTeams[currentTeamIndex].erasePath(ui->pathsBox->currentIndex() - 1);

    ui->pathsBox->removeItem(ui->pathsBox->currentIndex());

    updateFile(DATA_FILE.c_str());

    refreshUI(currentTeamIndex);
}

void MainWindow::on_removeAllPathsButton_clicked()
{
    allTeams[currentTeamIndex].eraseAllPaths();

    ui->pathsBox->clear();
    ui->pathsBox->addItem("New");

    updateFile(DATA_FILE.c_str());

    refreshUI(currentTeamIndex);
}
