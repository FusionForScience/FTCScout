#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "team.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QListWidgetItem>
#include <QInputDialog>
#include <fstream>
#include <QDebug>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructors
    explicit MainWindow(QWidget *parent = 0);

    // Destructors
    ~MainWindow();

    // Get and set functions
    vector <Team> getAllTeams();

private slots:

    // UI slots
    // Team list
    void on_addButton_clicked();
    void on_deleteTeamButton_clicked();
    void on_refreshTeamListButton_clicked();
    void on_yourTeamButton_clicked();
    void on_teamList_itemClicked(QListWidgetItem *item);

    // Editing
    void on_nameEditEdit_editingFinished();
    void on_placeEditEdit_editingFinished();
    void on_teleOpEdit1_clicked(bool checked);
    void on_teleOpEdit2_clicked(bool checked);
    void on_teleOpEdit3_clicked(bool checked);
    void on_autoEdit1_clicked(bool checked);
    void on_autoEdit2_clicked(bool checked);
    void on_autoEdit3_clicked(bool checked);
    void on_otherEditEdit_textChanged();

    // Path drawing
    void on_addPath_clicked();
    void on_clearPath_clicked();
    void on_removePathButton_clicked();
    void on_removeAllPathsButton_clicked();
    void on_pathsBox_currentIndexChanged(int index);
    void on_yourTeamAutos_currentIndexChanged(int index);
    void on_theirTeamAutos_currentIndexChanged(int index);
    void on_drawModeTabs_currentChanged(int index);

    // Custom Slots
    void enableAddButton();
    void saveOtherData();

private:
    // Constants
    const string FIELD_IMAGE = ":/img/Resources/img/field.PNG";
    const string DATA_FILE = "C:/Users/dogea/Desktop/data.txt";

    // Variables
    Ui::MainWindow *ui;
    DrawPath* scene;
    QImage *field;
    QPixmap *item;

    vector <Team> allTeams;

    int currentTeamIndex;

    // Function prototypes
    void drawField();
    void drawAutoPath(int teamIndex , int autoIndex , QString color);
    void addTeam(Team team);
    void updateFile(QString filename);
    void refreshUI(int teamIndex);

    // Event filter
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
