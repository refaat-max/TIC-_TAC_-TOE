#include "gamewindow.h"
#include <QDebug>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), vsAI(true), loggedIn(false)
{
    gameLogic = new GameLogic();
    userAuth = new UserAuth();

    // Set a consistent stylesheet for the QMainWindow to override system theme
    this->setStyleSheet("QMainWindow {"
                        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                        "stop:0 #1a1a2e, stop:1 #16213e);"
                        "}");

    setupUI();
    setupMenus();

    // By default, show the login screen
    showLoginScreen();
}

GameWindow::~GameWindow()
{
    delete gameLogic;
    delete userAuth;
}

void GameWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Already has a stylesheet, but ensure it’s applied correctly
    centralWidget->setStyleSheet("QWidget#centralWidget {"
                                 "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                 "stop:0 #1a1a2e, stop:1 #16213e);"
                                 "}");

    stackedWidget = new QStackedWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(stackedWidget);

    // Setup different screens
    setupGameBoard();
    setupLoginScreen();
    setupRegisterScreen();

    // Add to stacked widget
    gameScreen = new QWidget();
    QVBoxLayout *gameLayout = new QVBoxLayout(gameScreen);

    // Status label
    statusLabel = new QLabel("X's turn");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("QLabel {"
                               "color: #ffffff;"
                               "background-color: rgba(34, 40, 49, 0.8);"
                               "padding: 10px;"
                               "border-radius: 5px;"
                               "font-size: 16px;"
                               "font-weight: bold;"
                               "}");
    gameLayout->addWidget(statusLabel);

    // Game grid
    QGridLayout *gridLayout = new QGridLayout();
    for (int i = 0; i < 9; i++) {
        QPushButton *cell = new QPushButton("");
        cell->setFixedSize(100, 100);
        QFont font = cell->font();
        font.setPointSize(24);
        cell->setFont(font);
        cell->setStyleSheet("QPushButton {"
                            "background-color: #222831;"
                            "color: #ffffff;"
                            "border: 2px solid #00adb5;"
                            "border-radius: 5px;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #393e46;"
                            "}");
        connect(cell, &QPushButton::clicked, this, &GameWindow::cellClicked);
        cells.append(cell);
        gridLayout->addWidget(cell, i / 3, i % 3);
    }
    gameLayout->addLayout(gridLayout);

    // Game controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    resetButton = new QPushButton("Reset Game");
    resetButton->setStyleSheet("QPushButton {"
                               "background-color: #00adb5;"
                               "color: #ffffff;"
                               "padding: 8px 16px;"
                               "border: none;"
                               "border-radius: 5px;"
                               "font-size: 14px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #00d4dd;"
                               "}");
    connect(resetButton, &QPushButton::clicked, this, &GameWindow::resetGame);

    gameModeButton = new QPushButton(vsAI ? "Playing vs AI" : "Playing vs Human");
    gameModeButton->setStyleSheet("QPushButton {"
                                  "background-color: #00adb5;"
                                  "color: #ffffff;"
                                  "padding: 8px 16px;"
                                  "border: none;"
                                  "border-radius: 5px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #00d4dd;"
                                  "}");
    connect(gameModeButton, &QPushButton::clicked, this, &GameWindow::toggleGameMode);

    controlsLayout->addWidget(resetButton);
    controlsLayout->addWidget(gameModeButton);

    gameLayout->addLayout(controlsLayout);
    stackedWidget->addWidget(gameScreen);
}

void GameWindow::setupMenus()
{
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    fileMenu = menuBar->addMenu("File");
    QAction *exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    userMenu = menuBar->addMenu("User");
    logoutAction = userMenu->addAction("Logout");
    connect(logoutAction, &QAction::triggered, this, &GameWindow::showLoginScreen);

    historyAction = userMenu->addAction("Game History");
    connect(historyAction, &QAction::triggered, this, &GameWindow::showGameHistory);

    userMenu->setEnabled(false); // Disabled until user logs in

    // Style the menu bar comprehensively to avoid system theme influence
    menuBar->setStyleSheet("QMenuBar {"
                           "background-color: #222831;"
                           "color: #ffffff;"
                           "border: none;"
                           "padding: 5px;"
                           "}"
                           "QMenuBar::item {"
                           "background-color: #222831;"
                           "color: #ffffff;"
                           "padding: 5px 10px;"
                           "}"
                           "QMenuBar::item:selected {"
                           "background-color: #00adb5;"
                           "color: #ffffff;"
                           "}");

    fileMenu->setStyleSheet("QMenu {"
                            "background-color: #222831;"
                            "color: #ffffff;"
                            "border: 1px solid #00adb5;"
                            "}"
                            "QMenu::item {"
                            "background-color: #222831;"
                            "color: #ffffff;"
                            "padding: 5px 20px;"
                            "}"
                            "QMenu::item:selected {"
                            "background-color: #00adb5;"
                            "color: #ffffff;"
                            "}");

    userMenu->setStyleSheet("QMenu {"
                            "background-color: #222831;"
                            "color: #ffffff;"
                            "border: 1px solid #00adb5;"
                            "}"
                            "QMenu::item {"
                            "background-color: #222831;"
                            "color: #ffffff;"
                            "padding: 5px 20px;"
                            "}"
                            "QMenu::item:selected {"
                            "background-color: #00adb5;"
                            "color: #ffffff;"
                            "}");
}

void GameWindow::setupGameBoard()
{
    // Game board setup is handled in setupUI
}

void GameWindow::setupLoginScreen()
{
    loginScreen = new QWidget();
    QVBoxLayout *loginLayout = new QVBoxLayout(loginScreen);

    // Title label with larger font and modern color
    QLabel *titleLabel = new QLabel("Tic Tac Toe");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "color: #00adb5;"
                              "margin-bottom: 10px;"
                              "}");

    // Welcome message label
    QLabel *welcomeLabel = new QLabel("Welcome to the Game!");
    QFont welcomeFont = welcomeLabel->font();
    welcomeFont.setPointSize(14);
    welcomeLabel->setFont(welcomeFont);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("QLabel {"
                                "color: #e6e6e6;"
                                "margin-bottom: 20px;"
                                "}");

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);

    // Create individual labels for Username and Password with explicit styling
    QLabel *usernameLabel = new QLabel("Username:");
    usernameLabel->setStyleSheet("QLabel { color: #ffffff; }");

    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("QLabel { color: #ffffff; }");

    loginUsername = new QLineEdit();
    loginPassword = new QLineEdit();
    loginPassword->setEchoMode(QLineEdit::Password);
    loginUsername->setStyleSheet("QLineEdit {"
                                 "background-color: #e6e6e6;"
                                 "color: #222831;"
                                 "border: 1px solid #00adb5;"
                                 "border-radius: 5px;"
                                 "padding: 5px;"
                                 "}");
    loginPassword->setStyleSheet("QLineEdit {"
                                 "background-color: #e6e6e6;"
                                 "color: #222831;"
                                 "border: 1px solid #00adb5;"
                                 "border-radius: 5px;"
                                 "padding: 5px;"
                                 "}");

    formLayout->addRow(usernameLabel, loginUsername);
    formLayout->addRow(passwordLabel, loginPassword);

    loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("QPushButton {"
                               "background-color: #00adb5;"
                               "color: #ffffff;"
                               "padding: 8px 16px;"
                               "border: none;"
                               "border-radius: 5px;"
                               "font-size: 14px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #00d4dd;"
                               "}");
    connect(loginButton, &QPushButton::clicked, this, &GameWindow::loginUser);

    goToRegisterButton = new QPushButton("Register New Account");
    goToRegisterButton->setStyleSheet("QPushButton {"
                                      "background-color: #00adb5;"
                                      "color: #ffffff;"
                                      "padding: 8px 16px;"
                                      "border: none;"
                                      "border-radius: 5px;"
                                      "font-size: 14px;"
                                      "}"
                                      "QPushButton:hover {"
                                      "background-color: #00d4dd;"
                                      "}");
    connect(goToRegisterButton, &QPushButton::clicked, this, &GameWindow::showRegisterScreen);

    loginLayout->addWidget(titleLabel);
    loginLayout->addWidget(welcomeLabel);
    loginLayout->addLayout(formLayout);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(goToRegisterButton);

    stackedWidget->addWidget(loginScreen);
}

void GameWindow::setupRegisterScreen()
{
    registerScreen = new QWidget();
    QVBoxLayout *registerLayout = new QVBoxLayout(registerScreen);

    QLabel *titleLabel = new QLabel("Create New Account");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "color: #ffffff;"
                              "margin-bottom: 20px;"
                              "}");

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);

    // Create individual labels with explicit styling
    QLabel *usernameLabel = new QLabel("Username:");
    usernameLabel->setStyleSheet("QLabel { color: #ffffff; }");

    QLabel *passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("QLabel { color: #ffffff; }");

    QLabel *confirmPasswordLabel = new QLabel("Confirm Password:");
    confirmPasswordLabel->setStyleSheet("QLabel { color: #ffffff; }");

    registerUsername = new QLineEdit();
    registerPassword = new QLineEdit();
    registerPassword->setEchoMode(QLineEdit::Password);
    registerConfirmPassword = new QLineEdit();
    registerConfirmPassword->setEchoMode(QLineEdit::Password);
    registerUsername->setStyleSheet("QLineEdit {"
                                    "background-color: #e6e6e6;"
                                    "color: #222831;"
                                    "border: 1px solid #00adb5;"
                                    "border-radius: 5px;"
                                    "padding: 5px;"
                                    "}");
    registerPassword->setStyleSheet("QLineEdit {"
                                    "background-color: #e6e6e6;"
                                    "color: #222831;"
                                    "border: 1px solid #00adb5;"
                                    "border-radius: 5px;"
                                    "padding: 5px;"
                                    "}");
    registerConfirmPassword->setStyleSheet("QLineEdit {"
                                           "background-color: #e6e6e6;"
                                           "color: #222831;"
                                           "border: 1px solid #00adb5;"
                                           "border-radius: 5px;"
                                           "padding: 5px;"
                                           "}");

    formLayout->addRow(usernameLabel, registerUsername);
    formLayout->addRow(passwordLabel, registerPassword);
    formLayout->addRow(confirmPasswordLabel, registerConfirmPassword);

    registerButton = new QPushButton("Register");
    registerButton->setStyleSheet("QPushButton {"
                                  "background-color: #00adb5;"
                                  "color: #ffffff;"
                                  "padding: 8px 16px;"
                                  "border: none;"
                                  "border-radius: 5px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #00d4dd;"
                                  "}");
    connect(registerButton, &QPushButton::clicked, this, &GameWindow::registerUser);

    goToLoginButton = new QPushButton("Back to Login");
    goToLoginButton->setStyleSheet("QPushButton {"
                                   "background-color: #00adb5;"
                                   "color: #ffffff;"
                                   "padding: 8px 16px;"
                                   "border: none;"
                                   "border-radius: 5px;"
                                   "font-size: 14px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: #00d4dd;"
                                   "}");
    connect(goToLoginButton, &QPushButton::clicked, this, &GameWindow::showLoginScreen);

    registerLayout->addWidget(titleLabel);
    registerLayout->addLayout(formLayout);
    registerLayout->addWidget(registerButton);
    registerLayout->addWidget(goToLoginButton);

    stackedWidget->addWidget(registerScreen);
}

void GameWindow::showLoginScreen()
{
    loggedIn = false;
    currentUser = "";
    userMenu->setEnabled(false);
    stackedWidget->setCurrentWidget(loginScreen);
    loginUsername->clear();
    loginPassword->clear();
}

void GameWindow::showRegisterScreen()
{
    stackedWidget->setCurrentWidget(registerScreen);
    registerUsername->clear();
    registerPassword->clear();
    registerConfirmPassword->clear();
}

void GameWindow::showGameScreen()
{
    stackedWidget->setCurrentWidget(gameScreen);
    resetGame();
}

void GameWindow::loginUser()
{
    QString username = loginUsername->text();
    QString password = loginPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Please enter both username and password.");
        return;
    }

    if (userAuth->login(username, password)) {
        loggedIn = true;
        currentUser = username;
        userMenu->setEnabled(true);
        showGameModeDialog();
    } else {
        QMessageBox::warning(this, "Login Error", "Invalid username or password.");
    }
}

void GameWindow::showGameModeDialog()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Select Game Mode");
    msgBox.setText("Choose your opponent:");
    msgBox.setStyleSheet(
        "QMessageBox {"
        "background-color: #1a1a2e;"
        "color: #ffffff;"
        "}"
        "QLabel {"
        "color: #ffffff;"
        "font-size: 16px;"
        "font-family: Arial;"
        "}"
        "QPushButton {"
        "background-color: #00adb5;"
        "color: #ffffff;"
        "padding: 8px 16px;"
        "border: none;"
        "border-radius: 5px;"
        "font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "background-color: #00d4dd;"
        "}"
        );
    QPushButton *humanButton = msgBox.addButton("Play vs Human", QMessageBox::AcceptRole);
    QPushButton *aiButton = msgBox.addButton("Play vs AI", QMessageBox::AcceptRole);

    msgBox.exec();

    if (msgBox.clickedButton() == humanButton) {
        vsAI = false;
    } else if (msgBox.clickedButton() == aiButton) {
        vsAI = true;
    }

    gameModeButton->setText(vsAI ? "Playing vs AI" : "Playing vs Human");
    showGameScreen();
}

void GameWindow::registerUser()
{
    QString username = registerUsername->text();
    QString password = registerPassword->text();
    QString confirmPass = registerConfirmPassword->text();

    if (username.isEmpty() || password.isEmpty() || confirmPass.isEmpty()) {
        QMessageBox::warning(this, "Registration Error", "Please fill all fields.");
        return;
    }

    if (password != confirmPass) {
        QMessageBox::warning(this, "Registration Error", "Passwords do not match.");
        return;
    }

    if (userAuth->registerUser(username, password)) {
        QMessageBox::information(this, "Registration Success", "Account created successfully. You can now log in.");
        showLoginScreen();
    } else {
        QMessageBox::warning(this, "Registration Error", "Username already exists.");
    }
}

void GameWindow::toggleGameMode()
{
    vsAI = !vsAI;
    gameModeButton->setText(vsAI ? "Playing vs AI" : "Playing vs Human");
    resetGame();
}

void GameWindow::resetGame()
{
    gameLogic->resetGame();
    updateBoardUI();
    statusLabel->setText("X's turn");
}

void GameWindow::cellClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    int cellIndex = cells.indexOf(clickedButton);

    if (cellIndex == -1 || !gameLogic->isCellEmpty(cellIndex)) {
        return;
    }

    gameLogic->makeMove(cellIndex);
    updateBoardUI();

    GameResult result = gameLogic->checkGameStatus();
    if (result != GAME_ONGOING) {
        handleGameOver(result);
        return;
    }

    // Update status
    statusLabel->setText(gameLogic->getCurrentPlayer() == PLAYER_X ? "X's turn" : "O's turn");

    // If AI mode and it's O's turn, make AI move
    if (vsAI && gameLogic->getCurrentPlayer() == PLAYER_O) {
        makeAIMove();
    }
}

void GameWindow::makeAIMove()
{
    int aiMove = gameLogic->getBestMove();
    if (aiMove != -1) {
        gameLogic->makeMove(aiMove);
        updateBoardUI();

        GameResult result = gameLogic->checkGameStatus();
        if (result != GAME_ONGOING) {
            handleGameOver(result);
            return;
        }

        statusLabel->setText("X's turn");
    }
}

void GameWindow::updateBoardUI()
{
    for (int i = 0; i < 9; i++) {
        Cell cellState = gameLogic->getCellState(i);
        switch (cellState) {
        case CELL_X:
            cells[i]->setText("X");
            cells[i]->setStyleSheet("QPushButton {"
                                    "background-color: #222831;"
                                    "color: #ff6b6b;" // Red for X
                                    "border: 2px solid #00adb5;"
                                    "border-radius: 5px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "background-color: #393e46;"
                                    "}");
            break;
        case CELL_O:
            cells[i]->setText("O");
            cells[i]->setStyleSheet("QPushButton {"
                                    "background-color: #222831;"
                                    "color: #ffd60a;" // Yellow for O
                                    "border: 2px solid #00adb5;"
                                    "border-radius: 5px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "background-color: #393e46;"
                                    "}");
            break;
        case CELL_EMPTY:
            cells[i]->setText("");
            cells[i]->setStyleSheet("QPushButton {"
                                    "background-color: #222831;"
                                    "color: #ffffff;"
                                    "border: 2px solid #00adb5;"
                                    "border-radius: 5px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "background-color: #393e46;"
                                    "}");
            break;
        }
    }
}

void GameWindow::handleGameOver(GameResult result)
{
    QString message;
    switch (result) {
    case PLAYER_X_WINS:
        message = "X wins!";
        statusLabel->setText("X wins!");
        break;
    case PLAYER_O_WINS:
        message = "O wins!";
        statusLabel->setText("O wins!");
        break;
    case GAME_DRAW:
        message = "Game ended in a draw!";
        statusLabel->setText("Game ended in a draw!");
        break;
    default:
        return;
    }

    // Save game history if logged in
    if (loggedIn) {
        userAuth->saveGameResult(currentUser, result, vsAI);
    }

    // Show message box with result
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("Game Over");
    msgBox->setText(message);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setStyleSheet("QMessageBox {"
                          "background-color: #1a1a2e;"
                          "color: #ffffff;"
                          "}"
                          "QPushButton {"
                          "background-color: #00adb5;"
                          "color: #ffffff;"
                          "padding: 8px 16px;"
                          "border: none;"
                          "border-radius: 5px;"
                          "font-size: 14px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #00d4dd;"
                          "}"
                          "QLabel {"
                          "color: #ffffff;"
                          "}");

    // Connect OK button to resetGame slot
    connect(msgBox, &QMessageBox::accepted, this, &GameWindow::resetGame);

    msgBox->exec();
    delete msgBox;
}

void GameWindow::showGameHistory()
{
    if (!loggedIn) {
        return;
    }

    QStringList history = userAuth->getGameHistory(currentUser);
    qDebug() << "Game history for" << currentUser << ":" << history;

    QDialog *historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("Game History");
    historyDialog->setMinimumSize(600, 400);
    historyDialog->setStyleSheet("QDialog {"
                                 "background-color: #1a1a2e;"
                                 "color: #ffffff;"
                                 "}");

    QVBoxLayout *layout = new QVBoxLayout(historyDialog);

    QLabel *titleLabel = new QLabel("Game History for " + currentUser);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel {"
                              "color: #ffffff;"
                              "margin-bottom: 10px;"
                              "}");

    layout->addWidget(titleLabel);

    if (history.isEmpty()) {
        QLabel *noHistoryLabel = new QLabel("No game history available.");
        noHistoryLabel->setStyleSheet("QLabel { color: #ffffff; }");
        layout->addWidget(noHistoryLabel);
    } else {
        QTableWidget *historyTable = new QTableWidget(history.size(), 3, historyDialog);
        historyTable->setHorizontalHeaderLabels({"Date/Time", "Result", "Opponent"});
        historyTable->setStyleSheet(
            "QTableWidget {"
            "background-color: #222831;"
            "color: #ffffff;"
            "border: none;"
            "}"
            "QHeaderView::section {"
            "background-color: #00adb5;"
            "color: #ffffff;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QTableWidget::item {"
            "border: 1px solid #00adb5;"
            "padding: 5px;"
            "background-color: transparent;"
            "}"
            );
        historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        historyTable->verticalHeader()->setVisible(false);
        historyTable->setSelectionMode(QAbstractItemView::NoSelection);
        historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QRegularExpression re("\\[(.*)\\]\\s*(.*)\\s*\\(vs\\s*(.*)\\)");
        for (int row = 0; row < history.size(); ++row) {
            QString game = history[row];
            QString dateTime = "Unknown";
            QString result = "Unknown";
            QString opponent = "Unknown";

            QRegularExpressionMatch match = re.match(game);
            if (match.hasMatch()) {
                dateTime = "[" + match.captured(1) + "]";
                result = match.captured(2).trimmed();
                opponent = match.captured(3).trimmed();
            }

            QString resultLower = result.toLower();
            qDebug() << "Game" << row << "- Result:" << result << "(Normalized:" << resultLower << ")";

            QTableWidgetItem *dateItem = new QTableWidgetItem(dateTime);
            QTableWidgetItem *resultItem = new QTableWidgetItem(result);
            QTableWidgetItem *opponentItem = new QTableWidgetItem(opponent);

            if (resultLower == "x won") {
                resultItem->setBackground(QColor("#4CAF50"));
                qDebug() << "Game" << row << "- Set color to Green (Win)";
            } else if (resultLower == "o won") {
                resultItem->setBackground(QColor("#F44336"));
                qDebug() << "Game" << row << "- Set color to Red (Loss)";
            } else if (resultLower == "draw") {
                resultItem->setBackground(QColor("#FFCA28"));
                qDebug() << "Game" << row << "- Set color to Yellow (Draw)";
            } else {
                qDebug() << "Game" << row << "- No color set (Unknown result)";
            }

            dateItem->setTextAlignment(Qt::AlignCenter);
            resultItem->setTextAlignment(Qt::AlignCenter);
            opponentItem->setTextAlignment(Qt::AlignCenter);

            historyTable->setItem(row, 0, dateItem);
            historyTable->setItem(row, 1, resultItem);
            historyTable->setItem(row, 2, opponentItem);
        }

        QScrollArea *scrollArea = new QScrollArea(historyDialog);
        scrollArea->setWidget(historyTable);
        scrollArea->setWidgetResizable(true);
        scrollArea->setStyleSheet(
            "QScrollArea {"
            "background-color: #1a1a2e;"
            "border: none;"
            "}"
            "QScrollBar:vertical {"
            "background: #222831;"
            "width: 10px;"
            "margin: 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "background: #00adb5;"
            "border-radius: 5px;"
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "background: none;"
            "}"
            );
        layout->addWidget(scrollArea);
    }

    QPushButton *closeButton = new QPushButton("Close");
    closeButton->setStyleSheet("QPushButton {"
                               "background-color: #00adb5;"
                               "color: #ffffff;"
                               "padding: 8px 16px;"
                               "border: none;"
                               "border-radius: 5px;"
                               "font-size: 14px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: #00d4dd;"
                               "}");
    connect(closeButton, &QPushButton::clicked, historyDialog, &QDialog::accept);
    layout->addWidget(closeButton);

    historyDialog->exec();
    delete historyDialog;
}
