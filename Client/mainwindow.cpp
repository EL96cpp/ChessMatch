#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new Client(this))
    , rating_model(new QStandardItemModel(0, 2, this))
    , board_scene(new QGraphicsScene)
    , player_taken_figures_scene(new QGraphicsScene)
    , opponent_taken_figures_scene(new QGraphicsScene)
    , pawn_transform_scene(new QGraphicsScene) {

    ui->setupUi(this);
    setWindowTitle("Chess Match");

    SetFont();

    ui->ratingTableView->verticalHeader()->setVisible(false);
    ui->ratingTableView->setModel(rating_model);
    ui->ratingTableView->setColumnWidth(0, ui->ratingTableView->width()/2);
    ui->ratingTableView->setColumnWidth(1, ui->ratingTableView->width()/2);

    ui->ratingTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->ratingTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->boardGraphicsView->setScene(board_scene);
    ui->playerTakenView->setScene(opponent_taken_figures_scene);
    ui->opponentTakenView->setScene(player_taken_figures_scene);
    board = new Board(std::move(board_scene));
    taken_figures_manager = new TakenFiguresManager(std::move(opponent_taken_figures_scene),
                                                    std::move(player_taken_figures_scene));

    connect(this, &MainWindow::Login, client, &Client::OnLogin);
    connect(this, &MainWindow::Logout, client, &Client::OnLogout);
    connect(this, &MainWindow::Register, client, &Client::OnRegister);
    connect(this, &MainWindow::StartWaitingForOpponent, client, &Client::OnStartWaitingForOpponent);
    connect(client, &Client::ShowErrorMessage, this, &MainWindow::OnShowErrorMessage);
    connect(client, &Client::LoggedIn, this, &MainWindow::OnLoggedIn);
    connect(client, &Client::Loggedout, this, &MainWindow::OnLoggedout);
    connect(client, &Client::Registered, this, &MainWindow::OnRegistered);
    connect(client, &Client::StartWaitingForOpponentAccepted, this, &MainWindow::OnStartWaitingForOpponentAccepted);
    connect(client, &Client::GameStarted, this, &MainWindow::OnGameStarted);

    connect(client, &Client::MakeMoveAccepted, board, &Board::OnMakeMoveAccepted);
    connect(client, &Client::EatFigureAccepted, board, &Board::OnEatFigureAccepted);
    connect(client, &Client::MakeCastlingAccepted, board, &Board::OnMakeCastlingAccepted);
    connect(client, &Client::TransformPawnAccepted, board, &Board::OnTransformPawnAccepted);

    connect(board, &Board::MakeMove, client, &Client::OnMakeMove);
    connect(board, &Board::EatFigure, client, &Client::OnEatFigure);
    connect(board, &Board::MakeCastling, client, &Client::OnMakeCastling);
    connect(board, &Board::TransformPawn, client, &Client::OnTransformPawn);

    connect(board, &Board::SetMainWindowPlayerTurn, this, &MainWindow::SetPlayerTurn);
    connect(board, &Board::PlayerFigureTaken, taken_figures_manager, &TakenFiguresManager::AddPlayerTakenFigure);
    connect(board, &Board::OpponentFigureTaken, taken_figures_manager, &TakenFiguresManager::AddOpponentTakenFigure);
    connect(board, &Board::ShowTransformPawnChoice, this, &MainWindow::ShowTransformPawnChoice);
    connect(this, &MainWindow::SetPawnTransformChoice, board, &Board::SetPawnTransformChoice);
    connect(board, &Board::GameOver, this, &MainWindow::GameOver);

    connect(&waiting_dots_timer, &QTimer::timeout, this, &MainWindow::updateWaitingLabel);
    connect(&waiting_rectangles_timer, &QTimer::timeout, ui->waiting_widget, &WaitingWidget::updateRectCount);

    ui->boardGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->pawnTransformView->hide();

    //Start page

    ui->stackedWidget->setCurrentWidget(ui->start_page);

    //Login/register pages

    ui->passwordLogLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordRegLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordConfirmRegLineEdit->setEchoMode(QLineEdit::Password);

}

MainWindow::~MainWindow() {

    client->Disconnect();
    delete ui;

}

void MainWindow::SetPlayerTurn(const QString &turn) {

    ui->game_info_label->setText(turn);

}

void MainWindow::ShowTransformPawnChoice(const QString& pawn_color) {

    ui->pawnTransformView->setScene(pawn_transform_scene);
    ui->pawnTransformView->show();
    ui->pawnTransformView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (QString::compare(pawn_color, QString::fromLatin1("White"), Qt::CaseInsensitive) == 0) {

        SetWhitePawnTransformChoice();

    } else if (QString::compare(pawn_color, QString::fromLatin1("Black"), Qt::CaseInsensitive) == 0) {

        SetBlackPawnTransformChoice();

    }

}

void MainWindow::PawnTransformFigureClicked(ChessFigure* figure) {

    for (auto& figure : pawn_transform_figures) {

        pawn_transform_scene->removeItem(figure);

    }

    pawn_transform_figures.clear();
    ui->pawnTransformView->hide();
    emit SetPawnTransformChoice(figure->GetType());

}

void MainWindow::GameOver(const QString &winner_color) {

    QString winner = winner_color;
    winner += " wins!";
    ui->game_info_label->setText(winner);

}

void MainWindow::OnLoggedIn(const QString& nickname, const QString& rating, const QString& games_played, const QMap<QString, QString>& rating_values) {

    ui->nickname_value_label->setText(nickname);
    ui->rating_value_label->setText(rating);
    ui->games_played_value_label->setText(games_played);

    QStandardItem* player_item = new QStandardItem("Player");
    QStandardItem* rating_item = new QStandardItem("Rating");
    player_item->setFont(QFont(logo_font_family, 25));
    player_item->setBackground(QBrush(QColor(42, 75, 63, 50)));
    player_item->setForeground(QBrush(QColor(255, 255, 255)));
    rating_item->setFont(QFont(logo_font_family, 20));
    rating_item->setBackground(QBrush(QColor(42, 75, 63, 50)));
    rating_item->setForeground(QBrush(QColor(255, 255, 255)));

    QHeaderView* header = ui->ratingTableView->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setDefaultSectionSize(50);

    ui->top_players_label->setFont(QFont(logo_font_family));

    rating_model->setHorizontalHeaderItem(0, player_item);
    rating_model->setHorizontalHeaderItem(1, rating_item);
    ui->ratingTableView->setFont(QFont(typing_font_family, 20));

    ui->ratingTableView->setColumnWidth(0, ui->ratingTableView->width()/2);
    ui->ratingTableView->setColumnWidth(1, ui->ratingTableView->width()/2);


    for (auto [key, value] : rating_values.asKeyValueRange()) {

        QList<QStandardItem*> items;
        items.append(new QStandardItem(key));
        items.last()->setTextAlignment(Qt::AlignCenter);
        items.last()->setFlags(Qt::NoItemFlags);

        items.append(new QStandardItem(value));
        items.last()->setTextAlignment(Qt::AlignCenter);
        items.last()->setFlags(Qt::NoItemFlags);

        rating_model->appendRow(items);

    }

    ui->ratingTableView->sortByColumn(1, Qt::DescendingOrder);

    ui->nicknameLogLineEdit->clear();
    ui->passwordLogLineEdit->clear();

    ui->stackedWidget->setCurrentWidget(ui->profile_page);

}

void MainWindow::OnLoggedout() {

    ui->stackedWidget->setCurrentWidget(ui->start_page);
    rating_model->clear();

}

void MainWindow::OnRegistered(const QString &nickname) {

    ui->nicknameRegLineEdit->clear();
    ui->passwordRegLineEdit->clear();
    ui->passwordConfirmRegLineEdit->clear();

    ui->stackedWidget->setCurrentWidget(ui->start_page);

    QMessageBox::information(this, "Register success", "You've registered with nickname " + nickname);

}

void MainWindow::OnStartWaitingForOpponentAccepted() {

    ui->stackedWidget->setCurrentWidget(ui->waiting_page);

    waiting_dots_timer.setInterval(std::chrono::milliseconds(250));
    waiting_rectangles_timer.setInterval(std::chrono::milliseconds(500));

    waiting_dots_timer.start();
    waiting_rectangles_timer.start();

}

void MainWindow::OnGameStarted(const QString &player_color) {

    qDebug() << "Game started slot " << player_color;

    if (player_color == "White") {

        board->SetPlayerColor(FigureColor::WHITE);

    } else if (player_color == "Black") {

        board->SetPlayerColor(FigureColor::BLACK);

    }

    ui->game_info_label->setText("White turn");
    board->StartNewGame();
    taken_figures_manager->StartNewGame();

    ui->stackedWidget->setCurrentWidget(ui->game_page);

}

void MainWindow::OnShowErrorMessage(const QString &title, const QString &error_description) {

    QMessageBox::warning(this, title, error_description);

}


void MainWindow::on_BoardStyleComboBox_currentTextChanged(const QString &arg1) {

    if (QString::compare(arg1, QString::fromLatin1("Classic"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(255, 206, 158), QColor(209, 139, 71), QColor(54, 27, 0), QColor(0, 255, 224));

    } else if (QString::compare(arg1, QString::fromLatin1("Green"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(238, 238, 210), QColor(118, 150, 86), QColor(0, 61, 27), QColor(180, 145, 255));

    } else if (QString::compare(arg1, QString::fromLatin1("Blue"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(201, 216, 223), QColor(119, 154, 175), QColor(0, 75, 96), QColor(255, 140, 40));

    } else if (QString::compare(arg1, QString::fromLatin1("Red"), Qt::CaseInsensitive) == 0) {

        board->SetBrushes(QColor(193, 129, 130), QColor(110, 32, 32), QColor(37, 0, 27), QColor(104, 255, 84));

    }

}



void MainWindow::on_PlayerColorComboBox_currentTextChanged(const QString &player_color) {

    ui->game_info_label->setText("White turn");

    if (QString::compare(player_color, QString::fromLatin1("White"), Qt::CaseInsensitive) == 0) {

        board->SetPlayerColor(FigureColor::WHITE);

    } else {

        board->SetPlayerColor(FigureColor::BLACK);

    }

    board->StartNewGame();
    taken_figures_manager->StartNewGame();

}


void MainWindow::on_NewGameButton_clicked() {

    ui->game_info_label->setText("White turn");
    board->StartNewGame();
    taken_figures_manager->StartNewGame();

}

void MainWindow::SetFont() {

    qint32 logo_font_id = QFontDatabase::addApplicationFont(":/Fonts/LogoFont.ttf");
    qint32 typing_font_id = QFontDatabase::addApplicationFont(":/Fonts/Font.ttf");
    QStringList logo_font_list = QFontDatabase::applicationFontFamilies(logo_font_id);
    QStringList typing_font_list = QFontDatabase::applicationFontFamilies(typing_font_id);
    logo_font_family = logo_font_list.first();
    typing_font_family = typing_font_list.first();

    ui->logo_label->setFont(QFont(logo_font_family));

    ui->login_label->setFont(QFont(logo_font_family));
    ui->nickname_log_label->setFont(QFont(logo_font_family));
    ui->password_log_label->setFont(QFont(logo_font_family));

    ui->register_label->setFont(QFont(logo_font_family));
    ui->nickname_reg_label->setFont(QFont(logo_font_family));
    ui->password_reg_label->setFont(QFont(logo_font_family));
    ui->password_confirm_reg_label->setFont(QFont(logo_font_family));


    ui->startGameButton->setFont(QFont(typing_font_family));
    ui->exitProfileButton->setFont(QFont(typing_font_family));
    ui->logoutButton->setFont(QFont(typing_font_family));

    ui->nickname_label->setFont(QFont(logo_font_family));
    ui->rating_label->setFont(QFont(logo_font_family));
    ui->games_played_label->setFont(QFont(logo_font_family));

    ui->nickname_value_label->setFont(QFont(typing_font_family));
    ui->rating_value_label->setFont(QFont(typing_font_family));
    ui->games_played_value_label->setFont(QFont(typing_font_family));

    QStandardItem* player_item = new QStandardItem("Player");
    QStandardItem* rating_item = new QStandardItem("Rating");
    player_item->setFont(QFont(logo_font_family, 25));
    player_item->setBackground(QBrush(QColor(42, 75, 63, 50)));
    player_item->setForeground(QBrush(QColor(255, 255, 255)));
    rating_item->setFont(QFont(logo_font_family, 20));
    rating_item->setBackground(QBrush(QColor(42, 75, 63, 50)));
    rating_item->setForeground(QBrush(QColor(255, 255, 255)));

    QHeaderView* header = ui->ratingTableView->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setDefaultSectionSize(50);

    ui->top_players_label->setFont(QFont(logo_font_family));

    rating_model->setHorizontalHeaderItem(0, player_item);
    rating_model->setHorizontalHeaderItem(1, rating_item);
    ui->ratingTableView->setFont(QFont(typing_font_family, 20));

    ui->waiting_label->setFont(QFont(logo_font_family));
    ui->waiting_label_2->setFont(QFont(logo_font_family));

    ui->nicknameLogLineEdit->setFont(QFont(typing_font_family));
    ui->passwordLogLineEdit->setFont(QFont(typing_font_family));
    ui->nicknameRegLineEdit->setFont(QFont(typing_font_family));
    ui->passwordRegLineEdit->setFont(QFont(typing_font_family));
    ui->passwordConfirmRegLineEdit->setFont(QFont(typing_font_family));

    ui->loginButton->setFont(QFont(typing_font_family));
    ui->registerButton->setFont(QFont(typing_font_family));
    ui->exitButton->setFont(QFont(typing_font_family));
    ui->logLoginButton->setFont(QFont(typing_font_family));
    ui->logReturnButton->setFont(QFont(typing_font_family));
    ui->registerRegButton->setFont(QFont(typing_font_family));
    ui->regReturnButton->setFont(QFont(typing_font_family));

    ui->game_info_label->setFont(QFont(typing_font_family));
    ui->NewGameButton->setFont(QFont(typing_font_family));
    ui->board_style_label->setFont(QFont(typing_font_family));
    ui->player_color_label->setFont(QFont(typing_font_family));
    ui->BoardStyleComboBox->setFont(QFont(typing_font_family));
    ui->PlayerColorComboBox->setFont(QFont(typing_font_family));

}

void MainWindow::SetWhitePawnTransformChoice()
{
    int x = 0, y = 0;

    ChessFigure* rook = new Rook(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(rook, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    rook->setScale(0.6);
    pawn_transform_figures.push_back(rook);
    pawn_transform_scene->addItem(rook);

    ++y;
    ChessFigure* knight = new Knight(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(knight, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    knight->setScale(0.6);
    pawn_transform_figures.push_back(knight);
    pawn_transform_scene->addItem(knight);

    ++y;
    ChessFigure* bishop = new Bishop(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(bishop, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    bishop->setScale(0.6);
    pawn_transform_figures.push_back(bishop);
    pawn_transform_scene->addItem(bishop);

    ++y;
    ChessFigure* queen = new Queen(y, x, FigureColor::WHITE, FigureOwner::PLAYER, this);
    connect(queen, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    queen->setScale(0.6);
    pawn_transform_figures.push_back(queen);
    pawn_transform_scene->addItem(queen);

}


void MainWindow::SetBlackPawnTransformChoice()
{
    int x = 0, y = 0;

    ChessFigure* rook = new Rook(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(rook, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    rook->setScale(0.6);
    pawn_transform_figures.push_back(rook);
    pawn_transform_scene->addItem(rook);

    ++y;
    ChessFigure* knight = new Knight(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(knight, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    knight->setScale(0.6);
    pawn_transform_figures.push_back(knight);
    pawn_transform_scene->addItem(knight);

    ++y;
    ChessFigure* bishop = new Bishop(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(bishop, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    bishop->setScale(0.6);
    pawn_transform_figures.push_back(bishop);
    pawn_transform_scene->addItem(bishop);

    ++y;
    ChessFigure* queen = new Queen(y, x, FigureColor::BLACK, FigureOwner::PLAYER, this);
    connect(queen, &ChessFigure::FigureClicked, this, &MainWindow::PawnTransformFigureClicked);
    queen->setScale(0.6);
    pawn_transform_figures.push_back(queen);
    pawn_transform_scene->addItem(queen);
}



void MainWindow::on_loginButton_clicked() {

    ui->stackedWidget->setCurrentWidget(ui->login_page);

}


void MainWindow::on_registerButton_clicked() {

    ui->stackedWidget->setCurrentWidget(ui->register_page);

}


void MainWindow::on_exitButton_clicked() {

    qApp->exit();

}


void MainWindow::on_registerRegButton_clicked() {

    if (ui->nicknameRegLineEdit->text().isEmpty() ||
        ui->passwordRegLineEdit->text().isEmpty() ||
        ui->passwordConfirmRegLineEdit->text().isEmpty()) {

        QMessageBox::warning(this, "Register error", "Please, fill out all required fields");

    } else if (ui->passwordRegLineEdit->text() != ui->passwordConfirmRegLineEdit->text()) {

        QMessageBox::warning(this, "Register error", "Password confirmation does not match");

    } else {

        emit Register(ui->nicknameRegLineEdit->text(), ui->passwordRegLineEdit->text());

    }

}


void MainWindow::on_regReturnButton_clicked() {

    ui->nicknameRegLineEdit->clear();
    ui->passwordRegLineEdit->clear();
    ui->passwordConfirmRegLineEdit->clear();

    ui->stackedWidget->setCurrentWidget(ui->start_page);

}


void MainWindow::on_logReturnButton_clicked() {

    ui->nicknameLogLineEdit->clear();
    ui->passwordLogLineEdit->clear();

    ui->stackedWidget->setCurrentWidget(ui->start_page);

}


void MainWindow::on_logLoginButton_clicked() {

    if (ui->nicknameLogLineEdit->text().isEmpty() || ui->passwordLogLineEdit->text().isEmpty()) {

        QMessageBox::warning(this, "Login error", "Please, fill out all required fields");

    } else {

        emit Login(ui->nicknameLogLineEdit->text(), ui->passwordLogLineEdit->text());

    }

}


void MainWindow::on_startGameButton_clicked() {

    emit StartWaitingForOpponent();

}

void MainWindow::updateWaitingLabel() {

    QString waiting_text = ui->waiting_dots_label->text();

    if (waiting_text.count(".") < 3) {

        waiting_text.append(".");
        ui->waiting_dots_label->setText(waiting_text);

    } else {

        waiting_text.remove(".");
        ui->waiting_dots_label->setText(waiting_text);

    }

}


void MainWindow::on_waitingExitButton_clicked() {

    waiting_dots_timer.stop();
    waiting_rectangles_timer.stop();
    ui->stackedWidget->setCurrentWidget(ui->profile_page);

}


void MainWindow::on_exitProfileButton_clicked() {

    qApp->exit();

}


void MainWindow::on_logoutButton_clicked() {

    emit Logout(ui->nickname_value_label->text());

}

