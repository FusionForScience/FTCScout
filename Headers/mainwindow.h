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


/**
 * @brief The MainWindow class
 *
 * Holds elements and functions dealing with the main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructors
    explicit MainWindow(QWidget *parent = 0);

    // Destructors
    ~MainWindow();

private slots:
    // Team list

    /**
     * @brief Creates a new Team object
     *
     * Creats a dialog which returns user input
     * and creates a team with the number of the user input.
     *
     * @pre Data file should be accessible, read and write.
     *
     * @post Current UI changed, list of teams increases by one,
     *       and data file updated.
     */
    void on_addButton_clicked();

    /**
     * @brief Deletes a Team object from the vector.
     *
     * Removes a Team object from the vector containing every team.
     *
     * @pre Vector containing all the teams has to have a size greater than 1,
     *      data file needs to be able to write into.
     *
     * @post Current UI changed, list of teams decreases by one,
     *       and data file updated.
     */
    void on_deleteTeamButton_clicked();

    /**
     * @brief Refreshes the team list
     *
     * Scans from data file and adds/removes/reorders team list
     *
     * @pre Data file must be readable
     *
     * @post Team list may change
     */
    void on_refreshTeamListButton_clicked();

    /**
     * @brief Sets window focus on your team
     *
     * Unfocuses team list widget and displays your team's information
     * and allows editing
     *
     * @pre Vector containing all the teams has to have a size of at least 1
     *
     * @post Current UI may change, sets the current team to your team
     */
    void on_yourTeamButton_clicked();

    /**
     * @brief Sets window focus on team clicked
     * @param item Not used, immediately set to 0
     *
     * Focuses team list widget and displays clicked team's information
     * and allows editing
     *
     * @pre Vector containing all teams has to have an element corresponding
     *      to the team clicked
     *
     * @post Current UI may change, sets the current team to the team clicked
     */
    void on_teamList_itemClicked(QListWidgetItem *item);



    // Editing

    /**
     * @brief On Name: field edited
     *
     * When the Name: field is edited, then the corresponding team
     * is updated and the data file is updated.
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_nameEditEdit_editingFinished();
    void on_placeEditEdit_editingFinished();
    void on_teleOpEdit1_clicked(bool checked);
    void on_teleOpEdit2_clicked(bool checked);
    void on_teleOpEdit3_clicked(bool checked);
    void on_autoEdit1_clicked(bool checked);
    void on_autoEdit2_clicked(bool checked);
    void on_autoEdit3_clicked(bool checked);



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
    // Resource paths
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
