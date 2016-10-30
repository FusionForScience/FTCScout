#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "team.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QDir>
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
    /**
     * @brief Main window constructor
     * @param parent A QWidget
     *
     * Not used
     */
    explicit MainWindow(QWidget *parent = 0);

    // Destructors
    ~MainWindow();


private:
    // Constants
    // Resource paths
    const string FIELD_IMAGE = ":img/field.PNG"; /**< File location of field image (fixme) */
    string DATA_FILE = "../Data"; /**< File location of data file (fixme) */

    // Variables
    Ui::MainWindow *ui;     /**< Main window object */
    DrawPath* scene;        /**< Autonomous plan scene */
    QImage *field;          /**< Background of autonomous plan scene */
    QPixmap *item;          /**< Pixmap item that is the background of the scene */

    vector <Team> allTeams; /**< Holds all the Team objects */

    int currentTeamIndex;   /**< Holds the current team being looked at */

    // Function prototypes
    /**
     * @brief Redraws the field
     *
     * @pre None
     *
     * @post Scene reset
     */
    void drawField();

    /**
     * @brief Draws autonomous path
     * @param teamIndex Index of the Team whose path we're drawing
     * @param autoIndex Index of the path we're drawing
     * @param color Color to draw the path in
     *
     * @pre None
     *
     * @post Path drawn to scene
     */
    void drawAutoPath(int teamIndex , int autoIndex , QString color);

    /**
     * @brief Add a Team object to the allTeams vector
     * @param team Team object to be added
     *
     * @pre None
     *
     * @post allTeams vector modified
     */
    void addTeam(Team team);

    /**
     * @brief Updates data file
     *
     * @pre Data file must exist and data file must be available for writing
     *
     * @post Data file updated
     */
    void updateFile();

    /**
     * @brief Refreshes UI
     * @param Index of the team we're currently looking at
     *
     * @pre Team of index teamIndex must exist
     *
     * @post Current UI updated
     */
    void refreshUI(int teamIndex);

    // Event filter
    /**
     * @brief Filters events and acts when the Other text edit goes out of focus
     * @param obj Object the event is happening to
     * @param event Event that has occured
     * @return Honestly I have no idea what this does but it works.
     *         As long as you return false, it'll work.
     *
     * @pre Object and Event must exist
     *
     * @post Team element Other and data file might be updated
     */
    bool eventFilter(QObject *obj, QEvent *event);


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
    void on_addTeamButton_clicked();

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
     * @brief Updates Name field
     *
     * When the Name: field is edited, then the corresponding team
     * is updated and the data file is updated.
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_nameEditEdit_editingFinished();

    /**
     * @brief Updates Place field
     *
     * When the Place: field is edited, then the corresponding team
     * is updated and the data file is updated
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_placeEditEdit_editingFinished();

    /**
     * @brief Updates TeleOp Action 1 field
     *
     * TeleOp action 1 checkbox is toggled, then the corresponding team
     * is updated and the data file is updated
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_teleOpEdit1_clicked(bool checked);

    /**
     * @brief Updates TeleOp Action 2 field
     *
     * TeleOp action 2 checkbox is toggled, then the corresponding team
     * is updated and the data file is updated
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_teleOpEdit2_clicked(bool checked);

    /**
     * @brief Updates TeleOp Action 3 field
     *
     * TeleOp action 3 checkbox is toggled, then the corresponding team
     * is updated and the data file is updated
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_teleOpEdit3_clicked(bool checked);

    /**
     * @brief Updates Autonomous Action 1 field
     *
     * Autonomous action 1 checkbox is toggled, then the corresponding team
     * is updated and the data file is updated
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_autoEdit1_clicked(bool checked);

    /**
     * @brief Updates Autonomous Action 2 field
     *
     * Autonomous action 2 checkbox is toggled, then the corresponding team
     * is updated and the data file is updated
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_autoEdit2_clicked(bool checked);

    /**
     * @brief Updates Autonomous Action 3 field
     *
     * Autonomous action 3 checkbox is toggled, then the corresponding team
     * is updated and the data file is updated
     *
     * @pre The current team must exist and the data file must be available for writing
     *
     * @post Current Team object updated and data file updated
     */
    void on_autoEdit3_clicked(bool checked);



    // Path drawing
    /**
     * @brief Adds an autonomous path
     *
     * Autonomous path is added to Team object and data file updated
     *
     * @pre The scene must have a drawing on it and the data file must be available for writing
     *
     * @post Current UI updated, Team object updated, and data file updated
     */
    void on_addPath_clicked();

    /**
     * @brief Clears current drawing of path
     *
     * Current drawing on scene is removed
     *
     * @pre None
     *
     * @post Current UI updated and the Add path button is disabled
     */
    void on_clearPath_clicked();

    /**
     * @brief Removes a path
     *
     * Removes path from combobox and Team object, updates data file
     *
     * @pre Current path and Team must exist and data file must be available for writing
     *
     * @post Current UI updated, Team object updated, and data file updated
     */
    void on_removePathButton_clicked();

    /**
     * @brief Removes all paths
     *
     * All paths removed from combobox and Team object, updates data file
     *
     * @pre Team must exist and data file must be available for writing
     *
     * @post Current UI updated, Team object updated, and data file updated
     */
    void on_removeAllPathsButton_clicked();

    /**
     * @brief Draws the path clicked
     * @param index Index of the path chosen to draw
     *
     * @pre Current Team and path must exist
     *
     * @post Current UI updated and the Remove Path button may change enabled state
     */
    void on_pathsBox_currentIndexChanged(int index);

    /**
     * @brief Draws the path clicked from your team's paths
     * @param index Index of the path chosen to draw
     *
     * @pre Your team and its path must exist
     *
     * @post Current UI updated
     */
    void on_yourTeamAutos_currentIndexChanged(int index);

    /**
     * @brief Draws the path clicked from chosen team's paths
     * @param index Index of the path chosen to draw
     *
     * @pre Current team and its path must exist
     *
     * @post Current UI updated
     */
    void on_theirTeamAutos_currentIndexChanged(int index);

    /**
     * @brief Allows or disallows drawing
     * @param index Number of the tab switched to
     *
     * @pre None
     *
     * @post Scene allows or disallows drawing
     */
    void on_drawModeTabs_currentChanged(int index);



    // Custom Slots
    /**
     * @brief Enables the Add path button
     *
     * @pre None
     *
     * @post Add path button enabled
     */
    void enableAddButton();

    /**
     * @brief Saves data from Other field
     *
     * @pre Data file must be available for writing
     *
     * @post Team object updated and data file updated
     */
    void saveOtherData();
};

#endif // MAINWINDOW_H
