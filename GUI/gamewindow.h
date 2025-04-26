#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QVector>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QTableWidget>
#include <QHeaderView> // Added for table header operations
#include <QRegularExpression> // Added for parsing history strings
#include "gamelogic.h"
#include "userauth.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void cellClicked();
    void resetGame();
    void showLoginScreen();
    void showRegisterScreen();
    void showGameScreen();
    void loginUser();
    void registerUser();
    void toggleGameMode();
    void showGameHistory();
    void handleGameOver(GameResult result);
    void showGameModeDialog();

private:
    // UI Components
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;

    // Game screen
    QWidget *gameScreen;
    QVector<QPushButton*> cells;
    QLabel *statusLabel;
    QPushButton *resetButton;
    QPushButton *gameModeButton;
    bool vsAI;

    // Login screen
    QWidget *loginScreen;
    QLineEdit *loginUsername;
    QLineEdit *loginPassword;
    QPushButton *loginButton;
    QPushButton *goToRegisterButton;

    // Register screen
    QWidget *registerScreen;
    QLineEdit *registerUsername;
    QLineEdit *registerPassword;
    QLineEdit *registerConfirmPassword;
    QPushButton *registerButton;
    QPushButton *goToLoginButton;

    // Game history screen
    QWidget *historyScreen;

    // Menu
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *userMenu;
    QAction *logoutAction;
    QAction *historyAction;

    // Core components
    GameLogic *gameLogic;
    UserAuth *userAuth;

    void setupUI();
    void setupMenus();
    void setupGameBoard();
    void setupLoginScreen();
    void setupRegisterScreen();
    void updateBoardUI();
    void makeAIMove();
    void applyStyleSheet();
    void highlightWinningCells();
    bool loggedIn;
    QString currentUser;
};

#endif // GAMEWINDOW_H
